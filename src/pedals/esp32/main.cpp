#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#define clutchPin 34
#define breakPin 33
#define gasPin 32

uint8_t broadcastAddress[] = {0x4C, 0xC3, 0x82, 0x0C, 0x26, 0xA8};
typedef struct dataS {
  int clutchValue;
  int breakValue;
  int gasValue;
} dataS;

dataS data;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

 

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  pinMode(clutchPin, INPUT);
  pinMode(breakPin, INPUT);
  pinMode(gasPin, INPUT);
  if (esp_now_init() != ESP_OK) {

    Serial.println("Error initializing ESP-NOW");

    return;

  }
  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);

  peerInfo.channel = 0;  

  peerInfo.encrypt = false;    

  if (esp_now_add_peer(&peerInfo) != ESP_OK){

    Serial.println("Failed to add peer");

    return;

  }
}

void loop() {
  data.gasValue = analogRead(gasPin);
  data.breakValue = analogRead(breakPin);
  data.clutchValue = analogRead(clutchPin);
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &data, sizeof(data));
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }

  Serial.print(data.gasValue); Serial.println(" gas "); 
  Serial.print(data.breakValue); Serial.println(" break ");
  Serial.print(data.clutchValue); Serial.println(" clutch ");
  
  delay(5);
}
