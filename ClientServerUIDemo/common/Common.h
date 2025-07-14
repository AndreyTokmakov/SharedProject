/**============================================================================
Name        : Common.h
Created on  : 12.07.2025
Author      :
Version     : 1.0
Copyright   : Your copyright notice
Description : Request.h
============================================================================**/

#ifndef CPPWORKPROJECTS_COMMON_H
#define CPPWORKPROJECTS_COMMON_H

#include <cstdint>
#include <unistd.h>

namespace common
{
    using Socket = int32_t;
    using Port = uint16_t;

    constexpr Socket INVALID_SOCKET { -1 };
    constexpr Socket SOCKET_ERROR { -1 };

    // TODO: non-copy && non-move
    struct SocketGuard
    {
        Socket& socket;

        explicit SocketGuard(int& s): socket {s} {
        }

        ~SocketGuard()
        {
            ::close(socket);
            socket = INVALID_SOCKET;
        }
    };
}

#endif //CPPWORKPROJECTS_COMMON_H
