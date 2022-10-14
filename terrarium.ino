#include <Wire.h>
#include "Adafruit_HTU21DF.h"
#include "RTClib.h"

Adafruit_HTU21DF htu = Adafruit_HTU21DF();
RTC_DS3231 rtc;
 
const int fans = 6;
const int light = 8;
const int fog = 13;
const int hiRH = 80;
const int loRH = 60;

bool lightsOut = true;
bool fogON = false;

void setup() {
  Serial.begin(9600);

  pinMode(fans, OUTPUT);
  pinMode(fog, OUTPUT);
  pinMode(light, OUTPUT);

  digitalWrite(fans, LOW);
  digitalWrite(fog, HIGH);
  digitalWrite(light, HIGH);
  
  if (!htu.begin()) {
    Serial.println("Couldn't find sensor!");
    while (1);
  }

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
}

void loop() {
  DateTime now = rtc.now();
  
  if (lightsOut == true) {
    if (now.hour() >= 7 && now.hour() < 22) {
      digitalWrite(light, LOW);
      lightsOut = false;
    }
  }

  if (lightsOut == false) {
    if (now.hour() < 7 || now.hour() >= 22) {
      digitalWrite(light, HIGH);
      lightsOut = true;
    }
  }

  float rel_hum = htu.readHumidity();

  if (rel_hum >= hiRH && fogON == true) {
    digitalWrite(fog, HIGH);
    fogON = false;
  }

  if (rel_hum <= loRH && fogON == false) {
    digitalWrite(fog, LOW);
    fogON = true;
  }
}