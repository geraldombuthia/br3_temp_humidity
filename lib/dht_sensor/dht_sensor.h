#ifndef DHT_SENSOR_H
#define DHT_SENSOR_H
#include <DHT.h>

/**
 * @brief Data structure to store temperature and humidity
 * 
 */
struct DHT_Data
{
    float temp = {};
    float hum = {};
};

/**
 * @brief DHT Sensor class
 * 
 */
class DHTSensor
{
public:
    /**
     * @brief Construct a new DHTSensor object
     * 
     */
    DHTSensor(int pin, int type);
    /**
     * @brief Destroy the DHTSensor object
     * 
     */
    ~DHTSensor();
    /**
     * @brief Initialize the DHT sensor
     * 
     */
    void initDHT();
    /**
     * @brief Read the temperature and humidity from the DHT sensor
     * 
     * @return DHT_Data 
     */
    int readDHT(DHT_Data &data);
    /**
     * @brief Get the default instance of the DHT sensor
     * @return DHTSensor
     */
    static DHTSensor* get_default_instance();

protected:
    float readTemp();
    float readHum();
private:
    int _dht_pin;
    int _dht_type;
    DHT _dht;

};
#endif