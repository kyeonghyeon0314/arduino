#include <Stepper.h>
#define STEPS 2048

Stepper stepper(STEPS,8,10,9,11);

void setup() {
  Serial.begin(9600);
  stepper.setSpeed(10);
}

void loop() {
  Serial.println("Clockwise one cycle");
  stepper.step(STEPS);
  delay(1000);
  Serial.println("counterClockwis one cycle");
  stepper.step(-STEPS);
  delay(1000);
  Serial.println("180 degrees Clockwise one cycle");
  stepper.step(STEPS/2);
  delay(1000);
  Serial.println("512 steps 90 degrees Clockwise one cycle");
  stepper.step(512);
  delay(1000);
}
