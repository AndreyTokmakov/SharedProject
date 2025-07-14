/**============================================================================
Name        : TcpServer.cpp
Created on  : 11.07.2025
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : TcpServer.cpp
============================================================================**/

#include "TcpServer.h"

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


namespace
{
    constexpr uint32_t RECV_BUFFER_SIZE { 1024 };

}

namespace server
{
    using namespace std::string_literals;

    bool TcpServer::run(const std::string &host, common::Port port)
    {
        socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (common::INVALID_SOCKET == socket) {
            std::cerr << "Failed to create socket. Error = " << errno << std::endl;
            return false;
        }

        std::cout << socket << " created\n";

        common::SocketGuard guard { socket };
        const sockaddr_in server {
            PF_INET,
            htons(port),
            {.s_addr = inet_addr(host.data())}, {}
        };

        if (common::SOCKET_ERROR == ::bind(socket, reinterpret_cast<const sockaddr*>(&server), sizeof(server))) {
            std::cerr << "Failed to bind socket. Error = " << errno << std::endl;
            return false;
        }

        constexpr uint16_t backlog { 10 };
        if (common::SOCKET_ERROR == ::listen(socket, backlog)) {
            std::cerr << "Failed to bind socket. Error = " << errno << std::endl;
            return false;
        }

        sockaddr_in clientAddr{};
        socklen_t addLen { sizeof(clientAddr) };
        common::Socket clientSocket { common::INVALID_SOCKET };
        while (State::Run == state)
        {
            // std::cout << "Waiting for next connection ....\n";
            clientSocket = ::accept(socket, reinterpret_cast<sockaddr*>(&clientAddr), &addLen);
            if (common::INVALID_SOCKET  == clientSocket) {
                std::cerr << "Failed to create client socket. Error = " << errno << std::endl;
            }

            common::SocketGuard clientGuard { clientSocket };
            // std::cout << "Client connected " << inet_ntoa(clientAddr.sin_addr) << ':' << htons(clientAddr.sin_port) << std::endl;
            handleSession(clientSocket);
        }

        return true;
    }

    ssize_t TcpServer::sendResponse(common::Socket clientSocket, const common::Response& response)
    {
        std::string message;
        response.SerializeToString(&message);

        ssize_t bytes = ::send(clientSocket,message.data(),message.size(),0);
        // std::cout << bytes << " bytes send\n";
        return bytes;
    }

    ssize_t TcpServer::handleSession(common::Socket clientSocket)
    {
        ssize_t bytes { RECV_BUFFER_SIZE };
        std::string requestStr;
        char buffer[RECV_BUFFER_SIZE];
        while (RECV_BUFFER_SIZE == bytes)
        {
            bytes = ::recv(clientSocket, buffer, RECV_BUFFER_SIZE, 0);
            requestStr.append(buffer, bytes);
        }

        common::Request request;
        [[maybe_unused]] // TODO: Check result
        const bool result = request.ParseFromString(requestStr);

        if ("QUIT"s == request.payload()) {
            state = State::Stop;
        }

        common::Response response;
        response.set_payload("OK");

        return sendResponse(clientSocket, response);
    }

    TcpServer::~TcpServer()
    {
        ::close(socket);
    }
}