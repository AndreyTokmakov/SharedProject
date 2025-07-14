/**============================================================================
Name        : main.cpp
Created on  : 11.07.2024
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : C++ Client-Server application demo
============================================================================**/

#include <iostream>
#include <string_view>
#include <vector>
#include <memory>

#include "client/TcpClient.h"
#include "server/TcpServer.h"
#include "common/RequestBuilder.h"


int main([[maybe_unused]] int argc,
         [[maybe_unused]] char** argv)
{
    const std::vector<std::string_view> params (argv + 1, argv + argc);
    constexpr common::Port port = 52525;

    /*
    const bool isServer = static_cast<bool>(atoi(params.front().data()));
    if (isServer) {
        std::cout << "Running server.....\n";
        std::unique_ptr<server::IServer> server { std::make_unique<server::TcpServer>() };

        [[maybe_unused]]
        const bool result = server->run("0.0.0.0", port);
    }
    else
    {
        std::unique_ptr<client::IClient> client { std::make_unique<client::TcpClient>() };
        if (client->connect("0.0.0.0", port))
        {
            common::Request request;
            request.set_payload(params[1].data());
            [[maybe_unused]]
            const ssize_t bytesSend = client->send(request);

            common::Response response;
            [[maybe_unused]]
            const ssize_t bytesReceived = client->receive(response);
            std::cout << response.payload() << std::endl;
        }
    }*/


    common::Request shutdownCmd = common::RequestBuilder::createShutdownCommand();
    common::Request event = common::RequestBuilder::createEventRequest("Event-Payload");
    common::Request message = common::RequestBuilder::createMessageRequest("Message-Payload");


    std::cout << shutdownCmd.DebugString() << std::endl;
    std::cout << event.DebugString() << std::endl;
    std::cout << message.DebugString() << std::endl;


    return EXIT_SUCCESS;
}
