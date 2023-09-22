int sound = A0;
int LED = 3;
int State = LOW;
int check_time = 500;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int signalMax = 0;

  int t = millis();
  while (1){
    int k= analogRead(sound);
    signalMax = max(signalMax, k);
    int time = millis();
    if ( time-t > check_time ){break;}
  }
  Serial.println(signalMax);

  if(signalMax > 600){
    State = !State;
  }
  digitalWrite(LED, State);
}
