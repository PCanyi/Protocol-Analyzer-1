// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <iostream>
#include "../include/analyzer/Api.hpp"


int main(void)
{
    const auto protos = analyzer::utility::CheckALPNSupportedProtocols("www.google.com");
    if (protos.size() > 0) {
        std::cout << "Find next protocols: " << std::endl;
        for (auto &&p : protos) {
            std::cout << p << std::endl;
        }
        std::cout << std::endl;
    }


    analyzer::net::SocketSSL sock;
    if (sock.IsError()) {
        std::cout << "Socket fail..." << std::endl;
        return EXIT_FAILURE;
    }

    if (!sock.SetHttpProtocols()) {
        std::cout << "Set all HTTP protocols failed..." << std::endl;
        return EXIT_FAILURE;
    }
    if (!sock.SetOnlySecureCiphers()) {
        std::cout << "Secure ciphers failed..." << std::endl;
        return EXIT_FAILURE;
    }

    sock.Connect("www.google.ru");
    if (sock.IsError()) {
        std::cout << "Connection fail..." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Selected:   ";
    switch (sock.GetSelectedProtocol())
    {
        case analyzer::protocol::http::HTTP_VERSION::HTTP1_1:
            std::cout << "HTTP/1.1 protocol." << std::endl;
            break;
        case analyzer::protocol::http::HTTP_VERSION::HTTP2_0:
            std::cout << "HTTP/2.0 protocol." << std::endl;
            break;
        case analyzer::protocol::http::HTTP_VERSION::UNKNOWN:
            std::cout << "ALPN protocol UNKNOWN." << std::endl;
            break;
    }

    sock.Close();
    return EXIT_SUCCESS;
}


