#include "sockets.h"


int SocketConn::begin()
{
    bool conn = client.connect(websockets_server_host, websockets_server_port, "/");
    if (!conn)
    {
        Serial.println("Client Connection socket server failed");
        return -1;
    }
    Serial.println("Connected to the socket server successfully");
    return 0;
}

int SocketConn::messageSend(const JsonObject& jsonObj)
{   
    String str;
    serializeJson(jsonObj, str);
    bool send = client.send(str);

    if (!send)
    {
        Serial.println("Send data failed");
        return -1;
    }

    return 0;
}