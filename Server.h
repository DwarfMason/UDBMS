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

#define MAX_CONNECTIONS 10
#define MIN(a, b)       ((a) < (b) ? (a) : (b))

class Server {
public:
    Server() = default;

    void ServerInit(int port);
    void ClientTask(int id);
    void AcceptConnection(int id);

    std::stringstream buffer_cout;
    std::stringstream buffer_cerr;
    std::streambuf *old_cerr = std::cerr.rdbuf(buffer_cerr.rdbuf());
    std::streambuf *old_cout = std::cout.rdbuf(buffer_cout.rdbuf());

private:
    int _server_sock;
    int _live_connections = 0;
    int _created_connections = 0;
    std::vector<int> connection_socket;
    std::vector<std::thread> threads;
    int _hostshort;
    struct sockaddr_in _server_addr;
};

#endif //UDBMS_SERVER_H
