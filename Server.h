//
// Created by igor on 24.10.19.
//

#ifndef UDBMS_SERVER_H
#define UDBMS_SERVER_H
#include <filesystem>
#include "parser/driver.hpp"
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <thread>


class Connection {
public:
    Connection() = default;
    explicit Connection(int server_sock): _server_sock(server_sock){}

    void ConnectionInit();

private:
    int _connection_sock;
    int _server_sock;

};

class Server {
public:
    Server() = default;

    void ServerInit(int port);

    int _server_sock;

private:
    Connection cid[35];
    int _hostshort;
    struct sockaddr_in _server_addr;
};

#endif //UDBMS_SERVER_H
