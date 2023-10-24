#include <SimpleKeypad.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const byte nb_rows = 4;
const byte nb_cols = 4;

char key_chars[nb_rows][nb_cols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[nb_rows] = {9, 8, 7, 6};
byte colPins[nb_cols] = {10, 11, 12, 13};

SimpleKeypad kp1((char*)key_chars, rowPins, colPins, nb_rows, nb_cols);

Servo servo;
int Servopin = 5;
int angle = 90;

LiquidCrystal_I2C lcd(0x27, 16, 2);

char password[5] = "0000";
char inputPassword[5];
char newPassword[5];
byte passwordIndex = 0;
bool changeMode = false;
int Piezopin = 4;
int notes[] = {
  262,  // 1
  294,  // 2
  330,  // 3
  349,  // 4
  392,  // 5
  440,  // 6
  494,  // 7
  523,  // 8
  587,  // 9
  622,  // 0
  660,  // *
  698,  // #
  784,  // A
  831,  // B
  880,  // C
  988   // D
};
bool muteMode = false;

enum ScreenState {
  ENTER_PASSWORD,
  OPTION_SCREEN,
  CHANGE_PASSWORD
};
ScreenState currentScreen = ENTER_PASSWORD;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Enter pw:");

  servo.attach(Servopin);
  servo.write(0);
}
void playUnlockMelody() {
  if (muteMode) return;
  int melody[] = {262, 294, 330, 349};
  for(int i = 0; i < 4; i++){
    tone(Piezopin, melody[i], 200);
    delay(225);
  }
}

void playLockMelody() {
  if (muteMode) return;
  int melody[] = {349, 330, 294, 262};
  for(int i = 0; i < 4; i++){
    tone(Piezopin, melody[i], 200);
    delay(225);
  }
}

void playErrorTone() {
  if (muteMode) return;
  for(int i = 0; i < 3; i++) {
    tone(Piezopin, 196, 100);  
    delay(125);  
  }
}

void playNoteForButton(char key) {
  if (muteMode) return;
  int noteIndex = -1;

  if (key >= '1' && key <= '9') noteIndex = key - '1';
  else if (key == '0') noteIndex = 9;
  else if (key == '*') noteIndex = 10;
  else if (key == '#') noteIndex = 11;
  else if (key == 'A') noteIndex = 12;
  else if (key == 'B') noteIndex = 13;
  else if (key == 'C') noteIndex = 14;
  else if (key == 'D') noteIndex = 15;

  if (noteIndex != -1) {
    tone(Piezopin, notes[noteIndex], 500);
  }
}

void loop() {
  char key = kp1.getKey();

  if (key) {
    if (key == 'C') {
      muteMode = !muteMode;
      lcd.clear();
      if (muteMode) {
        lcd.print("Mute Mode On");
      } else {
        lcd.print("Mute Mode Off");
      }
      delay(1000);
      lcd.clear();
      
      switch (currentScreen) {
        case ENTER_PASSWORD:
          lcd.print("Enter pw:");
          break;
        case OPTION_SCREEN:
          lcd.print("change: A");
          lcd.setCursor(0, 1);
          lcd.print("lock: D");
          break;
        case CHANGE_PASSWORD:
          lcd.print("Enter new PW:");
          lcd.setCursor(0, 1);
          break;
      }
      return;
    }

    playNoteForButton(key);
    lcd.setCursor(passwordIndex, 1);

    if (key == '#') {
      passwordIndex = 0;
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      return;
    }

    if (changeMode) {
      if (key == 'B') {
        newPassword[passwordIndex] = '\0';

        if (strlen(newPassword) == 4) {
            strncpy(password, newPassword, 5);
            lcd.clear();
            lcd.print("PW updated");
            delay(500);
            currentScreen = OPTION_SCREEN;
            lcd.clear();
            lcd.print("change: A");
            lcd.setCursor(0,1);
            lcd.print("lock: D");
            changeMode = false;
        } else {
            lcd.clear();
            lcd.print("PW ERROR");
            playErrorTone();
            delay(1000);
            currentScreen = CHANGE_PASSWORD;
            lcd.clear();
            lcd.print("Enter new PW:");
            lcd.setCursor(0, 1);
        }
        passwordIndex = 0;
      } else {
        if(passwordIndex < 4) {
          newPassword[passwordIndex] = key;
          lcd.print(key);
          passwordIndex++;
        } else {
          lcd.clear();
          lcd.print("PW TOO LONG");
          playErrorTone();
          delay(1000);
          currentScreen = CHANGE_PASSWORD;
          lcd.clear();
          lcd.print("Enter new PW:");
          lcd.setCursor(0, 1);
          passwordIndex = 0;
        }
      }
      return;
    }

    if (key == 'D') {
      currentScreen = ENTER_PASSWORD;
      servo.write(0);
      playLockMelody();
      lcd.clear();
      lcd.print("Enter pw:");
      lcd.setCursor(0, 1);
    } else if (key == 'A' && currentScreen == OPTION_SCREEN) {
      currentScreen = CHANGE_PASSWORD;
      changeMode = true;
      passwordIndex = 0;
      lcd.clear();
      lcd.print("Enter new PW:");
      lcd.setCursor(0, 1);
    } else if (key == '*') {
      inputPassword[passwordIndex] = '\0';
      if (strcmp(inputPassword, password) == 0) {
        currentScreen = OPTION_SCREEN;
        lcd.clear();
        lcd.print("unlocked");
        servo.write(angle);
        playUnlockMelody();
        delay(1000);
        lcd.clear();
        lcd.print("change: A");
        lcd.setCursor(0, 1);
        lcd.print("lock: D");
      } else {
        currentScreen = ENTER_PASSWORD;
        lcd.clear();
        lcd.print("PW ERROR");
        playErrorTone();
        delay(1000);
        lcd.clear();
        lcd.print("Enter pw:");
        lcd.setCursor(0, 1);
      }
      passwordIndex = 0;
    } else {
      if (passwordIndex < 4) {
        inputPassword[passwordIndex] = key;
        lcd.print(key);
        passwordIndex++;
      } else {
        lcd.clear();
        lcd.print("PW TOO LONG");
        playErrorTone();
        delay(1000);
        lcd.clear();
        lcd.print("Enter pw:");
        lcd.setCursor(0, 1);
        passwordIndex = 0;
      }
    }
  }
}

