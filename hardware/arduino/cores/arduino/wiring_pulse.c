#include "Arduino.h"
#include "wiring_private.h"

//caution: if the pulse and timeout is too large, the CPU will continue 100% usage until the func quit.
unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout)
{
    unsigned long start;
    unsigned long value;

    pinMode(pin, INPUT);
    start = micros();
    while( digitalRead(pin) == state )
        if( micros() - start > timeout )
            return 0;
    
    while( digitalRead(pin) != state )
        if( micros() - start > timeout )
    return 0;

    value = micros();
    while( digitalRead(pin) == state )
        if( micros() - start > timeout )
            return 0;
    
    return micros() - value;
}
