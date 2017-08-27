int pirpin = D6;
int pirstate = LOW;
int val = 0;

#include "secrets.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Values defined in secrets.h
const char* ssid = SECRET_WIFI_SSID;
const char* password = SECRET_WIFI_PASSWORD;
const char* server = SECRET_MQTT_SERVER;
const char* mqttUser = SECRET_MQTT_USER;
const char* mqttPassword = SECRET_MQTT_PASSWORD;

char* topic = "home-assistant/motion";

const String MOTION_ON = "ON";
const String MOTION_OFF = "OFF";

WiFiClient wifiClient;

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}


PubSubClient client(server, 1884, callback, wifiClient);

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


int status = WL_IDLE_STATUS;     // the Wifi radio's status

void connect() {
  if (client.connect((char*) getClientName().c_str(), mqttUser, mqttPassword)) {
    Serial.println("Connected to MQTT broker");
    Serial.print("Topic is: ");
    Serial.println(topic);
    
    if (client.publish(topic, "hello from ESP8266")) {
      Serial.println("Publish ok");
    }
    else {
      Serial.println("Publish failed");
    }
  }
  else {
    Serial.println("MQTT connect failed");
    Serial.println("Will reset and try again...");
    abort();
  }
}

void connectAndSend(String payload) {
  if (client.connect((char*) getClientName().c_str(), mqttUser, mqttPassword)) {
    Serial.println("Connected to MQTT broker");
    sendPayload(payload);
  }
  else {
    Serial.println("MQTT connect failed");
    Serial.println("Will reset and try again...");
    abort();
  }
}


// Generate client name based on MAC address and last 8 bits of microsecond counter
String getClientName() {
  String clientName;
  clientName += "esp8266-";
  uint8_t mac[6];
  WiFi.macAddress(mac);
  clientName += macToStr(mac);
  clientName += "-";
  clientName += String(micros() & 0xff, 16);
  return clientName;
}

void setup() {
  Serial.begin(115200);

  delay(10);
  pinMode(pirpin, INPUT);

  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  

//  Serial.print("Connecting to ");
//  Serial.print(server);
//  Serial.print(" as ");
//  Serial.println(clientName);
//  
  connect();
  
  for (int i = 0; i < 20; i++) {

    Serial.print(".");

    delay(1000);

  }
}

// function to talk to the Carriot Platform

void sendPayload(String payload) {
  Serial.print("Sending payload: ");
  Serial.println(payload);
  
  if (client.publish(topic, (char*) payload.c_str())) {
    Serial.println("Publish ok");
  }
  else {
    Serial.println("Publish failed");
  }
}

void motionDetected(String status) {
  String payload = "{\"status\":";
    payload += "\"";
    payload += status;
    payload += "\"}";
  if (client.connected()){    
    sendPayload(payload);
  } else {
    connectAndSend(payload);
//    delay(3000);
//    sendPayload(payload);
  }
}

void loop() {

  val = digitalRead(pirpin);

//  Serial.println(val);

  if (val == HIGH) {

    if (pirstate == LOW) {

      Serial.println("Motion Detected");

      pirstate = HIGH;

      Serial.println(F("Send Stream"));

      motionDetected(MOTION_ON);

      delay(30000);

    }
  }

  else {

    if (pirstate == HIGH) {

      Serial.println("motion finished");

      motionDetected(MOTION_OFF);

      pirstate = LOW;

    }

  }

}
