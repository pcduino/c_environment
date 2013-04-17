/*
* Hall sensor test program
*/
#include <core.h>

const int SensorPin = 2;
const int ledPin   = 0;

void setup(){
    printf("Hall sensor test code!\n");
    printf("Using I/O_0=Drive LED, I/O_2=Sensor output.\n");
    
    pinMode(SensorPin, INPUT);
    pinMode(ledPin,OUTPUT);
} 
 
void loop(){
    int sensorValue = digitalRead(SensorPin);
    
    digitalWrite(ledPin, sensorValue);
}