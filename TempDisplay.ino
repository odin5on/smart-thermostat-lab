
#include <DallasTemperature.h>
#include "SevSeg.h"

#define ONE_WIRE_BUS A0

//#define BUTTON_PIN A1

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DeviceAddress insideThermometer = {0x28, 0xFD, 0xDB, 0x81, 0xE3, 0x38, 0x3C, 0x3B};
SevSeg sevseg;

void setup() {
  
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  bool updateWithDelays = true; // Default 'false' is Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected
  byte numDigits = 4;
  byte digitPins[] = {10, 12, 11, 13};
  byte segmentPins[] = {9, 8, 7, 6, 5, 4, 3, 2};
  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(60);
  
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");
  sensors.begin();
  sensors.setResolution(10);
  sensors.requestTemperatures();
}


void loop() {
  static unsigned long timer = millis();
  static int deciSeconds = 0;
  
  static float tempC = sensors.getTempC(insideThermometer);
  if (millis() - timer >= 100) {
    timer += 100;
    deciSeconds++; // 100 milliSeconds is equal to 1 deciSecond
    
    if (deciSeconds == 20) { // Update temperature reading every 2 seconds.
      sensors.requestTemperatures();
      tempC = sensors.getTempC(insideThermometer);
      deciSeconds=0;
      //sevseg.setNumber(tempC*10,1);
      
    }
    sevseg.setNumber(tempC*10,1);
  }
  
  sevseg.refreshDisplay();
  //delay(200);
  }

 
    