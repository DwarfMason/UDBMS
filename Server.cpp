//
// Created by igor on 24.10.19.
//

#include <cstring>
#include "Server.h"

std::mutex m;

void Server::ServerInit(int port) {
    /*FD_ZERO(&this->readset);*/
    this->_hostshort = port;
    this->_server_sock = socket(PF_INET, SOCK_STREAM, 0);
    _server_addr.sin_family = AF_INET;
    _server_addr.sin_port = htons(this->_hostshort);
    _server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(_server_addr.sin_zero, '\0', sizeof _server_addr.sin_zero);
    bind(_server_sock, (struct sockaddr *) &_server_addr,
         sizeof(_server_addr));

    if (listen(_server_sock, 5) == 0)
  /*      printf("Listening\n");
    else
        printf("Error\n");*/

    this->connection_socket.resize(MAX_CONNECTIONS);

    for (int i = 0; i < MAX_CONNECTIONS; i++)
        /*this->threads.emplace_back(std::thread(this->ClientTask, i));*/
        this->threads.emplace_back(std::thread(([i, this](){this->ClientTask(i);})));

    for (int i = 0; i < MAX_CONNECTIONS; i++)
        this->threads[i].join();

}

void  Server::ClientTask(int id) {
    this->AcceptConnection(id);

    while(true){
        std::stringstream buffer;

        ssize_t bytes_read;
        size_t size_to_get;

        bytes_read = recv(this->connection_socket[id], &size_to_get, sizeof(size_to_get), 0);
        if (bytes_read <= 0){
           // perror ("Client connection lost");
            close(this->connection_socket[id]);
            break;
        }

        char *buf = new char[size_to_get + 1];
        buf[size_to_get] = '\0';
        bytes_read = recv(this->connection_socket[id], buf, size_to_get, 0);

        if (bytes_read <= 0){
           // perror ("Client connection lost");
            close(this->connection_socket[id]);
            break;
        }

        {  //Блок закрытый lock_guard
            std::lock_guard<std::mutex> lock(m);

            std::stringstream buffer_cout;
            std::stringstream buffer_cerr;
            std::streambuf *old_cerr = std::cerr.rdbuf(buffer_cerr.rdbuf());
            std::streambuf *old_cout = std::cout.rdbuf(buffer_cout.rdbuf());

            buffer_cout.str("");
            buffer_cerr.str("");

            UDBMS::Driver driver;
            std::stringstream from_client;
            from_client << std::string(buf);

            driver.parse(from_client);

            std::cout.rdbuf(old_cerr);
            std::cerr.rdbuf(old_cout);

            char *send_data = strdup(buffer_cout.str().c_str());
            char *send_cerr = strdup(buffer_cerr.str().c_str());

            size_t size_send_data = buffer_cout.str().size();
            size_t size_send_cerr = buffer_cerr.str().size();

            if (size_send_data > size_send_cerr) {
                send(this->connection_socket[id], &size_send_data, sizeof(size_send_data), 0);
                send(this->connection_socket[id], "0", sizeof(char), 0);
                send(this->connection_socket[id], send_data, size_send_data, 0);
            } else {
                send(this->connection_socket[id], &size_send_cerr, sizeof(size_send_cerr), 0);
                send(this->connection_socket[id], "1", sizeof(char), 0);
                send(this->connection_socket[id], send_cerr, size_send_cerr, 0);
            }
        }

    }
    sleep(1);

 //   printf("Exit socket_thread \n");

    close(this->connection_socket[id]);
    this->threads[id].detach();
}

void Server::AcceptConnection(int id) {
    struct sockaddr_storage server_storage;
    socklen_t addr_size;
    this->connection_socket[id] = accept(this->_server_sock, (struct sockaddr *) &server_storage, &addr_size);
/*    FD_SET(this->connection_socket[id],&this->readset);*/
}
