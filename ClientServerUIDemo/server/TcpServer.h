/**============================================================================
Name        : TcpServer.h
Created on  : 11.07.2025
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : TcpServer.h
============================================================================**/

#ifndef CPPWORKPROJECTS_TCPSERVER_H
#define CPPWORKPROJECTS_TCPSERVER_H

#include "IServer.h"
#include "Common.h"

namespace server
{
    struct TcpServer: IServer
    {
        enum class State: uint8_t {
            Run,
            Stop
        };

        common::Socket socket { common::SOCKET_ERROR };
        State state { State::Run };

        [[nodiscard]]
        bool run(const std::string& host,  common::Port port) override;

        [[nodiscard]]
        ssize_t sendResponse(common::Socket clientSocket, const common::Response& response) override;

        [[nodiscard]]
        ssize_t handleSession(common::Socket clientSocket) override;

        ~TcpServer() override;
    };
}

#endif //CPPWORKPROJECTS_TCPSERVER_H
