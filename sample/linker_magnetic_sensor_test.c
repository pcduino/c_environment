/*
* Magnetic sensor test program
*/
#include <core.h>

const int MagneticPin = 1;
const int ledPin   = 0;

void setup(){
    printf("Magnetic sensor test code!\n");
    printf("Using I/O_0=Drive LED, I/O_1=Sensor output.\n");
    
    pinMode(MagneticPin, INPUT);
    pinMode(ledPin,OUTPUT);
} 
 
void loop(){
    int sensorValue = digitalRead(MagneticPin);
    
    digitalWrite(ledPin, sensorValue);
}