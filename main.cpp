#include <iostream>
#include <filesystem>
#include "parser/driver.hpp"
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <thread>
#include "Server.h"

#define PORT 2077

void *socket_thread(int* serverSocket) {

    struct sockaddr_storage server_storage;
    socklen_t addr_size;
    addr_size = sizeof server_storage;
    int new_socket = accept(*serverSocket, (struct sockaddr *) &server_storage, &addr_size);
    std::stringstream buffer;
    std::stringstream buffer_cout;
    std::stringstream buffer_cerr;
    std::streambuf *old_cerr = std::cerr.rdbuf(buffer_cerr.rdbuf());
    std::streambuf *old_cout = std::cout.rdbuf(buffer_cout.rdbuf());


    while (true) {
        int bytes_read;
        size_t size_to_get;

        recv(new_socket, &size_to_get, sizeof(size_to_get), 0);
        char *buf = new char[size_to_get + 1];
        buf[size_to_get] = '\0';
        bytes_read = recv(new_socket, buf, size_to_get, 0);

        buffer_cout.str("");
        buffer_cerr.str("");


     //   pthread_mutex_lock(&lock);
        UDBMS::Driver driver;
        std::stringstream from_client;
        from_client << std::string(buf);

        if (bytes_read <= 0) break;

        driver.parse(from_client);

        char *send_data = strdup(buffer_cout.str().c_str());
        char *send_cerr = strdup(buffer_cerr.str().c_str());

        size_t size_send_data = buffer_cout.str().size();
        size_t size_send_cerr = buffer_cerr.str().size();
       // pthread_mutex_unlock(&lock);

        if (size_send_data > size_send_cerr) {
            send(new_socket, &size_send_data, sizeof(size_send_data), 0);
            send(new_socket, "0", sizeof(char), 0);
            send(new_socket, send_data, size_send_data, 0);
        } else {
            send(new_socket, &size_send_cerr, sizeof(size_send_cerr), 0);
            send(new_socket, "1", sizeof(char), 0);
            send(new_socket, send_cerr, size_send_cerr, 0);
        }

    }
    sleep(1);

    printf("%d Exit socket_thread \n");

    close(new_socket);

    pthread_exit(nullptr);

}

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
        while (true){};
}

