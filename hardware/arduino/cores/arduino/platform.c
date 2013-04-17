#include "Arduino.h"

unsigned long millis()
{
    unsigned long m;
    struct timeval tv; 
    gettimeofday (&tv , NULL); 
    return (tv.tv_sec*1000 + tv.tv_usec/1000);
}

unsigned long micros() {
    unsigned long m;
    struct timeval tv;
        gettimeofday (&tv , NULL);
    return (tv.tv_sec * 1000000 + tv.tv_usec);
}

//under construct
//can refine with timer interrupt, but need more implement 
void delay(unsigned long ms)
{
/*
    unsigned long start = millis();
    while (ms > 0) {
        if (millis() - start >= ms)
            break;
    }
*/
    usleep(ms*1000);
}

//under construct
//can refine with timer interrupt, but need more implement 
void delayMicroseconds(unsigned int us)
{
/*
    unsigned long start = micros();
    while (us > 0) {
        if ( micros() - start >= us)
            break;
    }
*/
    usleep(us);
}

void delaySched(unsigned long ms)
{
    usleep(ms*1000);
}

void delayMicrosecondsSched(unsigned int us)
{
    usleep(us);
}

void pabort(const char *s)
{
    perror(s);
    abort();
}


