//--------------- All libraries ---------------------------

// Servos for pressing buttons
#include <Servo.h>
Servo myservo1; // Presses A button pin2
Servo myservo2; // Presses start button pin3
int pos = 0;

// Library for non volatile variables
#include <EEPROM.h>

// Library for the lcd display
#include <LiquidCrystal.h>
//Pins            BS  E  D4 D5  D6 D7
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

//--------------- All Variables ---------------------------

const int LIGHT = 0;    // Light Sensor on Analog Pin 0
int val = 0;            // Variable to hold the analog light reading
int buttonApin = 6;     // Right button for starting the bots
int buttonBpin = 5;     // Left button to save or access the menu
int lval = 150;         // Value for detection of a change in the light intensity
int y = 0;              // Variable used for false light signal detection
int counter = 0;        // main counter of the total SRs
int dailycounter = 0;   // counter of the daily SRs
int fails = 0;          // keeps track of failed servo presses
int fails2 = 0;         //keeps track of other fails
int FailPerc = 0;       // fails in %
bool beginning = false; // Run condition
bool shiny = false;     // Shiny condition
bool control = false;   // Light change condition

//--------------- Void functions  ---------------------------
void pressbuttonA(int angle)
{
    for (int pos = 0; pos <= angle; pos += 1)
    {
        myservo1.write(pos);
        delay(15);
    }
    for (pos = angle; pos >= 0; pos -= 1)
    {
        myservo1.write(pos);
        delay(15);
    }
}

void pressbuttonSTART(int angle)
{
    for (int pos = 0; pos <= angle; pos += 1)
    {
        myservo2.write(pos);
        delay(15);
    }
    for (pos = angle; pos >= 0; pos -= 1)
    {
        myservo2.write(pos);
        delay(15);
    }
}

//--------------- Arduino setup ---------------------------
void setup()
{
    // Serial setup
    Serial.begin(9600);

    // Button setup
    pinMode(buttonApin, INPUT_PULLUP);
    pinMode(buttonBpin, INPUT_PULLUP);

    // LCD setup
    lcd.begin(16, 2);

    // Servo setup
    myservo1.attach(2);
    myservo2.attach(4);

    //Assign counter the value stored in EEPROM address 0
    EEPROM.get(0, counter);
}

//------------------ Main Loop ---------------------------
void loop()
{
    // Initial servo position
    myservo1.write(0);
    myservo2.write(0);

    // Main cycle
    if (digitalRead(buttonApin) == LOW or beginning == true) //Press the right button to start the hunt
    {

        beginning = true;

        // Display counter and fails info on the lcd
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("SRs=");
        lcd.setCursor(4, 0);
        lcd.print(counter);
        lcd.setCursor(10, 1);
        lcd.print("F=");
        lcd.setCursor(12, 1);
        lcd.print(FailPerc);
        lcd.print("%");

        // Servo1 presses 4 times A button in order to get to the encounter
        pressbuttonA(23);
        delay(1000);
        pressbuttonA(23);
        delay(2500);
        pressbuttonA(23);
        delay(2500);
        pressbuttonA(23);
        delay(5500);

        // Cycle that checks for the shininess
        for (int i = 0; i < 1550; i++)
        {
            // Stream the light reading and the control value
            val = analogRead(LIGHT);
            lcd.setCursor(11, 0);
            lcd.print("L=   ");
            lcd.setCursor(13, 0);
            lcd.print(val);
            lcd.setCursor(0, 1);
            lcd.print(control);

            if (val > lval)
            {
                y++;
                if (y > 2 and i > 950 and control == false)
                {
                    shiny = true;
                }
                else
                {
                    if (y > 2 and i > 500)
                    {
                        control = true;
                    }
                }
            }
            else
            {
                y = 0;
            }
            delay(1);
        }

        // Variables update
        y = 0;          // Resets the fake signal control
        counter++;      // Updates counter
        dailycounter++; // Updates daily counter

        // Lcd counter update
        lcd.setCursor(0, 0);
        lcd.print("SRs=");
        lcd.setCursor(4, 0);
        lcd.print(counter);

        // If the shininess is true the cycle stops   
        while (shiny==true) {
            lcd.setCursor(0, 1);
            lcd.print("*Shiny*   ;)");
        }

        // Checks if the SR failed or not
        if (shiny != true and control == false)
        {
            fails++;
            // If the fail % is better than 80% the cycle stops
            if (FailPerc >= 80)
            {
                lcd.setCursor(0, 1);
                lcd.print("Fix");
                lcd.print(FailPerc);
            }
        }

        // Transform fails counter to % of fails related to daily SRs
        FailPerc = (fails * 100) / dailycounter;
        control = false;

        // Lcd fails update
        lcd.setCursor(10, 1);
        lcd.print("F=");
        lcd.setCursor(12, 1);
        lcd.print(FailPerc);
        lcd.print("%");

        // Soft Reset
        pressbuttonSTART(33);
        delay(8300);

        // Press the left button in order to save and change light value
        if (digitalRead(buttonBpin) == LOW)
        {
            EEPROM.put(0, counter);
            beginning = false;
            lcd.clear();
            lcd.setCursor(0, 1);
            lcd.print("Today's SRs: ");
            lcd.setCursor(12, 1);
            lcd.print(dailycounter);

            // Left button to decrease by 5 or right button to increase by 5 | Press both to exit the menu
            for (int s = 0; s < 100000000; s++)
            {
                if (digitalRead(buttonApin) == LOW)
                {
                    lval = lval + 5;
                }
                if (digitalRead(buttonBpin) == LOW)
                {
                    lval = lval - 5;
                }
                if (digitalRead(buttonBpin) == LOW and digitalRead(buttonApin) == LOW)
                {
                    break;
                }
                lcd.setCursor(0, 0);
                lcd.print("Soglia=");
                lcd.setCursor(7, 0);
                lcd.print(lval);
                delay(500);
            }
        }
    }
}
