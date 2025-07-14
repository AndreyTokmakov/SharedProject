/**============================================================================
Name        : IClient.h
Created on  : 29.02.2024
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : IClient
============================================================================**/

#include "Common.h"
#include "ProtobufMessages.h"

namespace client
{
    struct IClient
    {
        [[nodiscard]]
        virtual bool connect(const std::string& host, common::Port port) = 0;

        [[nodiscard]]
        virtual bool close() = 0;

        [[nodiscard]]
        virtual ssize_t send(const common::Request& request) = 0;

        [[nodiscard]]
        virtual ssize_t receive(common::Response& response) = 0;

        virtual ~IClient() = default;
    };
}