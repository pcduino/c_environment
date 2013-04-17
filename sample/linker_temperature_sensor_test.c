/*
* Temperature sensor test program
*/
#include <core.h>

//The analog pin the TMP36's Vout (sense) pin is connected to
//the resolution is 10 mV / degree centigrade with a
//500 mV offset to allow for negative temperatures
         
const int sensorADC = 0;

void setup(){
    printf("Temperature sensor test code!\n");
    printf("Using ADC_0=Sensor output.\n");
} 
 
void loop()
{
 //getting the voltage reading from the temperature sensor
 int reading = analogRead(sensorADC);  
 
 // converting that reading to voltage
 float voltage = reading * 2.0;
 voltage /= 64.0; 
 printf("adc:%d\n", reading);
 // print out the voltage
 printf("%.2f volts\n", voltage);
 
 // now print out the temperature
 float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((volatge - 500mV) times 100)

 printf("%.2f degrees C\n", temperatureC);
 
 // now convert to Fahrenheight
 float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
 printf("%.2f degrees F\n\n", temperatureF);
 
 delay(1000);                                     //waiting a second
}
