/*
* LED bar test program
*/
#include <core.h>

#define DATA_Pin 0  //DATA IN
#define CLK_Pin  1  //CLK IN

#define CmdMode 0x0000  //Work on 8-bit mode
#define ON 0x00ff               //8-bit 1 data
#define SHUT 0x0000         //8-bit 0 data

static int s_clk_flag = 0;

void send16bitData(unsigned int data)
{
    for(unsigned char i=0;i<16;i++){
        if(data & 0x8000){
            digitalWrite(DATA_Pin, HIGH);
        }else{
            digitalWrite(DATA_Pin, LOW);
        }
 
        if(s_clk_flag){
            digitalWrite(CLK_Pin, LOW);
            s_clk_flag = 0;
        }else{
            digitalWrite(CLK_Pin, HIGH);
            s_clk_flag = 1;
        }

        data <<= 1;
    }
}

//latch routine for MY9221 data exchange
void latchData(void)
{
    int latch_flag = 0;
    
    digitalWrite(DATA_Pin, LOW);
    delayMicroseconds(200);
    for(unsigned char i=0;i<8;i++){
        if(latch_flag){
            digitalWrite(DATA_Pin, LOW);
            latch_flag = 0;
        }else{
            digitalWrite(DATA_Pin, HIGH);
            latch_flag = 1;
        } 
    }
    delayMicroseconds(200);
} 


void sendLED(unsigned int LEDstate)
{
  unsigned char i;
  for(i=0;i<12;i++)
  {
    if(LEDstate&0x0001)
      send16bitData(ON);
    else 
      send16bitData(SHUT);

        LEDstate=LEDstate>>1;
  }
}

void setup()
{
    printf("LED bar test code!\n");
    printf("Using I/O_0=DATA, I/O_1=CLK.\n");   
    
    pinMode(DATA_Pin,OUTPUT);   //Data pin
    pinMode(CLK_Pin,OUTPUT);  //CLK pin
    digitalWrite(DATA_Pin, LOW);
    digitalWrite(CLK_Pin, LOW);
    s_clk_flag = 0;
}

void loop()
{
        send16bitData(CmdMode); //set LED Bar mode
    sendLED(0x0155);        //send LED Bar data
    latchData();  //make it come into effect
    delay(2000);
    
        send16bitData(CmdMode); //set LED Bar mode
    sendLED(0x02aa);        //send LED Bar data
    latchData();  //make it come into effect
    delay(2000);
/*
    unsigned int i = 0x0000;
    unsigned int loop = 0x0;
  
    while(i<=0x03ff)
  {
    send16bitData(CmdMode); //set LED Bar mode
    sendLED(i);     //send LED Bar data
    latchData();  //make it come into effect
    i=i*2+1;
    delay(200);
  }
  */
}