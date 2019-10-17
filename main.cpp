#include <iostream>
#include <filesystem>
#include "parser/driver.hpp"
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include<pthread.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *socketThread(void *arg) {
    int newSocket = *((int *) arg);

    while (true) {
        int bytes_read;
        size_t size_to_get;
        recv(newSocket, &size_to_get, sizeof(size_to_get), 0);
        char *buf = new char[size_to_get + 1];
        buf[size_to_get] = '\0';
        bytes_read = recv(newSocket, buf, size_to_get, 0);
        std::cout << std::string(buf);
        pthread_mutex_lock(&lock);
        UDBMS::Driver driver;
        std::stringstream from_client;
        from_client << std::string(buf);

        if (bytes_read <= 0) break;

        driver.parse( from_client );
        std::string str_to_send(buf);
        size_t size_to_send = str_to_send.size();
        pthread_mutex_unlock(&lock);


        send(newSocket, &size_to_send, sizeof(size_to_send), 0);
        send(newSocket, buf, size_to_send, 0);
    }
    sleep(1);

    printf("Exit socketThread \n");

    close(newSocket);

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

    int serverSocket, newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    serverSocket = socket(PF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(2096);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    if (listen(serverSocket, 50) == 0)
        printf("Listening\n");
    else
        printf("Error\n");

    pthread_t tid[60];

    int i = 0;

    while (true) {
        addr_size = sizeof serverStorage;
        newSocket = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);

        if (pthread_create(&tid[i], nullptr, socketThread, &newSocket) != 0)
            printf("Failed to create thread\n");

        if (i >= 50) {
            i = 0;
            while (i < 50) {
                pthread_join(tid[i++], nullptr);
            }
            i = 0;
        }
    }


}