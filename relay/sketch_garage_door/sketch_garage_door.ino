#include "secrets.h"
#include "WiFi.h"
#include "MQTT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/************* MQTT TOPICS (change these topics as you wish)  **************************/
const char* topic1 = SECRET_MQTT_TOPIC;

bool ignoreMeOnce = true;

/********************************** START CALLBACK*****************************************/
void callback(char* topic, byte* payload, unsigned int length) {
  char* message = callbackHelper(topic, payload, length);
  // Every startup appears to call this incorrectly once
  if (ignoreMeOnce) {
    ignoreMeOnce = false;
  } else {
    processTopic(topic, message);
  }
}

PubSubClient client(server, 1884, callback, wifiClient);

/************************************ Pin VALUES ******************************************/
int relayPin = D4;

/********************************** START RECONNECT*****************************************/
void processTopic(char* topic, char* message) {
  bool onCommand = strcmp(message, on_cmd) == 0;
  bool offCommand = strcmp(message, off_cmd) == 0;
  bool isTopic1 = strcmp(topic, topic1) == 0;
  if (onCommand) {
    if (isTopic1) {
      simulatePress();
//      relayOn();
    }
  } else if (offCommand) {
    if (isTopic1) {
      simulatePress();
//      relayOff();
    }
  }
}

void relayOn() {
  Serial.print("On\n");
  digitalWrite(relayPin, LOW);
}

void relayOff() {
  Serial.print("Off\n");
  digitalWrite(relayPin, HIGH);
}

void simulatePress() {
  relayOn();
  delay(750);
  relayOff();
}

void setup() {
  Serial.begin(115200);
  setup_wifi();

  mqttReConnect(client, topic1);
  client.setCallback(callback);
  pinMode(relayPin, OUTPUT);
  relayOff();
}

void loop() {
  if (!client.connected()) {
    mqttReConnect(client, topic1);
  }
  if (WiFi.status() != WL_CONNECTED) {
    delay(1);
    Serial.print("WIFI Disconnected. Attempting reconnection.");
    setup_wifi();
    return;
  }

  client.loop();

//  simulatePress();
//  delay(3000);
}
