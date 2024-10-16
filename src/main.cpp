#include <Arduino.h>
#include <string.h>
#include <stdlib.h>
#include "dht_sensor.h"
#include "sd_card.h"
#include "comms.h"
#include "uuid4Gen.h"
#include "sockets.h"
#include "../Utils/Time/ntpClient.h"


#define DHTPIN 4 // Digital Pin connected to the DHT sensor
#define DHTTYPE 11 // Type of the DHT
#define LED 2

const char* ssid = "GALLANT";
const char* password = "Rutherf0rdg@ll@nt001";
const char* serverPath = "192.168.100.61:3000/user/post-data";

DHTSensor dht_sensor(DHTPIN, DHTTYPE);
SD_FS sd_access(5);
DHT_Data env_data;
sd_card_info card_data;
SocketConn socket;

COMMS comms(ssid, password);
UUID4 uuid4;
NTP* ntp;

void setup() {

  String data_got = "";
  long time = 0;
  // put your setup code here, to run once:
  Serial.begin(115200);

  comms.begin();

  comms.getRequest(serverPath, data_got);

  socket.begin();

  NTP::get_default_instance()->configureNTP();
  NTP::get_default_instance()->getEpochTime(time);

  uuid4.begin();

  Serial.printf("Data got: %s\n", data_got.c_str());

  sd_access.get_card_info(card_data);
  
  Serial.println("DHT Initialized");

  sd_access.print_card_info();

  // Create a .csv file to store all the data
  sd_access.readFile("/dht_data.csv");

  if (sd_access.readFile("/dht_data.csv") != 0) {
      sd_access.writeFile("/dht_data.csv", "Id,Temperature,Humidity");
  } else {
    Serial.println("File already exists");
  }

  pinMode(LED, OUTPUT);
}

void loop() {
  static int counter = 0;
  String data_str = "";
  sensor_data_t sensor_data;
  String uuid_str;
  long time_stamp = 0;

  uuid4.gen_uniq_UUID(uuid_str);
  Serial.printf("UUID is %s\n", uuid_str.c_str());
  // put your main code here, to run repeatedly:
  dht_sensor.readDHT(env_data);
  Serial.print("Temp: ");
  Serial.print(env_data.temp);
  Serial.println(" *C");
  Serial.print("Humidity: ");
  Serial.print(env_data.hum);
  Serial.println(" %");
  data_str = String(counter) + "," + String(env_data.temp) +  "," + String(env_data.hum);
  // sd_access.appendFile("/dht_data.csv", data_str.c_str());

  uint64_t _id;
  comms.getChipId(_id);

  strncpy(sensor_data.chip_id, String(_id).c_str(), sizeof(sensor_data.chip_id));

  sensor_data.humidity = env_data.hum;
  sensor_data.temp = env_data.temp;
  sensor_data.uuid = uuid_str;
  NTP::get_default_instance()->getEpochTime(time_stamp);
  sensor_data.timestamp = time_stamp;
  // comms.postRequest(serverPath, sensor_data);
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);
  JsonDocument doc;
  doc["message"] = "Mayday, mayday Connection! Over";
  doc["timestamp"] = time_stamp; // Sends seconds
  doc["timeformat"] = "epoch";
  doc["senderId"] = String(_id);
  Serial.println("Timestamp below\n");
  Serial.printf("%ld", time_stamp * 1000);
  JsonObject jsonObj = doc.as<JsonObject>();
  socket.messageSend(jsonObj);
}