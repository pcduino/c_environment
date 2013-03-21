#include <stdio.h>
#include <string.h>
#include <core.h>
#include "SPI.h"

#define RCK 5
#define RCK_L digitalWrite(RCK,LOW);
#define RCK_H digitalWrite(RCK,HIGH);

const char table[]=
{0xc0,0xf9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};

void setup()
{
   pinMode(RCK,OUTPUT);
   SPI.begin();
   SPI.setClockDivider(SPI_CLOCK_DIV32);
   //SPI.setDataMode(SPI_MODE0);
   RCK_H;
   //SPI.setBitOrder(LSBFIRST);
   delay(500);
   //printf("spi initialize ok !\n");
} 

char table1[] = {0xf9,0x99,0x83,0xa1};

void loop()
{
  char i=0;
  //RCK_L;
  //SPI.transfer(0x81,SPI_LAST);
  //RCK_H;
  //delay(1)
  //delayMicroseconds(2);
  for(i=0; i<10; i++)
  {
     RCK_L;
     //delayMicroseconds(10);
     SPI.transfer(table[i],SPI_LAST);
     RCK_H;
     delay(300);
  }
}
