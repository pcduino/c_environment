/*
* RTC test program
*/
#include <core.h>
#include  "Wire.h"

#define  DS1307_I2C_ADDRESS  0x68 // This is the I2C address

int command = 0; // This is the command char, in ascii form, sent from the serial port 
int zero = 0; 
//long previousMillis = 0; // will store last time Temp was updated byte second, minute, hour, dayOfWeek, dayOfMonth, month, year; byte test;
//Convert normal decimal numbers to binary coded decimal
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
byte test;

byte decToBcd(byte val) 
{
   return ( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val) 
{
  return ( (val/16*10) + (val%16) );
}

void setDateDs1307()
{
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(zero);
  Wire.write(decToBcd(second));     // 0 to bit 7 starts the clock
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));           // If you want 12 hour am/pm you need to set
                                  // bit 6 (also need to change readDateDs1307)
  Wire.write(decToBcd(dayOfWeek));
  Wire.write(decToBcd(dayOfMonth));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
  Wire.endTransmission();   
    
}

 //Gets the date and time from the ds1307 and prints result 
void getDateDs1307() 
{
    // Reset the register pointer
    Wire.beginTransmission(DS1307_I2C_ADDRESS);
    Wire.write(zero);
    Wire.endTransmission();
    Wire.requestFrom(DS1307_I2C_ADDRESS, 7);
    // A few of these need masks because certain bits are control bits
    second     = bcdToDec(Wire.read() & 0x7f);
    minute     = bcdToDec(Wire.read());
    hour       = bcdToDec(Wire.read() & 0x3f);  // Need to change this if 12 hour am/pm
    dayOfWeek  = bcdToDec(Wire.read());
    dayOfMonth = bcdToDec(Wire.read());
    month      = bcdToDec(Wire.read());
    year       = bcdToDec(Wire.read());

    printf("%d:%d:%d  %d/%d/%d.\n", hour, minute, second, month, dayOfMonth, year);
}

void setup() 
{
    printf("RTC test code!\n");
    Wire.begin();
    
    getDateDs1307();

    if(argc > 1)
        year = atoi(argv[1]);
         
    if(argc > 2)
        month = atoi(argv[2]);

    if(argc > 3)
        dayOfMonth = atoi(argv[3]);

    if(argc > 4)
        hour = atoi(argv[4]);

    if(argc > 5)
        minute = atoi(argv[5]);

    if(argc > 6)
        second = atoi(argv[6]);
    
    //force setting
    setDateDs1307();
    
    // enable clock , 'CH'
} 
void loop()
{
  delay(2000);
    getDateDs1307();
}
