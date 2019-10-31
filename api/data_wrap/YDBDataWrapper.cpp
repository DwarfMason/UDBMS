#include <YeltsinDB/error_code.h>
#include <YeltsinDB/constants.h>
#include <YeltsinDB/table_page.h>
#include <exceptions.h>
#include "YDBDataWrapper.h"

YDBDataWrapper::YDBDataWrapper(const TableMetadata &table, const std::string &filename)
    : BaseDataWrapper(table, filename)
{
    engine_ = ydb_init_instance();
    YDB_Error load_status = ydb_load_table(engine_, filename.c_str());
    switch (load_status) {
        case YDB_ERR_TABLE_NOT_EXIST:
            load_status = ydb_create_table(engine_, filename.c_str());
            if (load_status != YDB_ERR_SUCCESS) {
                throw std::runtime_error("Engine exception on create, code " + std::to_string(load_status));
            }
            break;
        case YDB_ERR_SUCCESS:
            break;
        default:
            throw std::runtime_error("Engine exception on read, code " + std::to_string(load_status));
    }
    __read_rows();
}

YDBDataWrapper::~YDBDataWrapper()
{
    ydb_terminate_instance(engine_);
}

void YDBDataWrapper::insert_row(const Row &row)
{
    YDB_TablePage* p;

    int64_t initial_page_idx = ydb_tell_page(engine_);
    size_t initial_row_idx = curr_row_idx_;
    const size_t col_cnt = metadata_.get_columns().size();

    uint32_t free_mem_offset;
    uint32_t insert_row_size;

    // Count current page data size
    free_mem_offset = 0;
    for (const auto& page_row : curr_page_rows_)
    {
        free_mem_offset += sizeof(YDB_Flags) + page_row.get_size();
    }
    // Count size of row to be inserted
    insert_row_size = sizeof(YDB_Flags) + row.get_size();

    while (free_mem_offset + insert_row_size > YDB_TABLE_PAGE_SIZE - 11) // FIXME remove magic numbers
    {
        // Next page
        YDB_Error next_page_status = ydb_next_page(engine_);
        YDB_Flags flags;
        switch (next_page_status)
        {
            case YDB_ERR_SUCCESS:
                break;
            case YDB_ERR_NO_MORE_PAGES:
                // Allocate a new page and return;
                p = ydb_page_alloc(YDB_TABLE_PAGE_SIZE - 11); // FIXME magic numbers
                ydb_page_flags_set(p, 0);
                ydb_page_row_count_set(p, 0);
                ydb_append_page(engine_, p);
                break;
            default:
                throw std::runtime_error("Unhandled error, code" + std::to_string(next_page_status));
        }
        __read_rows();

        // Count current page data size
        free_mem_offset = 0;
        for (const auto& page_row : curr_page_rows_)
        {
            free_mem_offset += sizeof(YDB_Flags) + page_row.get_size();
        }
        // Count size of row to be inserted
        insert_row_size = sizeof(YDB_Flags) + row.get_size();
    };

    curr_page_rows_.push_back(row);
    __commit();

    // Go back
    ydb_seek_page(engine_, initial_page_idx);
    __read_rows();
    curr_row_idx_ = initial_page_idx;
}

void YDBDataWrapper::__read_rows()
{
    YDB_TablePage* p = ydb_page_clone(ydb_get_current_page(engine_));
    if (p == nullptr)
        throw std::runtime_error("ПРОИЗОШЁЛ ТРОЛЛИНГ, СТРАНИЦЫ НЕ БУДЕТ.........");

    size_t page_mem_size = YDB_TABLE_PAGE_SIZE - 11; // This magic shouldn't be there...
    size_t page_mem_read = 0;

    char data[page_mem_size];
    ydb_page_data_seek(p, 0);
    ydb_page_data_read(p, data, sizeof(data));

    curr_page_rows_.clear();
    curr_page_rowcount_ = ydb_page_row_count_get(p);
    size_t rows_read = 0;
    curr_row_idx_ = 0;

    ydb_page_free(p);

    size_t row_size = 0; // don't count row flags
    for (const auto& c : metadata_.get_columns())
    {
        row_size += c.get_phys_size();
    }

    while (rows_read != curr_page_rowcount_)
    {
        if (page_mem_read >= page_mem_size)
        {
            throw std::runtime_error("Page memory exceeded. The table data is probably corrupted.");
        }
        YDB_Flags row_flags = 0;
        memcpy(&row_flags, data + page_mem_read, sizeof(row_flags)); // In case flag size is more than 1
        page_mem_read += sizeof(row_flags);

        bool row_deleted = row_flags & YDB_TABLE_PAGE_FLAG_DELETED; // FIXME wrong flag

        std::vector<Cell> cells;

        for (const auto& c : metadata_.get_columns())
        {
            Cell cell(c.get_type(), c.get_phys_size());
            char cell_val_raw[c.get_phys_size()];
            memcpy(cell_val_raw, data + page_mem_read, sizeof(cell_val_raw));
            page_mem_read += sizeof(cell_val_raw);
            cell.from_raw(cell_val_raw);
            cells.push_back(cell);
        }
        curr_page_rows_.emplace_back(cells);

        if (row_deleted)
            curr_page_rows_.rbegin()->mark_as_deleted();

        ++rows_read;
    }
}

Row YDBDataWrapper::get_current_row() const
{
    if (curr_page_rows_.empty())
        throw cursor_eof_error();

    return curr_page_rows_.at(curr_row_idx_);
}

void YDBDataWrapper::delete_current_row()
{
    curr_page_rows_[curr_row_idx_].mark_as_deleted();
    __commit();
}

bool YDBDataWrapper::__is_curr_page_free()
{
    for (const auto& r : curr_page_rows_)
    {
        if (!r.is_deleted()) return false;
    }
    return true;
}

void YDBDataWrapper::__commit()
{
    YDB_TablePage* p = ydb_page_clone(ydb_get_current_page(engine_));
    if (p == nullptr)
        throw std::runtime_error("ПРОИЗОШЁЛ ТРОЛЛИНГ, СТРАНИЦЫ НЕ БУДЕТ.........\n"
                                 "Коммита, кстати, тоже.");

    // Set row count in page
    ydb_page_row_count_set(p, curr_page_rows_.size());

    // Rewind page
    ydb_page_data_seek(p, 0);

    // Get columns count to iterate over row cells
    const size_t col_cnt = metadata_.get_columns().size();

    // Page delete flag
    bool delete_page = true;
    // Page write status
    YDB_Error page_write_status;
    for (const auto& row : curr_page_rows_)
    {
        YDB_Flags flags = 0;

        // If at least one row is not deleted, unmark page
        if (!row.is_deleted())
        {
            delete_page = false;
        }
        else
        {
            // If row is deleted, set a flag
            flags += YDB_TABLE_PAGE_FLAG_DELETED; // FIXME wrong flag
        }
        // Write row flags
        page_write_status = ydb_page_data_write(p, &flags, sizeof(flags));
        // Check for errors
        switch (page_write_status)
        {
            case YDB_ERR_SUCCESS:
                break;
            case YDB_ERR_PAGE_NO_MORE_MEM:
            case YDB_ERR_PAGE_INDEX_OUT_OF_RANGE:
                throw std::runtime_error("Out of memory in page."); // TODO handle this
            default:
                throw std::runtime_error("Unhandled error, code" + std::to_string(page_write_status));
        }
        // Iterate through row cells
        for (size_t i = 0; i < col_cnt; ++i)
        {
            // Get cell value and physical size
            const auto& cell = row.get_at(i);
            const void* data = cell.to_raw();
            size_t data_size = cell.get_size();
            // Write it to page
            page_write_status = ydb_page_data_write(p, data, data_size);
            switch (page_write_status)
            {
                case YDB_ERR_SUCCESS:
                    break;
                case YDB_ERR_PAGE_NO_MORE_MEM:
                case YDB_ERR_PAGE_INDEX_OUT_OF_RANGE:
                    throw std::runtime_error("Out of memory in page."); // TODO handle this
                default:
                    throw std::runtime_error("Unhandled error, code" + std::to_string(page_write_status));
            }
        }
    }
    // Rewrite page
    ydb_replace_current_page(engine_, p);

    // Delete page if marked
    if (delete_page)
    {
        ydb_delete_current_page(engine_);
        __read_rows();
    }
}

void YDBDataWrapper::next_row()
{
    if (++curr_row_idx_ >= curr_page_rows_.size())
    {
        YDB_Error err = ydb_next_page(engine_);
        __read_rows();
        if (err == YDB_ERR_NO_MORE_PAGES) {
            throw cursor_eof_error();
        }
    }
}

void YDBDataWrapper::update_current_row(const std::map<std::string, std::any> &kv)
{
    for (const auto& kv_pair : kv)
    {
        size_t index = metadata_.get_index_by_name(kv_pair.first);
        const std::any& value = kv_pair.second;
        curr_page_rows_[curr_row_idx_].set_at(index, value);
    }
    __commit();
}
