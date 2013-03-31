/*
* Tilt sensor test program for a10
*/
#include <core.h>
int ledPin = 0;
int switchPin = 1;
int val = 0;
void setup()
{
	printf("Tilt sensor test code!\n");
	printf("Using I/O_0=Drive LED, I/O_1=Sensor output.\n");	
  pinMode(ledPin,OUTPUT);
  pinMode(switchPin,INPUT);
}
void loop()
{
  val = digitalRead(switchPin);
  if (HIGH == val)  digitalWrite(ledPin,HIGH);
  else  digitalWrite(ledPin,LOW);
}

