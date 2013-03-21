#include <core.h>
#include "Serial.h"
#define key1 8
#define key2 9
#define led 10 
 
void port_init()
{
  pinMode(key1,INPUT);
  pinMode(key2,INPUT);
  pinMode(led,OUTPUT);
} 

void wait()
{
  digitalWrite(led,LOW);
  delay(200);
  digitalWrite(led,HIGH);
  delay(200);
} 
 
void setup()
{
  //Initialize serial and wait for port to open:
  int rate = 19200;
  Serial.begin(rate); 
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  //prints title with ending line break 
  //Serial.println("serial initialize succfull !"); 
  port_init();
  delay(500);
}
 
void loop()
{
   //Serial.println("ATD + +8613125044252;");//dial the number
   //delay(500);es
   //while(1);
   int value1 = digitalRead(key1);
   int value2 = digitalRead(key2);
   wait();
   if(value1)
   {
     delay(10);
     value1 = digitalRead(key1);
     if(value1)
     {
       Serial.println("ATD + +8613125044252;");//dial the number
       digitalWrite(led,HIGH);
       do{ 
          value2 = digitalRead(key2);
	  if(value2)
	  {
	    delay(10);
	    value2 = digitalRead(key2);
	  }
       }while(!value2);
       Serial.println("ATD + ATH");
       digitalWrite(led,LOW);
       delay(500);
       while(1);
     }
   }
}


