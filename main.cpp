#include <iostream>
#include <fstream>
#include <filesystem>
#include "commands.cpp"


int main() {
    FILE *config;
    config = fopen("config", "r+");
    if (config == nullptr) {
        config = fopen("config", "w+");
        fclose(config);
        std::ofstream out("config");
        std::string path = std::filesystem::current_path();
        out << path;
        out.close();
    }
    while (1){
        std::string str;
        std::getline(std::cin,str);
        parse_request(str.c_str());
    }
}