
#include <ESP8266WiFi.h>

/************************************ Pin VALUES ******************************************/

int laserPin = D0;

void setup ()
{
   Serial.begin(115200);
   pinMode (laserPin, OUTPUT); // define the digital output interface 13 feet
}

void loop () {
   digitalWrite (laserPin, HIGH); // open the laser head
   delay (1000); // delay one second
   digitalWrite (laserPin, LOW); // turn off the laser head
   delay (1000); // delay one second
}
