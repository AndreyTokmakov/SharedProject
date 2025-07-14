/**============================================================================
Name        : TcpClient.cpp
Created on  : 11.07.2025
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : TcpClient.cpp
============================================================================**/

#include "TcpClient.h"

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


namespace
{
    constexpr uint32_t RECV_BUFFER_SIZE { 2048 };
}

namespace client
{
    bool TcpClient::connect(const std::string& host,  common::Port port)
    {
        socket = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (common::INVALID_SOCKET == socket) {
            std::cerr << "Failed to create socket. Error = " << errno << std::endl;
            return false;
        }

        const sockaddr_in server {PF_INET, htons(port), {.s_addr = inet_addr(host.data())}, {}};
        if (int error = ::connect(socket, (sockaddr*)&server, sizeof(server));
            common::SOCKET_ERROR == error)
        {
            std::cerr << "Connect function failed with error: " << errno << std::endl;
            ::close(socket);
            return false;
        }

        return true;
    }

    ssize_t TcpClient::send(const common::Request& request)
    {
        std::string message;
        request.SerializeToString(&message);

        const ssize_t bytes = ::send(socket, message.c_str(), message.length(), 0);
        // std::cout << bytes << " bytes send\n";
        return bytes;
    }

    ssize_t TcpClient::receive(common::Response& response)
    {
        ssize_t bytes { common::SOCKET_ERROR };
        std::string responseStr;
        char buffer[RECV_BUFFER_SIZE];
        while ((bytes = ::recv(socket, buffer, RECV_BUFFER_SIZE, 0)) > 0) {
            responseStr.append(buffer, bytes);
        }

        [[maybe_unused]] // TODO: Check result
        const bool result = response.ParseFromString(responseStr);
        return response.payload().size();
    }

    TcpClient::~TcpClient()
    {
        ::close(socket);
    }

    bool TcpClient::close()
    {
        return common::SOCKET_ERROR != ::close(socket);
    }
}