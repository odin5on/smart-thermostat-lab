#include <DallasTemperature.h>
#include "SevSeg.h"

#define ONE_WIRE_BUS A0
#define BUTTON_PIN A1

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DeviceAddress insideThermometer = {0x28, 0xFD, 0xDB, 0x81, 0xE3, 0x38, 0x3C, 0x3B};
SevSeg sevseg;

unsigned long lastTemperatureUpdate = 0;
unsigned long temperatureUpdateInterval = 1000; // 1 second interval

void setup() {
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  bool updateWithDelays = true;     // Default 'false' is Recommended
  bool leadingZeros = false;        // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false;     // Use 'true' if your decimal point doesn't exist or isn't connected
  byte numDigits = 4;
  byte digitPins[] = {10, 12, 11, 13};
  byte segmentPins[] = {9, 8, 7, 6, 5, 4, 3, 2};
  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, resistorsOnSegments,
               updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(60);

  pinMode(BUTTON_PIN, INPUT_PULLUP); // Set the button pin as input with internal pull-up resistor

  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");
  sensors.begin();
  sensors.setResolution(10);
  sensors.requestTemperatures();
}

void loop() {
  unsigned long currentMillis = millis();
  bool buttonState = digitalRead(BUTTON_PIN);

  // Check if the button is pressed
  if (buttonState == LOW) {
    // Button is pressed, display the current temperature
    if (currentMillis - lastTemperatureUpdate >= temperatureUpdateInterval) {
      lastTemperatureUpdate = currentMillis;

      // Update temperature reading
      sensors.requestTemperatures();
      float tempC = sensors.getTempC(insideThermometer);
      sevseg.setNumber(tempC * 10, 1);
    }
  } else {
    // Button is not pressed, turn off the display
    sevseg.blank();
  }

  sevseg.refreshDisplay();
}
