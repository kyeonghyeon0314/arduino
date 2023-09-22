#include <SimpleKeypad.h>

const byte nb_rows = 4;
const byte nb_cols = 4;
char key_chars[nb_rows][nb_cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[nb_rows] = {9,8,7,6};
byte colPins[nb_cols] = {10,11,12,13};

SimpleKeypad kp1((char*)key_chars, rowPins, colPins, nb_rows, nb_cols);

void setup() {
  Serial.begin(9600);
}

void loop() {
  char key = kp1.getKey();
  if (key) {
    Serial.println(key);
  }
}
