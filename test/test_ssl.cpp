#include <iostream>
#include "../include/Socket.hpp"
#include "../include/SocketManager.hpp"


int main (int argc, char** argv)
{
    analyzer::net::SocketSSL sock;
    if (sock.IsError()) {
        std::cout << "Socket fail..." << std::endl;
        return EXIT_FAILURE;
    }

    if (!sock.SetHttpOnlyProtocol()) {
        std::cout << "Secure ciphers fail..." << std::endl;
        return EXIT_FAILURE;
    }
    if (!sock.SetOnlySecureCiphers()) {
        std::cout << "Secure ciphers fail..." << std::endl;
        return EXIT_FAILURE;
    }

    sock.Connect("habrahabr.ru");
    if (sock.IsError()) {
        std::cout << "Connection fail..." << std::endl;
        return EXIT_FAILURE;
    }

    char buff[] = "GET / HTTP/1.1\r\nHost: habrahabr.ru\r\nConnection: keep-alive\r\nAccept: */*\r\nDNT: 1\r\n\r\n";
    sock.Send(buff, strlen(buff));
    if (sock.IsError()) {
        std::cout << "Send fail..." << std::endl;
        return EXIT_FAILURE;
    }

    char buff_r[DEFAULT_BUFFER_SIZE];
    int32_t len = sock.RecvToEnd(buff_r, DEFAULT_BUFFER_SIZE);
    if (sock.IsError()) {
        std::cout << "Recv fail..." << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Receiving data length: " << len << std::endl;
    analyzer::log::DbgLog("HTTP data of www.google.ru.\n", buff_r);
    analyzer::log::DbgHexDump("HTTP data of www.google.ru.", buff_r, len, 24);

    sock.Close();
    return EXIT_SUCCESS;
}