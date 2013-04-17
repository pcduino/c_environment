#include <core.h>

#define MAX_COUNT 	  30
#define INT_MODE	  FALLING

/* support two interrupts pin2/pin3, and default interrupt pins are pulled up*/
int led_pin0 = 18, led_pin1 = 19;
volatile int state0 = LOW;
volatile int state1 = LOW;
int bc0 = 0, bc1 = 0;

void blink0()
{
   state0 = !state0;
   bc0++;
   printf("blink0: %d, count=%d\r\n", state0, bc0);
}

void blink1()
{
   state1 = !state1;
   bc1++;
   printf("blink1: %d, count=%d\r\n", state1, bc1);
}

void setup()
{
   pinMode(led_pin0, OUTPUT);
   pinMode(led_pin1, OUTPUT);
   attachInterrupt(0, blink0, INT_MODE);
   attachInterrupt(1, blink1, INT_MODE);
}

void loop()
{
   digitalWrite(led_pin0, state0);
   digitalWrite(led_pin1, state1);
   if (bc0 >= MAX_COUNT) 
	  detachInterrupt(0);
   if (bc1 >= MAX_COUNT) 
	  detachInterrupt(1);

   delay(1000); 
}


