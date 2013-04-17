/*
* Relay sensor test program
*/
#include <core.h>
int RelayControlPin = 0;

void setup()
{
    printf("Relay test code!\n");
    printf("Using I/O_0.\n");   
    pinMode(RelayControlPin,OUTPUT);
}
void loop()
{
    digitalWrite(RelayControlPin, HIGH);
    delay(5000);
    digitalWrite(RelayControlPin, LOW);
    delay(5000);
}

