/*
* Touch sensor test program
*/
#include <core.h>

const int TouchPin = 1;
const int ledPin   = 0;

void setup(){
    printf("Touch sensor test code!\n");
    printf("Using I/O_0=Drive LED, I/O_1=Sensor output.\n");
    
    pinMode(TouchPin, INPUT);
    pinMode(ledPin,OUTPUT);
} 
 
void loop(){
    int sensorValue = digitalRead(TouchPin);
    
    digitalWrite(ledPin, sensorValue);
}