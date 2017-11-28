/*
  Simple example for receiving
  
  https://github.com/sui77/rc-switch/
*/

#include <RCSwitch.h>
#include "secrets.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//#include <SPI.h>
//#include <Servo.h>

const char* ssid = SECRET_WIFI_SSID;
const char* password = SECRET_WIFI_PASSWORD;
const char* server = SECRET_MQTT_SERVER;
const char* mqttUser = SECRET_MQTT_USER;
const char* mqttPassword = SECRET_MQTT_PASSWORD;

RCSwitch mySwitch = RCSwitch();

WiFiClient wifiClient;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
//  byte* p = (byte*)malloc(length);
//  memcpy(p,payload,length);
//  free(p);
//    Serial.print(payload);
//    Serial.print("] angle:");
//    String data;
//    for (int i = 0; i < length; i++) {
//        data += (char)payload[i];
//    }
//    double angle = ((data.toInt() * 180) / 100);
//    constrain(angle, 0, 180);
//    servo.write((int) angle);
//    Serial.println((int) angle);
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

// Generate client name based on MAC address and last 8 bits of microsecond counter
String getClientName() {
  String clientName;
  clientName += "esp8266-";
  uint8_t mac[6];
  WiFi.macAddress(mac);
  clientName += macToStr(mac);
  return clientName;
}

String topicString = "home-assistant/rf";
char* topic = (char*) topicString.c_str();

int status = WL_IDLE_STATUS;     // the Wifi radio's status

int on1 = 267571;
int off1 = 267580;

int on2 = 267715;
int off2 = 267724;

int on3 = 268035;
int off3 = 268044;

int on4 = 269571;
int off4 = 269580;

int on5 = 275715;
int off6 = 275724;


void mqttReConnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect((char*) getClientName().c_str(), mqttUser, mqttPassword)) {
            Serial.println("connected");
            client.subscribe(topic);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void connect() {
  if (client.connect((char*) getClientName().c_str(), mqttUser, mqttPassword)) {
    Serial.println("Connected to MQTT broker");
    Serial.print("Topic is: ");
    Serial.println(topic);
    client.subscribe(topic);
    
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
//    sendPayload(payload);
  }
  else {
    Serial.println("MQTT connect failed");
    Serial.println("Will reset and try again...");
    abort();
  }
}

void setup() {
  Serial.begin(115200);
  mySwitch.enableTransmit(D6);
  mySwitch.setProtocol(1);
  mySwitch.setPulseLength(190);
//  mySwitch.setRepeatTransmit(5);

  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  connect();
  client.setCallback(callback);
}

void loop() {
//  Serial.println("on");
//  mySwitch.send(on1, 24);
//  delay(1000);
//  
//  Serial.println("off");
//  mySwitch.send(off1, 24);
//  delay(1000);
  if (!client.connected()) {
      mqttReConnect();
  }
  client.loop();
}
