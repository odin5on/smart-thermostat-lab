
#include <DallasTemperature.h>
#include "SevSeg.h"

#define ONE_WIRE_BUS A0

#define BUTTON_PIN A1

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DeviceAddress insideThermometer;
SevSeg sevseg;

// int pinA = 12;
// int pinB = 10;
// int pinC = 5;
// int pinD = 3;
// int pinE = 2;
// int pinF = 11;
// int pinG = 6;
// int D1 = 13;
// int D2 = 8;
// int D3 = 9;
// int D4 = 7;
// int DP = 4;

void setup() {
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");

  sensors.begin();
  sensors.getAddress(insideThermometer, 0); // Use the correct sensor index (0) here

  byte numDigits = 4;
  byte digitPins[] = {13, 8, 9, 7};
  byte segmentPins[] = {12, 10, 5, 3, 2, 11, 6, 4};

  for (int i = 0; i < 4; i++) {
    pinMode(digitPins[i], OUTPUT);
  }

  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }

  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins);
  sevseg.setBrightness(90);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

byte lastButtonState = HIGH;
bool displayOn = false;
unsigned long lastDisplayUpdateTime = 0;
const unsigned long displayUpdateInterval = 200;

void printTemperature(DeviceAddress deviceAddress) {
  float tempC = sensors.getTempC(deviceAddress);
  if (displayOn) {
    if (millis() - lastDisplayUpdateTime >= displayUpdateInterval) {
      // Convert the floating-point temperature to an integer
      int tempInt = int(tempC);
      sevseg.setNumber(tempInt, 1);
    }
  }
  if (tempC == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: Could not read temperature data");
    return;
  }
  Serial.print("Temp C: ");
  Serial.print(tempC);
}

void loop() {
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");

  byte buttonState = digitalRead(BUTTON_PIN);
  if (buttonState != lastButtonState) {
    if (buttonState == LOW) {
      displayOn = true; // Turn on the display when the button is pressed
      lastDisplayUpdateTime = millis();
    } else {
      displayOn = false; // Turn off the display when the button is released
      sevseg.blank(); // Clear the display immediately
    }
    lastButtonState = buttonState;
  }

  printTemperature(insideThermometer);
    
  sevseg.refreshDisplay();
}