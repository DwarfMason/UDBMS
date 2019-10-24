//
// Created by igor on 24.10.19.
//

#ifndef UDBMS_SERVER_H
#define UDBMS_SERVER_H
#include <filesystem>
#include "parser/driver.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <thread>
#include <mutex>

#define MAX_CONNECTIONS 35
#define MIN(a, b)       ((a) < (b) ? (a) : (b))

class Server {
public:
    Server() = default;

    void ServerInit(int port);
    void ClientTask(int id);
    void AcceptConnection(int id);

private:
    int _server_sock;
    /*fd_set readset;*/
    std::vector<int> connection_socket;
    std::vector<std::thread> threads;
    int _hostshort;
    struct sockaddr_in _server_addr;
};

#endif //UDBMS_SERVER_H
