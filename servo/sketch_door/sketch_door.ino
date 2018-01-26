#include <Servo.h>

Servo myservo;

int servoPin = D4;
int lock = 0;
int unlock = 180;

void setup() {
  Serial.begin(115200);
  myservo.attach(servoPin); 
//  pinMode(servoPin, OUTPUT);
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
  doLock();
  delay(3000);
  doUnlock();
  delay(3000);  
}
