#include "DHT.h"
#define DHTTYPE DHT11

int DHTPIN = 3;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  float h =dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("% Temperature: ");
  Serial.print(t);
  Serial.print("℃, ");
  Serial.print(f);
  Serial.println("℉");
  
  delay(1000);

}
