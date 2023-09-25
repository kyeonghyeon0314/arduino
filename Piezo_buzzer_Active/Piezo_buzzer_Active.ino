int Buzzer_Pin = 5;

void setup() {
  pinMode(Buzzer_Pin, OUTPUT);

}

void loop() {
  digitalWrite(Buzzer_Pin, HIGH);
  delay(1000);
  digitalWrite(Buzzer_Pin, LOW);
  delay(3000);
}
