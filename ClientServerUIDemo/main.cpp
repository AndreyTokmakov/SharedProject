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
#include "ui/ClientWindow.h"


int main([[maybe_unused]] int argc,
         [[maybe_unused]] char** argv)
{
    const std::vector<std::string_view> params (argv + 1, argv + argc);

#if 0
    std::unique_ptr<server::IServer> server { std::make_unique<server::TcpServer>() };
    const bool result = server->run("0.0.0.0", 52525);
    return EXIT_SUCCESS;

#else

    return ui::run(argc, argv);
#endif
}
