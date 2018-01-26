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
  mySwitch.enableReceive(D3);  // Receiver on interrupt 0 => that is pin #2

  mySwitch.enableTransmit(D6);
//  mySwitch.setProtocol(1);
}

void loop() {
//
//
  if (mySwitch.available()) {
    
    int value = mySwitch.getReceivedValue();
    
    if (value == 0) {
      Serial.print("Unknown encoding");
    } else {
      Serial.print("Received ");
      Serial.print( mySwitch.getReceivedValue() );
      Serial.print(" / ");
      Serial.print( mySwitch.getReceivedBitlength() );
      Serial.print("bit ");
//      Serial.print("Pulse: ");
//      Serial.print( mySwitch.getReceivedPulseLength() );
      Serial.print("Protocol: ");
      Serial.println( mySwitch.getReceivedProtocol() );
    }

    mySwitch.resetAvailable();
  } else {
    Serial.println("on");
//    mySwitch.send(267571, 24);
    mySwitch.send(on3, 24);
    delay(1000);
    
    Serial.println("on2");
//    mySwitch.switchOn("267571", "00010");
    
//    Serial.println("off");
    mySwitch.send(off3, 24);
    delay(1000);
  }
}
