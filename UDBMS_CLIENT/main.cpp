#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <zconf.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 2078

bool RecCheck(int bytes_read, int* sock){
    if (bytes_read <= 0) {
        perror("Server connection lost");
        close(*sock);
        return false;
    }
    return true;
}

int main()
{
    int sock;
    std::string to_server_msg;
    char c;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }
    while (true) {
        to_server_msg = "";
        while (std::cin.get(c)){
            to_server_msg += c;
            if (c == ';') break;
        }
        //std::getline(std::cin, to_server_msg);
        size_t size = to_server_msg.size();
        send(sock, &size, sizeof(size_t), 0);
        char *send_data = strdup(to_server_msg.c_str());
        send(sock, send_data, size, 0);

        int bytes_read = recv(sock, &size, sizeof(size), 0);
        if (!RecCheck(bytes_read, &sock)) break;
        char *recv_data = new char[size + 1];
        recv_data[size] = '\0';
        std::string q(recv_data);
        char flag;
        bytes_read = recv(sock, &flag, sizeof(char), 0);
        if (!RecCheck(bytes_read, &sock)) break;
        bytes_read = recv(sock, recv_data, size, 0);
        if (!RecCheck(bytes_read, &sock)) break;
        if (flag != '0')
            std::cerr << recv_data;
        else
            std::cout << recv_data;
    }
    close(sock);
    return 0;
}