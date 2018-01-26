#include <Servo.h>
#include <RCSwitch.h>
#include "secrets.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/************* MQTT TOPICS (change these topics as you wish)  **************************/
const char* topic1 = "home-assistant/door/1";
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
}

/************************************ Network VALUES ******************************************/
WiFiClient wifiClient;
PubSubClient client(server, 1884, callback, wifiClient);

/************************************ Servo VALUES ******************************************/
Servo myservo;
int servoPin = D4;
int lock = 0;
int unlock = 180;


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
//  myservo.attach(servoPin); 
//  pinMode(servoPin, OUTPUT);
  setup_wifi();
  
  mqttReConnect();
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
