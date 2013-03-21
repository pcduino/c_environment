/*

 This sketch prints "Hello World!" to the LCD
 and shows the time.
 
  The circuit:
 * 5V to Arduino 5V pin
 * GND to Arduino GND pin
 * SCL to Digital 13
 * SDA to Digital 11
 * LAT to Digital 10
*/


// include the library code:
#include <core.h>
#include "Wire.h"
#include "LiquidCrystal.h"

// Connect via SPI. Data pin is #3, Clock is #2 and Latch is #4
LiquidCrystal lcd(11, 13, 10);
long running_time;

void setup() 
{
  // set up the LCD's number of rows and columns: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
  running_time = millis();
}


void loop() 
{
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print( (millis()-running_time)/1000);

  lcd.setBacklight(HIGH);
  delay(500);
  lcd.setBacklight(LOW);
  delay(500);
}
