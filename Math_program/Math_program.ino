#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SimpleKeypad.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
bool backlightOn = true;
Servo servo;

const byte nb_rows = 4; 
const byte nb_cols = 4; 
char key_chars[nb_rows][nb_cols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

int Servopin = 5;
int angle = 180;
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


byte rowPins[nb_rows] = {9, 8, 7, 6}; 
byte colPins[nb_cols] = {10, 11, 12, 13}; 
SimpleKeypad kp1((char*)key_chars, rowPins, colPins, nb_rows, nb_cols);

const int TOTAL_QUESTIONS = 10;
unsigned long TIME_LIMIT = 5000;
unsigned long startTime;

int currentQuestion = 0;
int score = 0;
int correctAnswer;
String userAnswer = "";
String wrongQuestions = "";
bool isScrollingCompleted = false;
bool isGameActive = false;


struct Question{
  int x,y,answer;
  bool isAnswered;
};

Question questions[TOTAL_QUESTIONS];

enum GameState{
  SELECT_DIFFICULTY,
  PLAYING,
  SHOW_SCORE
};

GameState gameState = SELECT_DIFFICULTY;

void initializeQuestions() {
  for (int i = 0; i < TOTAL_QUESTIONS; i++) {
    questions[i].x = 0;
    questions[i].y = 0;
    questions[i].answer = 0;
    questions[i].isAnswered = false;
  }
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  pinMode(Piezopin,OUTPUT);
  servo.attach(Servopin);
  servo.write(0);
  randomSeed(analogRead(0));
  initializeQuestions();
  showDifficultyScreen();
}



void loop() {  
  switch(gameState) {
    case SELECT_DIFFICULTY:
      selectDifficulty();
      break;
    case PLAYING:
      handleUserInput();
      break;
    case SHOW_SCORE:
      handleScoreScreen();
      break;
  }
}

void playNoteForKey(char key) {
  int noteIndex = -1;
  for (int row = 0; row < nb_rows; row++) {
    for (int col = 0; col < nb_cols; col++) {
      if (key_chars[row][col] == key) {
        noteIndex = row * nb_cols + col;
        break;
      }
    }
    if (noteIndex != -1) break;
  }
  if (noteIndex != -1) {
    tone(Piezopin, notes[noteIndex], 100);
  }
}

void showDifficultyScreen() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Select Difficulty");
  lcd.setCursor(2, 1);
  lcd.print("A: Hard");
  lcd.setCursor(2, 2);
  lcd.print("B: Normal");
  lcd.setCursor(2, 3);
  lcd.print("C: Easy");
  lcd.setCursor(14,3);
  lcd.print("D: OFF");
}
void selectDifficulty() {
  char key = kp1.getKey();
  if(key){
    playNoteForKey(key);
  if (key == 'A' || key == 'B' || key == 'C') {
    switch (key) {
      case 'A':
        TIME_LIMIT = 2000;
        break;
      case 'B':
        TIME_LIMIT = 4000;
        break;
      case 'C':
        TIME_LIMIT = 6000;
        break;
    }
    gameState = PLAYING;
    displayNewProblem();
    }
    else if (key == 'D'){
      if (backlightOn) {
        lcd.noBacklight();
        lcd.clear();
        backlightOn = false;
      } else {
        lcd.backlight();
        showDifficultyScreen();
        backlightOn = true;
      }
    }
  }
}

void handleUserInput() {
  if(!isGameActive) return;

  char key = kp1.getKey();
  if (key) {
    playNoteForKey(key);
    
  if (key == '*') {
      checkAnswer(userAnswer);
      displayNewProblem();
    }
    else if (key == '#'){
      userAnswer = "";
    } 
    else if (isdigit(key)) {
      userAnswer += key;
    }
    displayUserInput(userAnswer);
  }
  if (millis() - startTime > TIME_LIMIT) {
    if(userAnswer != "") {
      checkAnswer(userAnswer); 
    } else {
      recordWrongAnswer(userAnswer); 
    }
    displayNewProblem();
    return;
  }
}

void handleScoreScreen() {
  char key = kp1.getKey();
  if (key) {
    playNoteForKey(key);
  if (key == 'D') {
    if (score == TOTAL_QUESTIONS) {
      dispenseCandy();
      return;      
    } 
    else if (!isScrollingCompleted) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Wrong Answers:");
      lcd.setCursor(0, 1);
      displayScrollingWrongAnswers();
      isScrollingCompleted = true;
    }
    else{
      resetGame();
      gameState = SELECT_DIFFICULTY;
      showDifficultyScreen();
      isScrollingCompleted = false;
    }
  }
}
}

void displayScrollingWrongAnswers() {
  int line = 1;
  int questionCount = 0;
  int lastQuestionEndIndex = 0;  
  int wrongAnswerCount = 0;
  
  for (int i = 0; i < wrongQuestions.length(); i++) {
    if (wrongQuestions[i] == ' ') {
      wrongAnswerCount++;
    }
  }

  String firstLine = "";
  String secondLine = "";
  String thirdLine = "";

  int charCount = 0;
  for (int i = 0; i < wrongQuestions.length(); i++) {
    if (charCount < 20) {
      firstLine += wrongQuestions[i];
    } else if (charCount >= 20 && charCount < 40) {
      secondLine += wrongQuestions[i];
    } else if(charCount >= 40 && charCount < 60){
      thirdLine += wrongQuestions[i];
    }else {
      lastQuestionEndIndex = i; 
      break;
    }
    charCount++;
    if (wrongQuestions[i] == ' ' || i == wrongQuestions.length() - 1) {
    }
  }
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Wrong Answers:");
  lcd.setCursor(0, 1);
  lcd.print(firstLine);
  lcd.setCursor(0, 2);
  lcd.print(secondLine);
  lcd.setCursor(0, 3);
  lcd.print(thirdLine);

  
  if (wrongAnswerCount < 7) {
    return;
  }

  delay(1500);
 
  String scrollText = wrongQuestions.substring(lastQuestionEndIndex-20);   
  int scrollPosition = 20;
  unsigned long lastScrollTime = millis();

  while (millis() - lastScrollTime < 10000) {  
    char key = kp1.getKey();
    if(key == 'D'){
      return;
    }
    if (millis() - lastScrollTime > 500) {  
      lcd.setCursor(0, 3);
      delay(300);  
      String toDisplay = scrollText.substring(scrollPosition, min(scrollPosition + 20, scrollText.length()));
      lcd.print(toDisplay + String("                    ").substring(0, 20 - toDisplay.length()));

      scrollPosition++;
      if (scrollPosition + 20 > scrollText.length()) {
        scrollPosition = 0;  
        delay(1500);
      }
      lastScrollTime = millis();
    }
  }
}

void displayFinalScore() {
  gameState = SHOW_SCORE;
  isGameActive = false;
  userAnswer ="";
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Score: ");
  lcd.print(score);
  
  if (score == TOTAL_QUESTIONS) {
    lcd.setCursor(2, 2);
    lcd.print("Congratulations!");
    lcd.setCursor(0,3);
    lcd.print("Press D to get candy");
  } else {
    lcd.setCursor(2, 2);
    lcd.print("Press D to check");
    lcd.setCursor(3,3);
    lcd.print("wrong answers");
  }
}

void dispenseCandy() {

  int melody[] = {262, 294, 330, 349, 392, 440, 494}; 
  int noteDurations[] = {4, 4, 4, 4, 4, 4, 4};  

  for (int thisNote = 0; thisNote < 7; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(Piezopin, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(Piezopin);
  }

  servo.write(angle); 
  delay(1000); 
  servo.write(0);
  delay(1000);
  resetGame(); 
  gameState = SELECT_DIFFICULTY;
  showDifficultyScreen();
}

void displayNewProblem() {
  if (currentQuestion >= TOTAL_QUESTIONS) {
    displayFinalScore();
    return;
  }

  startTime = millis();
  isGameActive = true;
  userAnswer ="";
  bool isUnique;

  do {
    isUnique = true;
    questions[currentQuestion].x = random(2, 10);
    questions[currentQuestion].y = random(2, 10);
    
   
    for (int i = 0; i < currentQuestion; i++) {
      if (questions[currentQuestion].x == questions[i].x && questions[currentQuestion].y == questions[i].y) {
        isUnique = false; 
        break;
      }
    }
  } while (!isUnique); 

  questions[currentQuestion].answer = questions[currentQuestion].x * questions[currentQuestion].y;
  correctAnswer = questions[currentQuestion].answer;
  currentQuestion++;
  lcd.clear();
  lcd.setCursor(2, 1);
  lcd.print("Q");
  lcd.print(currentQuestion);
  lcd.print(": ");
  lcd.print(questions[currentQuestion - 1].x);
  lcd.print(" x ");
  lcd.print(questions[currentQuestion - 1].y);
  lcd.print(" = ");
}


void resetGame() {
  currentQuestion = 0;
  score = 0;
  wrongQuestions = "";
  isGameActive = false;
}



void checkAnswer(String userAnswer) {
  if (userAnswer.toInt() == correctAnswer) {
    score++;
    lcd.setCursor(12,2);
    lcd.print("Correct!");
  } else {
    recordWrongAnswer(userAnswer);
  }
  delay(500);
}

void displayUserInput(String userAnswer) {
  lcd.setCursor(15, 1);
  lcd.print(userAnswer);
}

void recordWrongAnswer(String answer) {
  lcd.setCursor(12,2);
  lcd.print("Wrong!");
  wrongQuestions += "Q" + String(currentQuestion) + ":" +
                    String(questions[currentQuestion - 1].x) + "x" +
                    String(questions[currentQuestion - 1].y) + "=" +
                    String(questions[currentQuestion - 1].answer) + " ";
}




