#include <core.h>
#include <stdio.h>
#include "PN532.h"

#define SCK 13
#define MOSI 11
#define SS 10
#define MISO 12

PN532 nfc;//(SCK, MISO, MOSI, SS);

void setup(void) 
{
    printf("\n");
    printf("Hello!\n");   
    nfc.begin(13, 12, 11, 10);  
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (! versiondata) 
    {
        printf("Didn't find PN53x board \n");
        while (1); // halt
    }
    /*do
    {
      nfc.begin(13, 12, 11, 10);
      delay(10);
      versiondata = nfc.getFirmwareVersion();
      if(!versiondata)  printf("Didn't find PN53x board \n");
      delay(300);  
    }while(!versiondata);*/
    //uint32_t versiondata = nfc.getFirmwareVersion();
    printf("Found chip PN5"); 
    printf("%x\n",(versiondata>>24) & 0xFF);
    printf("Firmware ver. ");
    printf("%d",(versiondata>>16) & 0xFF);
    printf("."); 
    printf("%d\n",(versiondata>>8) & 0xFF);
    printf("Supports "); 
    printf("%x\n",(versiondata & 0xFF));
    nfc.SAMConfig();
}


void loop()
{
    /*for (int i=0; i<8; i++)
     {
        digitalWrite(13, LOW);
        if (0x02 & (1<<i)) {
            digitalWrite(11, HIGH);
        } else {
            digitalWrite(11, LOW);
        }
        digitalWrite(13, HIGH);
    }*/
    //delayMicroseconds(1);
    unsigned long id;
    id = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A);
    if (id != 0) 
    {
        printf("Read card #"); 
	printf("%ld",id);
    }
    
}


