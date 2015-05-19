/*
    Copyright (c) 2015 Josh Gunnar

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the Software
    is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
    OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>
#include <Servo.h> 
#include "HardwareConfig.h"

#define WHITE 0x7 // For the LCD
 
Servo myservo; 
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
uint8_t buttons;

void setup() 
{ 
  // Setup IR pins
  pinMode(IR_OUTPUT_PIN, OUTPUT);
  pinMode(IR_INPUT_PIN, INPUT);
  
  // Setup servo acting as the starting gate
  myservo.attach(SERVO_PIN);
  myservo.write(0);
  delay(2000); // Can't call detach() before movement is finished
  myservo.detach(); // If left attached, the servo will 'twitch'
  
  // Setup the LCD
  lcd.begin(16, 2);
  lcd.setBacklight(WHITE);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("READY");
} 
 
void loop() { 
  // Basically, just wait for a race to start, signaled by the 'select' button
  buttons = lcd.readButtons();
  if (buttons) {
    if (buttons & BUTTON_SELECT) {
      race();
    }
  }
} 

void race() { 
  // Error flag
  boolean error = false;
  
  // Update display
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Race!");
  
  // Turn on the IR beam
  digitalWrite(IR_OUTPUT_PIN, HIGH);
  
  // Lower the gate
  myservo.attach(SERVO_PIN);
  myservo.write(GO_SERVO_POSITION);
  
  // Note the start time
  long start = millis();
  
  // Wait until the IR beam at the finish is broken
  while (analogRead(IR_INPUT_PIN) < IR_ANALOG_THRESHOLD) {
    // It's possible that the IR beam fails to register the finish..
    // so provide an out by checking for the left (back) button
    buttons = lcd.readButtons();
    if (buttons) {
      if (buttons & BUTTON_LEFT) {
        error = true;
        break;
      }
    }
  }
  
  if (!error) {
    // Figure out elapsed time in seconds
    long time = millis() - start;
    float timeSeconds = (float)time/1000;
    
    // Show the result
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Time: ");
    lcd.print(timeSeconds, 3);
    lcd.print("s");
  }
  else {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ready");
  }
  
  // Clean up, set the starting gate back up and turn off
  //  the IR beam
  myservo.write(STOP_SERVO_POSITION);
  delay(2000); // Can't detach until the movement is finished
  myservo.detach();
  digitalWrite(IR_OUTPUT_PIN, LOW);
}


