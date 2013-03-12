/*
* PWM test program for a10
*/
#include <core.h>
int pwm_id = 0;
int delay_us = 200000;
int value =  MAX_PWM_LEVEL/2;
void setup()
{
    if ( argc != 3 )
    {
        printf("Usage %s PWM_ID(0-5) Duty Level(0~256)\n", argv[0]);
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

