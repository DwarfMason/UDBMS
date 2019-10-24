//
// Created by igor on 24.10.19.
//

#include "Server.h"

void Server::ServerInit(int port){
    this->_hostshort = port;
    this->_server_sock = socket(PF_INET, SOCK_STREAM, 0);
    _server_addr.sin_family = AF_INET;
    _server_addr.sin_port = htons(this->_hostshort);
    _server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(_server_addr.sin_zero, '\0', sizeof _server_addr.sin_zero);
    bind(_server_sock, (struct sockaddr *) &_server_addr,
         sizeof(_server_addr));

    if (listen(_server_sock, 35) == 0)
        printf("Listening\n");
    else
        printf("Error\n");

    for (int i = 0; i < 35; i++) {
        cid[i] = Connection(this->_server_sock);
        cid[i].ConnectionInit();
    }
}

void Connection::ConnectionInit() {
    struct sockaddr_storage server_storage;
    socklen_t addr_size;
    this->_connection_sock = accept(this->_server_sock, (struct sockaddr *) &server_storage, &addr_size);
}