#include "Arduino.h"
#include "secrets.h"
#include <PubSubClient.h>

/************* MQTT TOPICS (change these topics as you wish)  **************************/
const char* on_cmd = "ON";
const char* off_cmd = "OFF";

/************ MQTT Information (CHANGE THESE FOR YOUR SETUP) ******************/
const char* mqttUser = SECRET_MQTT_USER;
const char* mqttPassword = SECRET_MQTT_PASSWORD;
const char* server = SECRET_MQTT_SERVER;

char* callbackHelper(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  char message[length + 1];
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';
  Serial.println(message);
  return message;
}

void mqttReConnect(PubSubClient client, const char* topic) {
  // Loop until we're reconnected
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
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

