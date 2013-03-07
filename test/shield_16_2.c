/************************************************************
* 16X2 LCD shield for pcDuino
*
*  Lei Wang 3/7/2013
*
*
*************************************************************/
#include <core.h>

#define RS 8
#define EN 9
#define backlight 10
#define RS_L digitalWrite(RS,LOW)
#define RS_H digitalWrite(RS,HIGH)
#define EN_L digitalWrite(EN,LOW)
#define EN_H digitalWrite(EN,HIGH)
int DB[] = {7,6,5,4};
/********************************************************************/
void write_command(int command)
{
	int i,temp;
	RS_L;
	EN_L;	
	temp=command & 0xf0;

	for (i=0; i < 4; i++)
	{
	  if(temp&0¡Á80)
              digitalWrite(DB[i],HIGH);
          else digitalWrite(DB[i],LOW);
              temp <<= 1;
        }
        EN_H;
        delayMicroseconds(1);
        EN_L;
        temp=(command & 0x0f)<<4;
        for (i=0; i < 4; i++)
        {
         if(temp&0¡Á80)
           digitalWrite(DB[i],HIGH);
         else digitalWrite(DB[i],LOW);
           temp <<= 1;
        }
        EN_H;
        delayMicroseconds(1);
        EN_L;
}

/********************************************************************/
void write_data(int dat)
{
    int i=0,temp;

    RS_H;
    EN_L;
    temp=dat & 0xf0;

    for (i=0; i < 4; i++)
    {
      if(temp&0¡Á80)
        digitalWrite(DB[i],HIGH);
      else
        digitalWrite(DB[i],LOW);
      temp <<= 1;
    }
    EN_H;
    delayMicroseconds(1);
    EN_L;
    temp=(dat & 0x0f)<<4;
    for (i=0; i < 4; i++)
    {
     if(temp&0¡Á80)
        digitalWrite(DB[i],HIGH);
     else digitalWrite(DB[i],LOW); 
     temp <<= 1;

   }
   EN_H;
   delayMicroseconds(1);
   EN_L;
}

/********************************************************************/
void LCD_write_char( int x,int y,int dat)
{
int address;
if (x ==0) address = 0¡Á80 + y;
else address = 0xC0 + y;
write_command(address);
write_data(dat);
delayMicroseconds(10);
}
/********************************************************************/
void lcd1602_init()
{
int i = 0;
pinMode(RS,OUTPUT);
pinMode(EN,OUTPUT);
pinMode(backlight,OUTPUT);
digitalWrite(backlight,HIGH);
for (i=0; i < 4; i++)
{
pinMode(DB[i],OUTPUT);
}
delay(100);
write_command(0¡Á28);
delay(50);
write_command(0¡Á06);
delay(50);
write_command(0x0c);
delay(50);
write_command(0¡Á80);
delay(50);
write_command(0¡Á01);
delay(100);
}
/********************************************************************/
void setup (void)
{
lcd1602_init();
}
/********************************************************************/
void loop (void)
{
write_command(0¡Á02);
delay(10);
LCD_write_char(0,2,¡¯W');
LCD_write_char(0,3,¡¯e');
LCD_write_char(0,4,¡¯l');
LCD_write_char(0,5,¡¯c');
LCD_write_char(0,6,¡¯o');
LCD_write_char(0,7,¡¯m');
LCD_write_char(0,8,¡¯e');

LCD_write_char(0,10,¡¯t');
LCD_write_char(0,11,¡¯o');

LCD_write_char(1,4,¡¯p');
LCD_write_char(1,5,¡¯c');
LCD_write_char(1,6,¡¯D');
LCD_write_char(1,7,¡¯u');
LCD_write_char(1,8,¡¯i');
LCD_write_char(1,9,¡¯n');
LCD_write_char(1,10,¡¯o');
while(1);
}



