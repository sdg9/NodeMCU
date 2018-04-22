#include "secrets.h"
#include "WiFi.h"
#include "MQTT.h"
#include <ESP8266WiFi.h>

/************************************ Pin VALUES ******************************************/
int pResistor = A0;

void setup() {
  Serial.begin(115200);
  setup_wifi();
  
  pinMode(pResistor, INPUT);
}

int value;          // Store value from photoresistor (0-1023)

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    delay(1);
    Serial.print("WIFI Disconnected. Attempting reconnection.");
    setup_wifi();
    return;
  }

  value = analogRead(pResistor);
  Serial.print(value);
  Serial.print("\n");
  delay(500);
}
