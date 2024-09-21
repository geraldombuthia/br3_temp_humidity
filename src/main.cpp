#include <Arduino.h>
#include "dht_sensor.h"

#define DHTPIN 5 // Digital Pin connected to the DHT sensor
#define DHTTYPE 11 // Type of the DHT
#define LED 2

DHTSensor dht_sensor(5, 11);

DHT_Data env_data;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("DHT temp start");
  pinMode(LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  dht_sensor.readDHT(env_data);
  Serial.print("Temp: ");
  Serial.print(env_data.temp);
  Serial.println(" *C");
  Serial.print("Humidity: ");
  Serial.print(env_data.hum);
  Serial.println(" %");
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);
}