/*
  Simple example for receiving
  
  https://github.com/sui77/rc-switch/
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

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

void setup() {
  Serial.begin(115200);
  mySwitch.enableTransmit(D6);
  mySwitch.setProtocol(1);
  mySwitch.setPulseLength(190);
//  mySwitch.setRepeatTransmit(5);
  
}

void loop() {
  Serial.println("on");
  mySwitch.send(on1, 24);
  delay(1000);
  
  Serial.println("off");
  mySwitch.send(off1, 24);
  delay(1000);
}
