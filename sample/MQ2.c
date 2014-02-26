/* Sample code for MQ2 Smoke Sensor Shield for pcDuino
   05/09/2013
**********************************************/
#include <core.h>
 
const int analogInPin =0;
 
int sensorValue = 0;        // value read from the pot
int count1;
 
void setup() {
  pinMode(7, OUTPUT);  
}
 
void loop() {
 
  count1++;
  // read the analog in value:
  sensorValue = analogRead(analogInPin);            
 
  if(count1==3000)
  {
    count1=0;
 
   printf("sensor=%d\n", sensorValue);
  }
}