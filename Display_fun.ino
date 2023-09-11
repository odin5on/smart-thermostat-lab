#include <DHT.h>
#include "SevSeg.h"

#define DHTPIN A0
#define DHTTYPE DHT11
#define BUTTON_PIN A1

SevSeg sevseg;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  byte numDigits = 4;
  byte digitPins[] = {12, 9, 8, 6};
  byte segmentPins[] = {11, 7, 4, 2, 13, 10, 5, 3};

  for (int i = 0; i < 4; i++) {
    pinMode(digitPins[i], OUTPUT);
  }

  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }

  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins);
  sevseg.setBrightness(90);

  dht.begin();
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

byte lastButtonState = HIGH;
int displayOn = true;
unsigned long lastDisplayUpdateTime = 0;
const unsigned long displayUpdateInterval = 2000;

void loop() {
  byte buttonState = digitalRead(BUTTON_PIN);
  if (buttonState != lastButtonState) {
    if (buttonState == LOW) {
      displayOn = !displayOn;
      if (!displayOn) {
        // If the display is turned off, immediately clear it
        sevseg.setNumber(0);
      }
      lastDisplayUpdateTime = millis();
    }
    lastButtonState = buttonState;
  }

  if (displayOn) {
    // Update the display with temperature data
    float temperatureCelsius = dht.readTemperature();
    float temperatureFahrenheit = (temperatureCelsius * 1.8) + 32.0;
    int tempInt = (int)temperatureFahrenheit;
    int tempDecimal = (int)((temperatureFahrenheit - tempInt) * 10.0);

    if (millis() - lastDisplayUpdateTime >= displayUpdateInterval) {
      sevseg.setNumber(tempInt * 10 + tempDecimal, 1);
    }
  }

  sevseg.refreshDisplay();
}
