#include <core.h>
const char relayPin[4] = {4,5,6,7};

void setup()
{
  int i;
  for(i = 0; i < 4; i++)
  {
    pinMode(relayPin[i],OUTPUT);
  }
}

//just an simple demo to close and open 4 relays
// every other 1s.
void loop()
{

  int i=0;
  for(i = 0; i < 4; i++)
  {
    digitalWrite(relayPin[i],HIGH);
  }
  delay(1000);
  for(i = 0; i < 4; i++)
  {
    digitalWrite(relayPin[i],LOW);
  } 
  delay(1000);  
}
