/**============================================================================
Name        : RequestBuilder.h
Created on  : 12.07.2025
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : MessageBuilder.h
============================================================================**/

#ifndef CPPWORKPROJECTS_REQUESTBUILDER_H
#define CPPWORKPROJECTS_REQUESTBUILDER_H

#include "ProtobufMessages.h"

namespace common
{
    struct RequestBuilder
    {
        static common::Request createShutdownCommand();
        static common::Request createMessageRequest(const std::string& data);
        static common::Request createEventRequest(const std::string& data);
    };
}


#endif //CPPWORKPROJECTS_REQUESTBUILDER_H
