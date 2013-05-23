/*
* PWM test program
*/
#include <core.h>
int pwm_id = 5;
int delay_us = 200000;
int freq = 781; //Hz
int value =  MAX_PWM_LEVEL/2;
void setup()
{
    int step = 0;
#ifndef PCDUINO_IDE
    if ( argc != 4 )
    {
        printf("Usage %s PIN_ID(3/9/10/11) Frequency[125-2000]Hz Duty Level or PIN_ID(5/6) Frequency[195,260,390,520,781]Hz Duty Level\n ", argv[0]);
        exit(-1);   
    }
    
    pwm_id = atoi(argv[1]);
    freq = atoi(argv[2]);
    value = atoi(argv[3]); 
#endif

    step = pwmfreq_set(pwm_id, freq);
    printf("PWM%d set freq %d and valid duty cycle range [0, %d]\n", pwm_id, freq, step);
    if (step > 0){
       printf("PWM%d test with duty cycle %d\n", pwm_id, value);
       analogWrite(pwm_id, value);
    }
}

void loop()
{
    delayMicroseconds(delay_us); 
}

