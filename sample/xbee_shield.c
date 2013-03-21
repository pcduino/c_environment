#include <core.h>
#include "Serial.h"
#define key1 8
#define key2 9
 
void port_init()
{
  pinMode(key1,INPUT);
  pinMode(key2,INPUT);
}  
 
void setup()
{
  //Initialize serial and wait for port to open:
  int rate = 9600;
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
   int value1 = digitalRead(key1);
   int value2 = digitalRead(key2);
   if(value1)
   {
     delay(20);
     value1 = digitalRead(key1);
     if(value1)
     {
       Serial.print('a');
     }
   }
   
   if(value2)
   {
      delay(20);
      value2 = digitalRead(key2);
      if(value2)
      {
        Serial.print('b');
      }
   }
   
   delay(100);
   /*
   //if(Serial.available())
  // {
      Serial.print("a");
      delay(500);
      Serial.print("b");
      delay(500);
   //}*/
  
}


