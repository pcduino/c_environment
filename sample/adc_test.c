/*
* ADC test program
*/
#include <core.h>
int adc_id = 0;
int delay_us = 100000;

void setup()
{
    if ( argc != 2 )
    {
        printf("Usage %s ADC_ID(0/1/2/3/4/5/6/7/8/9/10/11)\n", argv[0]);
        printf("Default will get ADC0 value\n");  
    }
    
    if(argc==2) 
        adc_id = atoi(argv[1]);
}

void loop()
{
    int value = analogRead(adc_id); // get adc value

    printf("ADC%d level is %d\n",adc_id, value);

    delayMicroseconds(delay_us);  
}
