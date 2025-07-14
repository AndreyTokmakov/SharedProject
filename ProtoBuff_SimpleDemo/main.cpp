/**============================================================================
Name        : main.cpp
Created on  : 
Author      : 
Version     : 1.0
Copyright   : Your copyright notice
Description : Protobuf simple example
============================================================================**/

#include <iostream>
#include <string_view>
#include <vector>
#include <deque>

#include "request.pb.h"
#include "response.pb.h"

namespace demo
{
    struct Network
    {
        std::deque<std::string> packets;

        void write(std::string pkt)
        {
            packets.push_back(std::move(pkt));
        }

        std::string read()
        {
            std::string packet = std::move(packets.front());
            packets.pop_front();
            return packet;
        }
    };

    struct Client
    {
        Network& network;

        void sendRequest()
        {
            messages::Request request;
            request.set_data("Message_1");

            std::string data;
            request.SerializeToString(&data);

            network.write(data);
            std::cout << "[Client] send: Request { data: " << request.data() << " }\n";
        }

        void readResponse()
        {
            std::string data = network.read();
            messages::Response response;
            response.ParseFromString(data);

            std::cout << "[Client] received: Response { code: " << response.code() << ", payload: " << response.payload() << " }\n";
        }
    };

    struct Server
    {
        Network& network;

        void receiveAndSendResponse()
        {
            std::string data = network.read();
            messages::Request request;

            request.ParseFromString(data);

            std::cout << "[Server] received: Request { data: " << request.data() << " }\n";

            messages::Response response;
            response.set_code(123);
            response.set_payload("Echo - " + request.data());

            response.SerializeToString(&data);

            network.write(data);
            std::cout << "[Server] send: Response { code: " << response.code() << ", payload: " << response.payload() << " }\n";
        }
    };

    void run()
    {
        Network internet;
        Client client { internet };
        Server server { internet };

        client.sendRequest();
        server.receiveAndSendResponse();
        client.readResponse();
    }
}


int main([[maybe_unused]] int argc,
         [[maybe_unused]] char** argv)
{
    const std::vector<std::string_view> args(argv + 1, argv + argc);

    demo::run();

    return EXIT_SUCCESS;
}