#pragma once

#include <YeltsinDB/ydb.h>
#include "BaseDataWrapper.h"

class YDBDataWrapper : public BaseDataWrapper
{
public:
    explicit YDBDataWrapper(const TableMetadata& table, const std::string& filename);
    ~YDBDataWrapper() override;
    void insert_row(const Row& row) override;
    [[nodiscard]] Row get_current_row() const override;
    void delete_current_row() override;
    void update_current_row(const std::map<std::string, std::any>& kv) override;
    void next_row() override;
private:
    YDB_Engine* engine_;
    std::vector<Row> curr_page_rows_;
    size_t curr_row_idx_ = 0;
    size_t curr_page_rowcount_ = 0;
    void __read_rows();
    bool __is_curr_page_free();
    void __commit();
    //void __next_page();
};
