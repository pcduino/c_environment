/*
* LED test program for a10
*/
#include <core.h>
int led_pin = 1;

void setup()
{
	if(argc != 2){
		goto _help;
	}
	
	led_pin = atoi(argv[1]);
	if((led_pin < 0) || (led_pin > 13)){
		goto _help;
	}
         
	pinMode(led_pin, OUTPUT);
  return;
    
_help:
	printf("Usage %s LED_PIN_NUM(0-13)\n", argv[0]);
	exit(-1);
}

void loop()
{
  digitalWrite(led_pin, HIGH);	// set the LED on
  delay(1000);									// wait for a second
  digitalWrite(led_pin, LOW);		// set the LED off
  delay(1000);									// wait for a second
}
