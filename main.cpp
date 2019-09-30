#include <iostream>
#include <filesystem>

int main(int argc, char *argv[]) {
    auto kal = std::filesystem::current_path();
    /*
    TODO: переписать на std::filesystem
    FILE *config;
    config = fopen("config", "r+");
    if (config == nullptr){
        config = fopen("config", "w+");
        fwrite(argv[0], sizeof(*argv[0]), strlen(argv[0]) - 9, config);
    }
    fclose(config);
    */
    return 0;
}