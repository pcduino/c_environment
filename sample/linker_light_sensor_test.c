/*
* Light sensor test program
*/
#include <core.h>
#include <math.h>

const int ledPin = 0;                       //Connect the LED Grove module to I/O_0
const int thresholdvalue = 10;  //The treshold for which the LED should turn on.
const int adcIn = 0;    

unsigned int filter[4];
unsigned int *ptsampleNew;
unsigned int sampleCount;
void setup() {
    printf("Light sensor test code!\n");
    printf("Using I/O_0=Drive LED, ADC_0=Sensor output.\n");    
  pinMode(ledPin,OUTPUT);
  
  filter[0] = analogRead(adcIn);
  filter[1] = analogRead(adcIn);
  filter[2] = analogRead(adcIn);
  filter[3] = analogRead(adcIn);
  sampleCount = 0;

}
void loop() {
  int sensorValue = analogRead(adcIn);
  int Rsensor;
  
  if(sampleCount >= 3){
    sampleCount = 0;
  }else{
    sampleCount++;
  }
  printf("adc %d :%d!\n", sampleCount, sensorValue);
  ptsampleNew = &filter[sampleCount];
  *ptsampleNew = sensorValue;
  
  sensorValue = (filter[0] + filter[1] + filter[2] + filter[3]) / 4;
  
  Rsensor = (sensorValue*100)/64;
 
  if(Rsensor < thresholdvalue){
    digitalWrite(ledPin,HIGH);
  }else{
        digitalWrite(ledPin,LOW);
    }
    
    printf("adc:%d!\n", sensorValue);
    delay(200);
} 
