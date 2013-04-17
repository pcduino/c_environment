/*
* Joystick test program
*/
#include <core.h>

void setup(){
    printf("Joystick Test Code!\n");
    printf("Using ADC_2 and ADC_3.\n"); 
}

void loop() {
  int sensorValue  = analogRead(2);
  int sensorValue2 = analogRead(3);
  

  printf("The X and Y coordinate is:%d x %d\n", sensorValue, sensorValue2);

  delay(500);
}
