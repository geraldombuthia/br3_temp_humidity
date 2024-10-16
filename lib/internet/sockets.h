#ifndef SOCKETS_H
#define SOCKETS_H

#include <ArduinoWebsockets.h>
#include "ArduinoJson.h"

using namespace websockets;

class SocketConn
{
public:
    int begin();
    int messageReceive(WebsocketsMessage message);
    int messageSend(const JsonObject& jsonObj);
    int sendPing();
    int poll();

private:
    WebsocketsClient client;
    bool is_connected;
    const char *websockets_server_host = "192.168.100.61"; // Enter server adress
    const uint16_t websockets_server_port = 8081;            // Enter server port
};

#endif //SOCKETS_H