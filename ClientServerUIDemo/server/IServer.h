/**============================================================================
Name        : IServer.h
Created on  : 29.02.2024
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : IServer
============================================================================**/

#include "Common.h"
#include "ProtobufMessages.h"

namespace server
{
    struct IServer
    {
        [[nodiscard]]
        virtual bool run(const std::string& host,  common::Port port) = 0;

        [[nodiscard]]
        virtual ssize_t sendResponse(common::Socket clientSocket, const common::Response& response) = 0;

        [[nodiscard]]
        virtual ssize_t handleSession(common::Socket clientSocket) = 0;

        virtual ~IServer() = default;
    };
}