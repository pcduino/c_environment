/*
* Button test program
*/
#include <core.h>

int buttonPin = 1;
int ledPin    = 0;


void setup(){
    printf("Button Test Code!\n");
    printf("Using I/O_0=Drive LED and I/O_1=Button input.\n");  
  pinMode(ledPin, OUTPUT);    // set ledPin to OUTPUT   
  pinMode(buttonPin, INPUT);    // set ledPin to OUTPUT 
}

void loop() {
  int buttonIn = digitalRead(buttonPin);
  
    digitalWrite(ledPin, buttonIn);
  
  delay(50);
}
