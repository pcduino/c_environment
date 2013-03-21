#include "TFTv2.h"
#include <SPI.h>

void setup()
{
    TFT_BL_ON;      // turn on the background light
    Tft.TFTinit();  // init TFT library               
    Tft.drawString("welcome",30,50,4,RED);      
    Tft.drawString("to",90,110,4,YELLOW);  
    Tft.drawString("pcDuino!",30,170,4,CYAN);    
    Tft.drawString("16/03/2013",80,280,2,WHITE); 
}

void loop()
{
  
}
