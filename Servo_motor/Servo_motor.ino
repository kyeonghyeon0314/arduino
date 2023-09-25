#include<Servo.h>
Servo servo;

int Servopin = 9;
int angle = 90;

void setup() {

  Serial.begin(9600);

  servo.attach(Servopin);
  servo.write(0);
}

void loop() {
  servo.write(angle);
  delay(2000);
  servo.write(0);
  delay(2000);
}
