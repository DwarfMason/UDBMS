#include <iostream>
#include <fstream>
#include <filesystem>
#include "commands.cpp"


int main() {
    parse_request("create table qwe(int t, int u);\n drop table qwe;\n create show table;");

    FILE *config;
    config = fopen("config", "r+");
    if (config == nullptr){
        config = fopen("config", "w+");
        fclose(config);
        std::ofstream out("config");
        std::string path = std::filesystem::current_path();
        out << path;
        out.close();
    }
    return 0;
}