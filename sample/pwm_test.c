/*
* PWM test program
*/
#include <core.h>
int pwm_id = 0;
int delay_us = 200000;
int value =  MAX_PWM_LEVEL/2;
void setup()
{
    if ( argc != 3 )
    {
        printf("Usage %s PIN_ID(3/9/10/11) Duty Level(0~32) or PIN_ID(5/6) Duty Level(0~255)\n ", argv[0]);
        exit(-1);   
    }
    
    pwm_id = atoi(argv[1]);
    value = atoi(argv[2]); 
    printf("PWM%d test with duty cycle %d\n", pwm_id, value);
    analogWrite(pwm_id, value);
}

void loop()
{
    delayMicroseconds(delay_us); 
}

