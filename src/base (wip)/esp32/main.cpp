#include <esp_now.h>
#include <WiFi.h>

#define defClutch 550
#define defBreak 850
#define defGas 1100
#define maxClutch 1200
#define maxBreak 1500
#define maxGas 2000 

typedef struct dataS {
  int clutchValue;
  int breakValue;
  int gasValue;
} dataS;

dataS data;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&data, incomingData, sizeof(data));
  Serial.print("break: ");
  Serial.println(data.breakValue);
  Serial.print("gas: ");
  Serial.println(data.gasValue);
  Serial.print("clutch: ");
  Serial.println(data.clutchValue);
}
 
void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 16, 17); 
  
  WiFi.mode(WIFI_MODE_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  Serial2.printf("%d,%d,%d\n", data.gasValue, data.breakValue, data.clutchValue); 
}