#ifndef COMMS_H
#define COMMS_H
#include <WiFi.h>
#include <HTTPClient.h>



/**
 * @brief 
 */

typedef struct sensor_data {
    char chip_id[14];
    float temp;
    float humidity;
    String uuid;
    long timestamp;
} sensor_data_t;

class COMMS {
    public:
    COMMS(const char* ssid, const char* password);
    ~COMMS();
    int begin();
    int wifiConnStatus();
    int getChipId(uint64_t &chip_id);
    int getRequest(const char* serverName, String &data);
    int postRequest(const char* serverName, sensor_data_t data_cont);

    private:
    String isoToUtf8(String input);
    const char* _ssid;
    const char* _password;
    bool _is_connected = false;
    int conn_timeout = 0;
    uint64_t chipId;
};

#endif //COMMS_H