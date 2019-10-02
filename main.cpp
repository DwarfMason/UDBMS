#include <iostream>
#include <fstream>
#include <filesystem>
#include "commands.cpp"


int main() {
    while (1){
        std::string str;
        std::getline(std::cin,str);
        std::cout << str;
        parse_request(str.c_str());
    }
}