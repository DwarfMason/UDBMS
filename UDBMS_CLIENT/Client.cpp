//
// Created by igor on 28.10.19.
//

#include "Client.h"

void Client::ClientInit(int port) {
    struct sockaddr_in addr;

    this->_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (_sock < 0) {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(this->_sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("connect");
        exit(2);
    }
    if (!TESTING) this->ClientCommunication(&this->_sock);
}

void Client::ClientCommunication(int *sock, const std::string& request) {
    while (true) {
        std::string to_server_msg;
        char c;
        to_server_msg = "";

        if (!TESTING)
        while (std::cin.get(c)) {
            to_server_msg += c;
            if (c == ';') break;
        }
        else to_server_msg = request;

        //std::getline(std::cin, to_server_msg);
        size_t size = to_server_msg.size();
        send(*sock, &size, sizeof(size_t), 0);
        char *send_data = strdup(to_server_msg.c_str());
        send(*sock, send_data, size, 0);
        free(send_data);

        int bytes_read = recv(*sock, &size, sizeof(size), 0);
        if (!this->RecCheck(bytes_read, sock)) break;
        char *recv_data = new char[size + 1];
        recv_data[size] = '\0';
        char flag;
        bytes_read = recv(*sock, &flag, sizeof(char), 0);
        if (!this->RecCheck(bytes_read, sock)) break;
        bytes_read = recv(*sock, recv_data, size, 0);
        if (!this->RecCheck(bytes_read, sock)) break;
        if (flag != '0')
            std::cerr << recv_data;
        else
            std::cout << recv_data;
        delete[] recv_data;
        if(TESTING) break;
    }
}

bool Client::RecCheck(int bytes_read, int *sock) {
    if (bytes_read <= 0) {
       // perror("Server connection lost");
        close(*sock);
        return false;
    }
    return true;
}

int &Client::GetSocket() {
    return this->_sock;
}
