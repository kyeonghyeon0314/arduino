int x_pin = A0;
int y_pin = A1;
int z_pin = 8;

void setup(){
  Serial.begin(9600);
  pinMode(z_pin,INPUT_PULLUP);
}

void loop(){
  Serial.print("  X: ");
  Serial.print(analogRead(x_pin));
  Serial.print(" ,Y: ");
  Serial.print(analogRead(y_pin));
  Serial.print(" , Z: ");
  Serial.println(digitalRead(z_pin));

  delay(100);
}