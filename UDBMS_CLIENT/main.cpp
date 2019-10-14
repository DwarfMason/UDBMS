#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>


int main() {
    std::string buff_out;
    std::string buff_in;
    int sock;
    struct sockaddr_in serv_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Unable to create socket. Restart the program or try later..\n");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(2077);
    serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Couldn`t connect to server. Try again later.\n");
        exit(2);
    }

    while (buff_out != "quit;") {
        std::cin >> buff_out;
        send(sock, &buff_out, sizeof(buff_out), 0);
        recv(sock, &buff_in, sizeof(buff_in), 0);
        std::cout << buff_in;
    }
    return 0;
}