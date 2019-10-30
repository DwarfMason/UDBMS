//
// Created by igor on 28.10.19.
//
#include "Client.h"
#include <gtest/gtest.h>


class MyTestEnvironment {
public:
    MyTestEnvironment() = default;

    explicit MyTestEnvironment(Client *client) : _client(*client) {};

    Client &GetClient() { return this->_client; };

private:
    Client _client;
};

MyTestEnvironment env;

::testing::AssertionResult TestDBMS(const std::string& input, const std::string& expected = "") {
    std::stringstream buffer_cout;
    std::stringstream buffer_cerr;
    std::streambuf *old_cerr = std::cerr.rdbuf(buffer_cerr.rdbuf());
    std::streambuf *old_cout = std::cout.rdbuf(buffer_cout.rdbuf());

    buffer_cout.str("");
    buffer_cerr.str("");

        env.GetClient().ClientCommunication(&env.GetClient().GetSocket(), input);

        std::cout.rdbuf(old_cerr);
    std::cerr.rdbuf(old_cout);

    if (buffer_cout.str().empty())
        if (buffer_cerr.str() != expected + '\n')
            return ::testing::AssertionFailure() << "got: " << buffer_cerr.str() << "expected: " << expected << std::endl;
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

TEST(BASE_FUNCTIONALITY, TABLE_CREATION_CASE){
    TestDBMS("create table a( a integer);");
    EXPECT_TRUE(TestDBMS("show create table a;", "*************************** 1. row ***************************\n"
                                                 "       Table: a\n"
                                                 "Create Table: CREATE TABLE `a` (\n"
                                                 "  `a` INTEGER\n"
                                                 ");"));
    TestDBMS("drop table a;");
}

TEST(BASE_FUNCTIONALITY, TABLE_CREATION_UNIQUE_CASE){
    TestDBMS("create table a( a integer UNIQUE);");
    EXPECT_TRUE(TestDBMS("show create table a;", "*************************** 1. row ***************************\n"
                                                 "       Table: a\n"
                                                 "Create Table: CREATE TABLE `a` (\n"
                                                 "  `a` INTEGER UNIQUE\n"
                                                 ");"));
    TestDBMS("drop table a;");
}

TEST(BASE_FUNCTIONALITY, TABLE_EXISTS_CASE){
    TestDBMS("create table a( a integer UNIQUE);");
    EXPECT_TRUE(TestDBMS("Create table a( b float);", "Error 101: Table does already exist"));
    TestDBMS("drop table a;");
}

TEST(BASE_FUNCTIONALITY, TABLE_DOES_NOT_EXIST_CASE){
    EXPECT_TRUE(TestDBMS("drop table a;", "Error 100: Table does not exist"));
}

TEST(BASE_FUNCTIONALITY, SEVERAL_TABLES_DROP_CASE){
    TestDBMS("create table a(a integer);");
    TestDBMS("create table b(a integer);");
    TestDBMS("drop table a, b;");
    EXPECT_TRUE(TestDBMS("drop table a;", "Error 100: Table does not exist") && TestDBMS("drop table b;", "Error 100: Table does not exist"));
}
