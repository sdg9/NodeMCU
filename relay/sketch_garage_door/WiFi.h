#include "Arduino.h"
#include "secrets.h"
#include <ESP8266WiFi.h>

/************ WIFI Information (CHANGE THESE FOR YOUR SETUP) ******************/
const char* ssid = SECRET_WIFI_SSID;
const char* password = SECRET_WIFI_PASSWORD;

/************************************ Network VALUES ******************************************/
WiFiClient wifiClient;

/********************************** START MAC TO STRING*****************************************/
String macToStr(const uint8_t* mac)
{
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}

/********************************** START MAC CLIENT NAME *****************************************/
// Generate client name based on MAC address and last 8 bits of microsecond counter
String getClientName() {
  String clientName;
  clientName += "esp8266-";
  uint8_t mac[6];
  WiFi.macAddress(mac);
  clientName += macToStr(mac);
  return clientName;
}


/********************************** START SETUP WIFI*****************************************/
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


