int CDSpin = A0;
int Ledpin = 2;
int rate = 500;

void setup() {
  Serial.begin(9600);

  pinMode(CDSpin, INPUT);
  pinMode(Ledpin, OUTPUT);
}

void loop() {
  int val = analogRead(CDSpin);

  Serial.print(" val : ");
  Serial.println(val);

  if(val < rate){
    digitalWrite(Ledpin, HIGH);
  }
  else{
    digitalWrite(Ledpin, LOW);
  }
}
