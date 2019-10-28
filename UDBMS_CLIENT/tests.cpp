//
// Created by igor on 28.10.19.
//

#include <gtest/gtest.h>
#include <string>


::testing::AssertionResult TestDBMS(std::string input, std::string output){
    std::stringstream buffer_cin;
    std::stringstream buffer_cout;
    std::stringstream buffer_cerr;
    std::streambuf *old_cerr = std::cerr.rdbuf(buffer_cerr.rdbuf());
    std::streambuf *old_cout = std::cout.rdbuf(buffer_cout.rdbuf());
    std::streambuf *old_cin = std::cin.rdbuf(buffer_cout.rdbuf());


}