#include <iostream>
#include <filesystem>
#include <fstream>
#include "Server.h"

#define PORT 2081

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

    Server server{};
    server.ServerInit(PORT);


    /*for (int i = 0; i < 35; i++)
        tid[i].join();*/
}

