/*
  Receives MQTT packages and fires RF commands
*/

#include <RCSwitch.h>
#include "secrets.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#include <ArduinoJson.h>


/************* MQTT TOPICS (change these topics as you wish)  **************************/
const char* topic1 = "home-assistant/rf/1";
const char* topic2 = "home-assistant/rf/2";
const char* topic3 = "home-assistant/rf/3";
const char* topic4 = "home-assistant/rf/4";
const char* topic5 = "home-assistant/rf/5";
const char* on_cmd = "ON";
const char* off_cmd = "OFF";

/************ WIFI and MQTT Information (CHANGE THESE FOR YOUR SETUP) ******************/
const char* ssid = SECRET_WIFI_SSID;
const char* password = SECRET_WIFI_PASSWORD;
const char* server = SECRET_MQTT_SERVER;
const char* mqttUser = SECRET_MQTT_USER;
const char* mqttPassword = SECRET_MQTT_PASSWORD;

/********************************** START CALLBACK*****************************************/
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  char message[length + 1];
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';
  Serial.println(message);

  processTopic(topic, message);
//  if (!processJson(message)) {
//    return;
//  }
}

/************************************ MISC VALUES ******************************************/
RCSwitch mySwitch = RCSwitch();
WiFiClient wifiClient;
PubSubClient client(server, 1884, callback, wifiClient);

//int status = WL_IDLE_STATUS;     // the Wifi radio's status

/************************************ RF VALUES ******************************************/
int on1 = 267571;
int off1 = 267580;

int on2 = 267715;
int off2 = 267724;

int on3 = 268035;
int off3 = 268044;

int on4 = 269571;
int off4 = 269580;

int on5 = 275715;
int off5 = 275724;

/****************************************FOR JSON***************************************/
//const int BUFFER_SIZE = JSON_OBJECT_SIZE(10);
//#define MQTT_MAX_PACKET_SIZE 512

/********************************** START PROCESS JSON*****************************************/

void processTopic(char* topic, char* message) {
  if (strcmp(message, on_cmd) == 0) {
    if (strcmp(topic, topic1) == 0) {
      mySwitch.send(on1, 24);
    } else if (strcmp(topic, topic2) == 0) {
      mySwitch.send(on2, 24);      
    } else if (strcmp(topic, topic3) == 0) {
      mySwitch.send(on3, 24);      
    } else if (strcmp(topic, topic4) == 0) {
      mySwitch.send(on4, 24);      
    } else if (strcmp(topic, topic5) == 0) {
      mySwitch.send(on5, 24);      
    }
  } else if (strcmp(message, off_cmd) == 0) {
    if (strcmp(topic, topic1) == 0) {
      mySwitch.send(off1, 24);
    } else if (strcmp(topic, topic2) == 0) {
      mySwitch.send(off2, 24);      
    } else if (strcmp(topic, topic3) == 0) {
      mySwitch.send(off3, 24);      
    } else if (strcmp(topic, topic4) == 0) {
      mySwitch.send(off4, 24);      
    } else if (strcmp(topic, topic5) == 0) {
      mySwitch.send(off5, 24);      
    }
  }
  
}

//bool processJson(char* message) {
//  StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
//
//  JsonObject& root = jsonBuffer.parseObject(message);
//
//  if (!root.success()) {
//    Serial.println("parseObject() failed");
//    return false;
//  }
//
//  if (root.containsKey("1")) {
//    if (strcmp(root["1"], on_cmd) == 0) {
//      mySwitch.send(on1, 24);
//    }
//    else if (strcmp(root["1"], off_cmd) == 0) {
//      mySwitch.send(off1, 24);
//    }
//  }
//  
//  if (root.containsKey("2")) {
//    if (strcmp(root["2"], on_cmd) == 0) {
//      mySwitch.send(on2, 24);
//    }
//    else if (strcmp(root["2"], off_cmd) == 0) {
//      mySwitch.send(off2, 24);
//    }
//  }
//  
//  if (root.containsKey("3")) {
//    if (strcmp(root["3"], on_cmd) == 0) {
//      mySwitch.send(on3, 24);
//    }
//    else if (strcmp(root["3"], off_cmd) == 0) {
//      mySwitch.send(off3, 24);
//    }
//  }
//  
//  if (root.containsKey("4")) {
//    if (strcmp(root["4"], on_cmd) == 0) {
//      mySwitch.send(on4, 24);
//    }
//    else if (strcmp(root["4"], off_cmd) == 0) {
//      mySwitch.send(off4, 24);
//    }
//  }
//  
//  if (root.containsKey("5")) {
//    if (strcmp(root["5"], on_cmd) == 0) {
//      mySwitch.send(on5, 24);
//    }
//    else if (strcmp(root["5"], off_cmd) == 0) {
//      mySwitch.send(off5, 24);
//    }
//  }
//
//  return true;
//}

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

/********************************** START RECONNECT*****************************************/
void mqttReConnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect((char*) getClientName().c_str(), mqttUser, mqttPassword)) {
      Serial.println("connected");
      client.subscribe(topic1);
      client.subscribe(topic2);
      client.subscribe(topic3);
      client.subscribe(topic4);
      client.subscribe(topic5);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
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

/********************************** START SETUP*****************************************/
void setup() {
  
  Serial.begin(115200);
  mySwitch.enableTransmit(D6);
  mySwitch.setProtocol(1);
  mySwitch.setPulseLength(190);
  mySwitch.setRepeatTransmit(5);

  setup_wifi();
  
  mqttReConnect();
  client.setCallback(callback);
}

/********************************** START MAIN LOOP*****************************************/
void loop() {
  if (!client.connected()) {
      mqttReConnect();
  }
  if (WiFi.status() != WL_CONNECTED) {
    delay(1);
    Serial.print("WIFI Disconnected. Attempting reconnection.");
    setup_wifi();
    return;
  }
  
  client.loop();
}
