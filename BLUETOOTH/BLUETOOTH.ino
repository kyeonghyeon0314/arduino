#include <SoftwareSerial.h>

int BT_TX = 2;
int BT_RX = 3;

SoftwareSerial BTSerial(BT_RX, BT_TX);

void setup(){
  Serial.begin(9600);
  BTSerial.begin(9600); 
}

void loop(){
  if (BTSerial.available()){ 
    byte data = BTSerial.read();
    Serial.write(data);
    }

if (Serial.available()) {
  byte data = Serial.read();
  BTSerial.write(data);
  }
}
