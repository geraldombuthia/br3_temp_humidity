#ifndef EVENT_PUBLISHER_H
#define EVENT_PUBLISHER_H
#include "./comms.h"
#include <Arduino_JSON.h>
#include <Arduino.h>

class EventPublisher {
    public:
    EventPublisher();
    ~EventPublisher();
    int begin();
    int wifiConnStatus();
    int post(sensor_data_t data_cont);
    private:
    int conn_timeout = 0;

};

#endif //EVENT_PUBLISHER_H
