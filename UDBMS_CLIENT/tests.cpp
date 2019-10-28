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

::testing::AssertionResult TestDBMS(std::string input, std::string expected) {
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
        if (buffer_cerr.str() != expected)
            return ::testing::AssertionFailure() << "got:" << buffer_cerr.str() << "expected" << expected << std::endl;
        else return ::testing::AssertionSuccess();
    else if (buffer_cout.str() != expected)
        return ::testing::AssertionFailure() << "got:" << buffer_cout.str() << "expected" << expected << std::endl;
    else return ::testing::AssertionSuccess();
}

TEST(PARSER_CHECK, CREATE_AND_DROP){
    EXPECT_FALSE(TestDBMS("crate table a( a integer);", "200:syntax error, unexpected NAME"));
}
