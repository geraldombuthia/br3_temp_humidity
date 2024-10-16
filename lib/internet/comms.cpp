#include "comms.h"
#include <Arduino_JSON.h>

#define CONNECTION_TIMEOUT_INTERVAL 3000
COMMS::COMMS(const char *ssid, const char *password) : _ssid(ssid), _password(password)
{
}
COMMS::~COMMS()
{
}

int COMMS::begin()
{
    chipId = ESP.getEfuseMac();

    Serial.printf("Chip ID: %llu\n", chipId);
    WiFi.begin(_ssid, _password);
    Serial.println("Connecting to WiFi network");
    if (conn_timeout < CONNECTION_TIMEOUT_INTERVAL)
    {
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
            conn_timeout += 500;
        }
        Serial.println(" ");
        Serial.println("Connected to WiFi network with IP Address");
        Serial.println(WiFi.localIP());
        _is_connected = true;
        return 0;
    } else {
        Serial.println("Connection timeout");
        return -1;
    }
}

int COMMS::wifiConnStatus()
{
    return WiFi.status() == WL_CONNECTED;
}
int COMMS::getRequest(const char *serverName, String &data)
{
    WiFiClient client;
    HTTPClient http;

    String url = String("http://") + serverName;
    http.begin(client, url);

    http.addHeader("X-Chip-ID", String(chipId).c_str());
    int httpResponseCode = http.GET();

    String payload = "--";

    if (httpResponseCode > 0)
    {
        Serial.printf("HTTP Response Code: %d\n",httpResponseCode);

        String contentType = http.header("Content-Type");
        Serial.printf("Content-Type: %s\n", contentType.c_str());

        payload = http.getString();

    }
    else
    {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
        http.end();
        return -1;
    }
    http.end();

    JSONVar myObject = JSON.parse(payload);

    if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed");
        return -1;
    }
    Serial.print("Received JSON: ");
    Serial.println(myObject);

    // data = (const char*) myObject[keys[0]];
    data = String((const char *)myObject["message"]);
    // Serial.println(myObject["message"]);
    return 0;
}

int COMMS::postRequest(const char *serverName, sensor_data_t data_cont)
{
    WiFiClient client;
    HTTPClient http;

    String url = String("http://") + serverName;

    String sensorReadings;

    http.begin(client, url.c_str());

    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.addHeader("X-Chip-ID", String(chipId).c_str());
    String sendRequestData = "chipid=" + String(chipId)+ "&temp=" + String(data_cont.temp) +"&hum=" + String(data_cont.humidity) + "&uuid=" + String(data_cont.uuid) + "&timestamp=" + String(data_cont.timestamp);
    int httpResponseCode = http.POST(sendRequestData);

    if (httpResponseCode > 0) {
        Serial.printf("HTTP Response code: %d\n", httpResponseCode);
    }
    http.end();

    Serial.printf("Data posted successfully\n");
    return 0;
}

String COMMS::isoToUtf8(String input) {
    String output = "";
    for (int i = 0; i < input.length(); i++) {
        uint8_t c = input[i];
        if (c < 128) {
            // ASCII characters remain the same
            output += (char)c;
        } else {
            // Convert ISO-8859-1 characters to UTF-8 (2 bytes)
            output += (char)(0xC0 | (c >> 6));  // First byte of UTF-8
            output += (char)(0x80 | (c & 0x3F)); // Second byte of UTF-8
        }
    }
    return output;
}

int COMMS::getChipId(uint64_t &chip_id)  {

    chip_id = chipId;

    return 0;
}