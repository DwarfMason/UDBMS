#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include "parser/driver.hpp"


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
    UDBMS::Driver driver;
    driver.parse( std::cin );

}