#include <Adafruit_NeoPixel.h>

int Neo_pin = 7;
int enc = A0;

Adafruit_NeoPixel Neo = Adafruit_NeoPixel(25, Neo_pin, NEO_GRB);

void setup() {
  Neo.begin();
  Neo.setBrightness(100);
  Neo.clear();

}
void loop() {
  
int val = analogRead(enc);
int x = map(val, 0, 1023, 0, 255);
for (int i = 0; i < 25; i++) {
  Neo.setPixelColor(i, Wheel(x));
  }
  Neo.show();
  delay(20);
}

uint32_t Wheel
(byte WheelPos){
  int R, G, B;
  WheelPos = 255- WheelPos;
  if(WheelPos < 85){
    R = 255-(WheelPos *3);
    G = 0;
    B = WheelPos * 3;
    }
    else if(WheelPos < 170){
      WheelPos = WheelPos- 85;
      R = 0;
      G = WheelPos * 3;
      B = 255-(WheelPos * 3);
      }
      else{
        WheelPos = WheelPos- 170;
        R = WheelPos * 3;
        G = 255-(WheelPos * 3);
        B = 0;
        }
        return Neo.Color(R, G, B);
        }


