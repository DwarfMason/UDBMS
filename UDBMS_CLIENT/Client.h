//
// Created by igor on 28.10.19.
//

#ifndef UDBMS_CLIENT_CLIENT_H
#define UDBMS_CLIENT_CLIENT_H
#include <sys/socket.h>
#include <netinet/in.h>
#include <zconf.h>
#include <string>
#include <arpa/inet.h>
#include <iostream>
#include <memory.h>
#include "Generator.h"

#define TESTING true


class Client {
public:
    explicit Client() = default;

    void ClientInit(int port);
    void ClientCommunication(int* sock, const std::string& request="0");
    int& GetSocket();

private:
    int _sock;
    bool RecCheck(int bytes_read, int* sock);
};


#endif //UDBMS_CLIENT_CLIENT_H
