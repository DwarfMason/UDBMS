#include <iostream>
#include <fstream>
#include <filesystem>
#include "commands.cpp"
#include "storage_engine.h"


int main() {
    while (1){
        std::string str;
        std::getline(std::cin,str);
        std::cout << str;
        parse_request(str.c_str());
    }
}