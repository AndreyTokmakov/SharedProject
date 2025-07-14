/**============================================================================
Name        : TcpClient.h
Created on  : 11.07.2025
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : TcpClient.h
============================================================================**/

#ifndef CPPWORKPROJECTS_TCPCLIENT_H
#define CPPWORKPROJECTS_TCPCLIENT_H

#include "IClient.h"
#include "Common.h"

namespace client
{
    struct TcpClient: IClient
    {
        common::Socket socket { common::INVALID_SOCKET };

        [[nodiscard]]
        bool connect(const std::string& host, common::Port port) override;

        [[nodiscard]]
        bool close() override;

        [[nodiscard]]
        ssize_t send(const common::Request& request) override;

        [[nodiscard]]
        ssize_t receive(common::Response& response) override;

        ~TcpClient() override;
    };
}

#endif //CPPWORKPROJECTS_TCPCLIENT_H
