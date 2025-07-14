/**============================================================================
Name        : RequestBuilder.cpp
Created on  : 12.07.2025
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : RequestBuilder.cpp
============================================================================**/

#include "RequestBuilder.h"

namespace common
{
    common::Request RequestBuilder::createShutdownCommand()
    {
        common::Request request;
        request.set_event_type(common::Request_Type::Request_Type_Command);
        request.set_cmd_type(common::Request_CommandType::Request_CommandType_Shutdown);
        return request;
    }

    common::Request RequestBuilder::createMessageRequest(const std::string& data)
    {
        common::Request request;
        request.set_event_type(common::Request_Type::Request_Type_Message);
        request.set_payload(data);
        return request;
    }

    common::Request RequestBuilder::createEventRequest(const std::string& data)
    {
        common::Request request;
        request.set_event_type(common::Request_Type::Request_Type_Event);
        request.set_payload(data);
        return request;
    }
}