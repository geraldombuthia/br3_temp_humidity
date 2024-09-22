#include "dht_sensor.h"
// class DHTSensor {
//     public:
//     DHTSensor();
//     ~DHTSensor();
//     float readTemp();
//     float readHum();
//     DHT_Data readDHT();

// };
DHTSensor::DHTSensor(int pin = 2, int type=11):_dht_pin(pin), _dht_type(type), _dht(_dht_pin, _dht_type){
   
    _dht.begin();
}
DHTSensor::~DHTSensor(){
    
}
float DHTSensor::readTemp(){
    return _dht.readTemperature();
}
float DHTSensor::readHum(){
    return _dht.readHumidity();
}
int DHTSensor::readDHT(DHT_Data &data){
    data.temp = readTemp();
    data.hum = readHum();

    if (isnan(data.temp) || isnan(data.hum)) {
        return -1;  // Error reading from the sensor
    }
    return 0;
}
DHTSensor* DHTSensor::get_default_instance(){
    static DHTSensor instance;
    return &instance;
}