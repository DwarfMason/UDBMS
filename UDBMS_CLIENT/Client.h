//
// Created by igor on 28.10.19.
//

#ifndef UDBMS_CLIENT_CLIENT_H
#define UDBMS_CLIENT_CLIENT_H
#include <sys/socket.h>
#include <netinet/in.h>
#include <zconf.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <gtest/gtest.h>

#define TESTS false

class Client {
public:
    explicit Client() = default;

    void ClientInit(int port);
    void ClientCommunication(int* sock);

private:
    int _port;
    bool RecCheck(int bytes_read, int* sock);
};


#endif //UDBMS_CLIENT_CLIENT_H
