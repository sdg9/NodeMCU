#include <Servo.h>
#include "secrets.h"
#include "WiFi.h"
#include "MQTT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/************* MQTT TOPICS (change these topics as you wish)  **************************/
const char* topic1 = "home-assistant/door/1";

/********************************** START CALLBACK*****************************************/
void callback(char* topic, byte* payload, unsigned int length) {
  char* message = callbackHelper(topic, payload, length);
  processTopic(topic, message);
}

PubSubClient client(server, 1884, callback, wifiClient);

/************************************ Servo VALUES ******************************************/
Servo myservo;
int servoPin = D4;
int lock = 0;
int unlock = 180;

/********************************** START RECONNECT*****************************************/
void processTopic(char* topic, char* message) {
  bool onCommand = strcmp(message, on_cmd) == 0;
  bool offCommand = strcmp(message, off_cmd) == 0;
  bool isTopic1 = strcmp(topic, topic1) == 0;
  if (onCommand) {
    if (isTopic1) {
      doLock();
    }
  } else if (offCommand) {
    if (isTopic1) {
      doUnlock();
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();

  mqttReConnect(client, topic1);
  client.setCallback(callback);
}

void doLock() {
  Serial.print("Lock\n");
  myservo.attach(servoPin);
  myservo.write(lock);
  delay(500);
  myservo.detach();
}

void doUnlock() {
  Serial.print("Unlock\n");
  myservo.attach(servoPin);
  myservo.write(unlock);
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
}
