/*
* Potentiometer test program
*/
#include <core.h>

int adcPin = 0;     // select the input pin for the potentiometer
int ledPin = 0;     // select the pin for the LED
int adcIn  = 0;     // variable to store the value coming from the sensor

void setup(){
    printf("Potentiometer Test Code!\n");
    printf("Using ADC_0 and I/O_0.\n"); 
  pinMode(ledPin, OUTPUT);    // set ledPin to OUTPUT   
}

void loop() {
  adcIn = analogRead(adcPin);   // read the value from the sensor.
  
  if(adcIn >= 30)  digitalWrite(ledPin,HIGH);   // if adc in >= 30, led light
  else digitalWrite(ledPin, LOW);
  
  printf("adc:%d!\n", adcIn);
  delay(500);
}
