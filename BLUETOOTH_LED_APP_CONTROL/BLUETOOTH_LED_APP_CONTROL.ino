#include <SoftwareSerial.h>

int BT_TX = 2;
int BT_RX = 3;
int LED_1 = 12;
int LED_2 = 13;

SoftwareSerial BTSerial(BT_RX, BT_TX);

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
}
void loop() {
  if (BTSerial.available()) {
    byte data = BTSerial.read();
    if (data == 'a') { digitalWrite(LED_1, HIGH); }
    if (data == 'b') { digitalWrite(LED_1, LOW); }
    if (data == 'c') { digitalWrite(LED_2, HIGH); }
    if (data == 'd') { digitalWrite(LED_2, LOW); }
    Serial.write(data);
  }
  
  if (Serial.available()) {
    byte data = Serial.read();
    BTSerial.write(data);
  }
}
