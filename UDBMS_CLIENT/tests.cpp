//
// Created by igor on 28.10.19.
//
#include "Client.h"
#include <gtest/gtest.h>
#include <queue>
#include <thread>


class MyTestEnvironment {
public:
    MyTestEnvironment() = default;

    explicit MyTestEnvironment(Client *client) { _client.push_back(*client); };

    Client &GetClient(int number = 0) { return this->_client[number]; };

    void AddClient() {
        auto a = _client.size();
        _client.emplace_back(Client());
        _client.back().ClientInit(PORT);
    };

    void EraseClients() { _client.clear(); };

private:
    std::vector<Client> _client;
};

MyTestEnvironment env;

void StraightRequest(const std::string& buf, int client_num = 0){
    env.GetClient(client_num).ClientCommunication(&env.GetClient(client_num).GetSocket(), buf);
}

std::string exec(const char *cmd) {
    char buffer[128];
    std::string result = "";
    FILE *pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != nullptr) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

void KillServer(int time = 0) {
    close(env.GetClient().GetSocket());
    std::string buf = exec("pidof UDBMS");
    usleep(time);
    system("killall -s 9 UDBMS");
    env.EraseClients();
}

void KillAllServers() {
    close(env.GetClient().GetSocket());
    system("killall -s 9 UDBMS");
    env.EraseClients();
}

void StartServer() {
    close(env.GetClient().GetSocket());
    env.EraseClients();
    system("../UDBMS &");
    sleep(1);
    env.AddClient();
}


::testing::AssertionResult TestDBMS(const std::string &input, const std::string &expected = "", int client_number = 0) {
    std::stringstream buffer_cout;
    std::stringstream buffer_cerr;
    std::streambuf *old_cerr = std::cerr.rdbuf(buffer_cerr.rdbuf());
    std::streambuf *old_cout = std::cout.rdbuf(buffer_cout.rdbuf());

    buffer_cout.str("");
    buffer_cerr.str("");

    env.GetClient(client_number).ClientCommunication(&env.GetClient(client_number).GetSocket(), input);

    std::cout.rdbuf(old_cout);
    std::cerr.rdbuf(old_cerr);

    if (buffer_cout.str().empty())
        if (buffer_cerr.str() != expected + '\n')
            return ::testing::AssertionFailure() << "got: " << buffer_cerr.str() << "expected: " << expected
                                                 << std::endl;
        else return ::testing::AssertionSuccess();
    else if (buffer_cout.str() != expected + '\n')
        return ::testing::AssertionFailure() << "got: " << buffer_cout.str() << "expected: " << expected << std::endl;
    else return ::testing::AssertionSuccess();
}

TEST(PARSER_CHECK, crate_CASE) {
    EXPECT_TRUE(TestDBMS("crate table a( a integer);", "200:syntax error, unexpected NAME"));
}

TEST(PARSER_CHECK, tble_CASE) {
    EXPECT_TRUE(TestDBMS("create tble a( a integer);", "200:syntax error, unexpected NAME, expecting TABLE"));
}

TEST(PARSER_CHECK, MULTISTRING_INPUT_CASE) {
    EXPECT_TRUE(TestDBMS("create table a( a integer,\nb integer);", "Table created."));
    TestDBMS("drop table a;");
}

TEST(PARSER_CHECK, NO_NAME_CASE) {
    EXPECT_TRUE(TestDBMS("create table ( a integer);", "200:syntax error, unexpected '(', expecting NAME"));
}

TEST(PARSER_CHECK, create_NAME_CASE) {
    EXPECT_TRUE(TestDBMS("create table create_table( a integer);", "Table created."));
    TestDBMS("drop table create_table;");
}

TEST(PARSER_CHECK, NO_BRACKET_CASE) {
    EXPECT_TRUE(TestDBMS("create table create_table a integer);", "200:syntax error, unexpected NAME, expecting '('"));
    TestDBMS("drop table a;");
}

TEST(BASE_FUNCTIONALITY, TABLE_CREATION_CASE) {
    TestDBMS("create table a( a integer);");
    EXPECT_TRUE(TestDBMS("show create table a;", "*************************** 1. row ***************************\n"
                                                 "       Table: a\n"
                                                 "Create Table: CREATE TABLE `a` (\n"
                                                 "  `a` INTEGER\n"
                                                 ");"));
    TestDBMS("drop table a;");
}

TEST(BASE_FUNCTIONALITY, TABLE_CREATION_UNIQUE_CASE) {
    TestDBMS("create table a( a integer UNIQUE);");
    EXPECT_TRUE(TestDBMS("show create table a;", "*************************** 1. row ***************************\n"
                                                 "       Table: a\n"
                                                 "Create Table: CREATE TABLE `a` (\n"
                                                 "  `a` INTEGER UNIQUE\n"
                                                 ");"));
    TestDBMS("drop table a;");
}

TEST(BASE_FUNCTIONALITY, TABLE_EXISTS_CASE) {
    TestDBMS("create table a( a integer UNIQUE);");
    EXPECT_TRUE(TestDBMS("Create table a( b float);", "Error 101: Table does already exist"));
    TestDBMS("drop table a;");
}

TEST(BASE_FUNCTIONALITY, TABLE_DOES_NOT_EXIST_CASE) {
    EXPECT_TRUE(TestDBMS("drop table a;", "Error 100: Table does not exist"));
}

TEST(BASE_FUNCTIONALITY, SEVERAL_TABLES_DROP_CASE) {
    TestDBMS("create table a(a integer);");
    TestDBMS("create table b(a integer);");
    TestDBMS("drop table a, b;");
    EXPECT_TRUE(TestDBMS("drop table a;", "Error 100: Table does not exist") &&
                TestDBMS("drop table b;", "Error 100: Table does not exist"));
}

TEST(BASE_FUNCTIONALITY, LOTS_OF_COLUMNS_CASE) {
    TestDBMS("create table a(a integer,"
             "b integer,"
             "c integer,"
             "d integer,"
             "e integer,"
             "f integer,"
             "g integer,"
             "h integer,"
             "i integer,"
             "j integer,"
             "k integer,"
             "l integer,"
             "m integer,"
             "n integer,"
             "o integer,"
             "p integer,"
             "q integer,"
             "r integer,"
             "s integer,"
             "t integer,"
             "u integer,"
             "v integer,"
             "w integer,"
             "x integer,"
             "y integer,"
             "z integer);");
    EXPECT_TRUE(TestDBMS("show create table a;", "*************************** 1. row ***************************\n"
                                                 "       Table: a\n"
                                                 "Create Table: CREATE TABLE `a` (\n"
                                                 "  `a` INTEGER,\n"
                                                 "  `b` INTEGER,\n"
                                                 "  `c` INTEGER,\n"
                                                 "  `d` INTEGER,\n"
                                                 "  `e` INTEGER,\n"
                                                 "  `f` INTEGER,\n"
                                                 "  `g` INTEGER,\n"
                                                 "  `h` INTEGER,\n"
                                                 "  `i` INTEGER,\n"
                                                 "  `j` INTEGER,\n"
                                                 "  `k` INTEGER,\n"
                                                 "  `l` INTEGER,\n"
                                                 "  `m` INTEGER,\n"
                                                 "  `n` INTEGER,\n"
                                                 "  `o` INTEGER,\n"
                                                 "  `p` INTEGER,\n"
                                                 "  `q` INTEGER,\n"
                                                 "  `r` INTEGER,\n"
                                                 "  `s` INTEGER,\n"
                                                 "  `t` INTEGER,\n"
                                                 "  `u` INTEGER,\n"
                                                 "  `v` INTEGER,\n"
                                                 "  `w` INTEGER,\n"
                                                 "  `x` INTEGER,\n"
                                                 "  `y` INTEGER,\n"
                                                 "  `z` INTEGER\n"
                                                 ");"));
    TestDBMS("drop table a;");
}


TEST(DATA_FUNCTIONALITY, INSERT_VALUE_CASE) {
    TestDBMS("create table a(id integer);");
    TestDBMS("insert into a(id) values(1)");
    EXPECT_TRUE(TestDBMS("select id from a;", "+----+\n"
                                              "| id |\n"
                                              "+----+\n"
                                              "| 1  |\n"
                                              "+----+"));
    TestDBMS("drop table a;");
}

TEST(DATA_FUNCTIONALITY, NO_COLUMN_CASE) {
    TestDBMS("create table a(id integer);");
    TestDBMS("insert into a(id) values(1)");
    EXPECT_TRUE(TestDBMS("select b from a;", "303:No such column!"));
    TestDBMS("drop table a;");
}

TEST(DATA_FUNCTIONALITY, MULTI_INSERT_AND_WILDCART_CASE) {
    TestDBMS("create table a( id integer, a integer, b integer);");
    TestDBMS("insert into a (id, a) values (1, 2);");
    EXPECT_TRUE(TestDBMS("select * from a;", "+----+---+---+\n"
                                             "| id | a | b |\n"
                                             "+----+---+---+\n"
                                             "| 1  | 2 | 0 |\n"
                                             "+----+---+---+"));
    TestDBMS("drop table a;");
}

TEST(DATA_FUNCTIONALITY, REPOSITION_OF_COLUMN_NAMES_CASE) {
    TestDBMS("create table a( id integer, a integer, b integer);");
    TestDBMS("insert into a (id, a) values (1, 2);");
    EXPECT_TRUE(TestDBMS("select a,id from a;", "+---+----+\n"
                                                "| a | id |\n"
                                                "+---+----+\n"
                                                "| 2 | 1  |\n"
                                                "+---+----+"));
    TestDBMS("drop table a;");
}

TEST(DATA_FUNCTIONALITY, SEVERAL_SELECT_COLUMNS_IN_SELECT_CASE) {
    TestDBMS("create table a( id integer, a integer, b integer);");
    TestDBMS("insert into a (id, a) values (1, 2);");
    EXPECT_TRUE(TestDBMS("select id, a from a;", "+----+---+\n"
                                                 "| id | a |\n"
                                                 "+----+---+\n"
                                                 "| 1  | 2 |\n"
                                                 "+----+---+"));
    TestDBMS("drop table a;");
}

/*TEST(RANDOM_FUNC, RANDOM_CASE) {
    std::queue<std::string> callstack;
    int cnt = 0;
    Generator a;
    std::string b;
    while (!exec("pidof UDBMS").empty()) {
        b = a.GenerateRequest();
        if (callstack.size() < 25)
            callstack.push(b);
        else {
            callstack.push(b);
            callstack.pop();
        }
        TestDBMS(b);
        cnt++;
        if (cnt >= 500) break;
    }
    if (cnt >= 1000) EXPECT_TRUE(true);
    std::cout << "Server died at " << b << std::endl;
    std::cout << cnt << " Tests passed" << std::endl;
    std::cout << "==================Stack==================" << std::endl;
    for (int i = 0; i < 25; i++) {
        std::cout << i << ") " << callstack.front() << std::endl;
        callstack.pop();
    }
}*/

TEST(CONSISTENCY_TESTS, RECCONECTION_AFTER_SERVER_DIES_CASE) {
    KillServer();
    env.EraseClients();
    StartServer();
    StraightRequest("select * from a;");
    EXPECT_TRUE(TestDBMS("drop table a;", "Error 100: Table does not exist"));
    KillAllServers();
    env.EraseClients();
}

TEST(CONSISTENCY_TESTS, WORKING_SERVER_IS_KILLED) {
    StartServer();
    usleep(500);
    std::vector<bool> drop_pool;
    TestDBMS("create table a(a integer,"
             "b integer,"
             "c integer,"
             "d integer,"
             "e integer,"
             "f integer,"
             "g integer,"
             "h integer,"
             "i integer,"
             "j integer,"
             "k integer,"
             "l integer,"
             "m integer,"
             "n integer,"
             "o integer,"
             "p integer,"
             "q integer,"
             "r integer,"
             "s integer,"
             "t integer,"
             "u integer,"
             "v integer,"
             "w integer,"
             "x integer,"
             "y integer,"
             "z integer);");
    for (int i = 2; i < 40; ++i) {
        system("cp -f a.dat a1.dat");
        std::stringstream buffer_cout;
        std::streambuf *old_cout = std::cout.rdbuf(buffer_cout.rdbuf());

        buffer_cout.str("");

        StraightRequest("select * from a;");
        std::string buf = buffer_cout.str();
        std::thread a = std::thread([i](){KillServer(7.5*996200);});
        a.detach();

        TestDBMS("insert into a (a, b, c , d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z) "
                 "values(0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5);");
        TestDBMS("insert into a (a, b, c , d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z) "
                 "values(0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5);");
        TestDBMS("insert into a (a, b, c , d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z) "
                 "values(0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5);");
        TestDBMS("insert into a (a, b, c , d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z) "
                 "values(0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5);");
        sleep(1);
        StartServer();
        buffer_cout.str("");
        StraightRequest("select * from a;");
        std::string buf_after = buffer_cout.str();
        std::cout.rdbuf(old_cout);
        if(!((exec("cmp a.dat a1.dat").empty())^(strcmp(buf.c_str(), buf_after.c_str()) == 0))) {
            StraightRequest("select * from a;");
        }
    }
    for (int i = 0; i< drop_pool.size(); i++)
        std::cout << drop_pool[i];
    TestDBMS("drop table a;");
    KillAllServers();
}

