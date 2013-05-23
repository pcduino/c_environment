#include <signal.h>
#include "Arduino.h"
#include "wiring_private.h"

static const char *swirq_dev = "/dev/swirq";

#define SWIRQ_START     (0x201)
#define SWIRQ_STOP      (0x202)
#define SWIRQ_SETPID    (0x203)
#define SWIRQ_ENABLE    (0x204)
#define SWIRQ_DISABLE   (0x205)

typedef struct tagSwirq_Config {
    uint8_t channel;
    int mode;
    int pid;
} SWIrq_Config,*pSWIrq_Config;


void attachInterrupt(uint8_t interruptNum, void (*userFunc)(void), int modec) 
{
   int ret = -1, fd = -1;
   int hwmode = 0, pid = 0;
   SWIrq_Config irqconfig;

   if((interruptNum < EXTERNAL_NUM_INTERRUPTS) && userFunc) {

      switch (modec) {
      case LOW: 
         hwmode = 0x3;
         break;
      case FALLING: 
         hwmode = 0x1;
         break;
      case RISING: 
         hwmode = 0x0;
         break;     
      case CHANGE: 
         hwmode = 0x4;
         break;     

      default:
         fprintf(stderr, "%s ERROR: set interrupt %d to invalid mode\n", __FUNCTION__, interruptNum);
         return;
      }

      switch (interruptNum) {
      case 0:
         signal(SIGUSR1, (void (*) (int))userFunc);
         break;
      case 1:
         signal(SIGUSR2, (void (*) (int))userFunc);
         break;
      }

      pid = getpid();
      irqconfig.channel = interruptNum;
      irqconfig.mode = hwmode;
      irqconfig.pid = pid;

      fd = open(swirq_dev, O_RDONLY);
      if ( fd < 0 )
         pabort("open swirq device fail");

      ret = ioctl(fd, SWIRQ_STOP, &interruptNum);
      if (ret < 0)
         pabort("can't set SWIRQ_STOP");
      ret = ioctl(fd, SWIRQ_SETPID, &irqconfig);
      if (ret < 0)
         pabort("can't set SWIRQ_SETPID");
      ret = ioctl(fd, SWIRQ_START, &interruptNum);
      if (ret < 0)
         pabort("can't set SWIRQ_START");

      //printf("ch:%d, mode:%d, pid:%d \r\n", irqconfig.channel, irqconfig.mode, irqconfig.pid);
      if (fd) 
         close(fd);
   }

}

void detachInterrupt(uint8_t interruptNum) 
{
   int ret = -1, fd = -1;

   if(interruptNum < EXTERNAL_NUM_INTERRUPTS) {

      fd = open(swirq_dev, O_RDONLY);
      if ( fd < 0 )
         pabort("open swirq device fail");

      ret = ioctl(fd, SWIRQ_STOP, &interruptNum);
      if (ret < 0)
         pabort("can't set SWIRQ_STOP");

      if (fd) 
         close(fd);
   } 

}

void interrupts() 
{
   int ret = -1, fd = -1;
   uint8_t interruptNum = 0;

   fd = open(swirq_dev, O_RDONLY);
   if ( fd < 0 )
      pabort("open swirq device fail");

   ret = ioctl(fd, SWIRQ_ENABLE, &interruptNum);
   if (ret < 0)
      pabort("can't set interrupt0 SWIRQ_ENABLE");

   interruptNum = 1;
   ret = ioctl(fd, SWIRQ_ENABLE, &interruptNum);
   if (ret < 0)
      pabort("can't set interrupt1 SWIRQ_ENABLE ");

   if (fd) 
      close(fd);
}

void noInterrupts() 
{
   int ret = -1, fd = -1;
   uint8_t interruptNum = 0;

   fd = open(swirq_dev, O_RDONLY);
   if ( fd < 0 )
      pabort("open swirq device fail");

   ret = ioctl(fd, SWIRQ_DISABLE, &interruptNum);
   if (ret < 0)
      pabort("can't set interrupt0 SWIRQ_DISABLE");

   interruptNum = 1;
   ret = ioctl(fd, SWIRQ_DISABLE, &interruptNum);
   if (ret < 0)
      pabort("can't set interrupt1 SWIRQ_DISABLE");

   if (fd) 
      close(fd);
}

