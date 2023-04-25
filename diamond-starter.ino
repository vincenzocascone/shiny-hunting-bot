#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <Servo.h>

// Servos for pressing buttons
Servo servo1; // Presses A button pin2
Servo servo2; // Presses start button pin4
Servo servo3; // Presses arrow up pin3

// LiquidCrystal display
//Pins            RS  E  D4 D5  D6 D7
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Global constants and variables
const int LIGHT_SENSOR_PIN = 0; // Light Sensor on Analog Pin 0
const int BUTTON_A_PIN = 6;     // Right button for starting the bot
const int BUTTON_B_PIN = 5;     // Left button to save or access the menu

const int DETECTION_VALUE = 210;
const int WAIT_DETECTION_VALUE = 140;

int cycleCounter = 0;           // Cycle counter
int lightValue = 0;             // Variable to hold the analog light reading
int totalCounter = 0;           // Main counter of the total SRs
int dailyCounter = 0;           // Counter of the daily SRs
int failCounter = 0;            // Keeps track of failed servo presses
int failPercentage = 0;         // Fails in %
bool isRunning = false;         // Run condition
bool isShiny = false;           // Shiny condition
bool isControlled = false;      // Light change condition
bool isWaiting = true;          // Helps to avoid a failed SR because of servo malfunction

// Function prototypes
void pressButtonA(int angle);
void pressButtonSTART(int angle);
void pressButtonUP(int angle);
void updateLCD();
void softReset();
void adjustThreshold();

void setup() {
  // Serial setup
  Serial.begin(9600);

  // Button setup
  pinMode(BUTTON_A_PIN, INPUT_PULLUP);
  pinMode(BUTTON_B_PIN, INPUT_PULLUP);

  // LCD setup
  lcd.begin(16, 2);

  // Servo setup
  servo1.attach(2);
  servo2.attach(4);
  servo3.attach(3);

  // Assign counter the value stored in EEPROM address 0
  EEPROM.get(0, totalCounter);
}

void loop() {
  // Initial servo position
  servo1.write(0);
  servo2.write(0);
  servo3.write(35);

  // Main cycle
  if (digitalRead(BUTTON_A_PIN) == LOW || isRunning) {
    isRunning = true;

    updateLCD();

    // Press the buttons in sequence to get to the encounter
    pressButtonA(23);
    delay(1300);
    pressButtonA(23);
    delay(2900);
    pressButtonA(23);
    delay(2800);
    pressButtonUP(35);
    delay(2500);
    pressButtonA(23);
    delay(4800);

    // Wait for encounter and check for fail
    while (isWaiting) {
      cycleCounter++;
      pressButtonA(23);
      lightValue = analogRead(LIGHT_SENSOR_PIN);
      lcd.setCursor(11, 0);
      lcd.print("L=   ");
      lcd.setCursor(13, 0);
      lcd.print(lightValue);
      lcd.setCursor(0, 1);
      delay(1000);

      for (int i = 0; i < 100; i++) {
        if (lightValue < WAITDETECTION_VALUE) {
            cycleCounter++;
            if (cycleCounter > 2) {
                isWaiting = false;
            }
        }
        delay(1);
      }
        
      lcd.print(isWaiting);

      if (cycleCounter > 45) {
        failCounter++;
        break;
      }
    }

    cycleCounter = 0;
    isWaiting = true;
    lcd.clear();
    delay(3800);

    // Press the buttons in sequence to finish the encounter
    pressButtonA(23);
    delay(1500);
    pressButtonA(23);
    delay(1500);
    pressButtonA(23);
    delay(7500);

    // Check for shininess
    for (int i = 0; i < 1950; i++) {
      lightValue = analogRead(LIGHT_SENSOR_PIN);
      lcd.setCursor(11, 0);
      lcd.print("L=   ");
      lcd.setCursor(13, 0);
      lcd.print(lightValue);
      lcd.setCursor(0, 1);
      lcd.print(isControlled);

      if (lightValue < DETECTION_VALUE) {
        cycleCounter++;

        if (cycleCounter > 2 && i > 1500 && !isControlled) {
          isShiny = true;
        } else {
          if (cycleCounter > 2 && i > 500) {
            isControlled = true;
          }
        }
      } else {
        cycleCounter = 0;
      }
      delay(1);
    }

    // Update counters and display
    cycleCounter = 0;
    totalCounter++;
    dailyCounter++;
    updateLCD();

    // If shiny, display message and halt loop
    while (isShiny) {
      lcd.setCursor(0, 1);
      lcd.print("*Shiny*   ;)");
    }

    // Check for failed SRs
    if (!isShiny && !isControlled) {
      failCounter++;

      // If the fail percentage is 80% or greater, display message and halt loop
      if (failPercentage >= 80) {
        lcd.setCursor(0, 1);
        lcd.print("Fix");
        lcd.print(failPercentage);
      }
    }

    // Calculate fail percentage and update LCD
    failPercentage = (failCounter * 100) / dailyCounter;
    isControlled = false;
    updateLCD();

    // Perform soft reset
    softReset();

    // Check for button press to save and adjust light value
    if (digitalRead(BUTTON_B_PIN) == LOW) {
      EEPROM.put(0, totalCounter);
      isRunning = false;
      adjustThreshold();
    }
  }
}

void pressButtonA(int angle) {
    for (int pos = 0; pos <= angle; pos += 1) {
        servo1.write(pos);
        delay(15);
    }
    for (pos = angle; pos >= 0; pos -= 1) {
        servo1.write(pos);
        delay(15);
    }
}

void pressButtonSTART(int angle) {
    for (int pos = 0; pos <= angle; pos += 1) {
        servo2.write(pos);
        delay(15);
    }
    for (pos = angle; pos >= 0; pos -= 1) {
        servo2.write(pos);
        delay(15);
    }
}

void pressButtonUP(int angle) {
    for (int pos = angle; pos >= 0; pos -= 1) {
        servo3.write(pos);
        delay(15);
    }
    for (pos = 0; pos <= angle; pos += 1) {
        servo3.write(pos);
        delay(15);
    }
}

void softReset() {
    pressButtonSTART(33);
    delay(8700);
}

void updateLCD() {
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

void adjustThreshold() {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Today's SRs: ");
    lcd.setCursor(12, 1);
    lcd.print(dailyCounter);

    // Use left button to decrease threshold by 5, right button to increase by 5.
    // Press both buttons to exit the menu.
    for (int s = 0; s < 100000000; s++) {
        if (digitalRead(BUTTON_A_PIN) == LOW) {
            DETECTION_VALUE += 5;
        }
        if (digitalRead(BUTTON_B_PIN) == LOW) {
            DETECTION_VALUE -= 5;
        }
        if (digitalRead(BUTTON_B_PIN) == LOW && digitalRead(BUTTON_A_PIN) == LOW) {
            break;
        }
        lcd.setCursor(0, 0);
        lcd.print("Threshold=");
        lcd.setCursor(10, 0);
        lcd.print(DETECTION_VALUE);
        delay(500);
    }
}
