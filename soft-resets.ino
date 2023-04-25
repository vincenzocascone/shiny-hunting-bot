// Include libraries
#include <Servo.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>

// Declare servo objects and variables
Servo servoA;
Servo servoStart;
int pos = 0;

// Initialize LCD display
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Declare constants and variables
const int LIGHT_SENSOR_PIN = 0;
int lightValue = 0;
int buttonAPin = 6;
int buttonBPin = 5;
int lightThreshold = 150;
int falseSignalCounter = 0;
int totalCounter = 0;
int dailyCounter = 0;
int failCounter = 0;
int failPercentage = 0;
bool isRunning = false;
bool isShiny = false;
bool lightControl = false;

// Define functions to press buttons
void pressButtonA(int angle) {
  for (int pos = 0; pos <= angle; pos++) {
    servoA.write(pos);
    delay(15);
  }
  for (pos = angle; pos >= 0; pos--) {
    servoA.write(pos);
    delay(15);
  }
}

void pressButtonStart(int angle) {
  for (int pos = 0; pos <= angle; pos++) {
    servoStart.write(pos);
    delay(15);
  }
  for (pos = angle; pos >= 0; pos--) {
    servoStart.write(pos);
    delay(15);
  }
}

// Arduino setup
void setup() {
  Serial.begin(9600);
  pinMode(buttonAPin, INPUT_PULLUP);
  pinMode(buttonBPin, INPUT_PULLUP);
  lcd.begin(16, 2);
  servoA.attach(2);
  servoStart.attach(4);
  EEPROM.get(0, totalCounter);
}

// Main loop
void loop() {
  servoA.write(0);
  servoStart.write(0);

  if (digitalRead(buttonAPin) == LOW || isRunning) {
    isRunning = true;
    displayStatus();

    triggerEncounter();

    checkShiny();

    updateCounters();

    if (isShiny) {
      displayShiny();
    } else {
      checkFail();
      calculateFailPercentage();
      displayFail();
      softReset();
    }

    saveAndUpdateThreshold();
  }
}

void displayStatus() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SRs=");
  lcd.setCursor(4, 0);
  lcd.print(totalCounter);
  lcd.setCursor(10, 1);
  lcd.print("F=");
  lcd.setCursor(12, 1);
  lcd.print(failPercentage);
  lcd.print("%");
}

void triggerEncounter() {
  for (int i = 0; i < 4; i++) {
    pressButtonA(23);
    delay(i < 3 ? 2500 : 5500);
  }
}

void checkShiny() {
  for (int i = 0; i < 1550; i++) {
    lightValue = analogRead(LIGHT_SENSOR_PIN);
    displayLightValue();

    if (lightValue > lightThreshold) {
      falseSignalCounter++;
      updateShinyStatus(i);
    } else {
      falseSignalCounter = 0;
    }
    delay(1);
  }
}

void displayLightValue() {
  lcd.setCursor(11, 0);
  lcd.print("L=   ");
  lcd.setCursor(13, 0);
  lcd.print(lightValue);
  lcd.setCursor(0, 1);
  lcd.print(lightControl);
}

void updateShinyStatus(int i) {
  if (falseSignalCounter > 2 && i > 950 && !lightControl) {
    isShiny = true;
  } else if (falseSignalCounter > 2 && i > 500) {
    lightControl = true;
  }
}

void updateCounters() {
  falseSignalCounter = 0;
  totalCounter++;
  dailyCounter++;
}

void displayShiny() {
  while (isShiny) {
  lcd.setCursor(0, 1);
  lcd.print("Shiny ;)");
  }
}

void checkFail() {
  if (!isShiny && !lightControl) {
    failCounter++;
  }
}

void calculateFailPercentage() {
  failPercentage = (failCounter * 100) / dailyCounter;
  lightControl = false;
}

void displayFail() {
  if (failPercentage >= 80) {
    lcd.setCursor(0, 1);
    lcd.print("Fix");
    lcd.print(failPercentage);
  }
}

void softReset() {
  pressButtonStart(33);
  delay(8300);
}

void saveAndUpdateThreshold() {
  if (digitalRead(buttonBPin) == LOW) {
    EEPROM.put(0, totalCounter);
    isRunning = false;
    displayDailyCounter();
    
    while (!isRunning) {
      adjustThreshold();
      displayThreshold();
      delay(500);
    }
  }
}

void displayDailyCounter() {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Today's SRs: ");
  lcd.setCursor(12, 1);
  lcd.print(dailyCounter);
}

void adjustThreshold() {
  if (digitalRead(buttonAPin) == LOW) {
    lightThreshold += 5;
  } 
  if (digitalRead(buttonBPin) == LOW) {
    lightThreshold -= 5;
  }
  if (digitalRead(buttonBPin) == LOW && digitalRead(buttonAPin) == LOW) {
    isRunning = true;
  }
}

void displayThreshold() {
  lcd.setCursor(0, 0);
  cd.print("Soglia=");
  lcd.setCursor(7, 0);
  lcd.print(lightThreshold);
}
