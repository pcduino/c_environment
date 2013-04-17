#include "Arduino.h"
#include "wiring_private.h"

static const char *pwm_dev = "/dev/pwmtimer";

typedef struct tagPWM_Config {
    int channel;
    int dutycycle;
} PWM_Config,*pPWM_Config;

#define PWMTMR_START    (0x101)
#define PWMTMR_STOP     (0x102)
#define PWMTMR_FUNC     (0x103)
#define PWMTMR_TONE     (0x104)
#define PWM_CONFIG      (0x105)
#define HWPWM_DUTY      (0x106)

//under construct
uint8_t analog_reference = 0;
void analogReference(uint8_t mode)
{
    analog_reference = mode;
}

int analogRead(uint8_t pin)
{
    char str[10];
    char buf[32];    
    int ret = -1;
    int fd = -1;
    char *p = NULL;
        
    if ( pin >= 0 && pin <= MAX_ADC_NUM )
    {
        memset(buf, 0, sizeof(buf));
        lseek(adc_fd[pin], 0, SEEK_SET);
        ret = read(adc_fd[pin], buf, sizeof(buf));
        
        if ( ret <= 0 )
        {
            fprintf(stderr, "read adc %d failed\n", pin);
            exit(-1);
        }

        memset((void *)str, 0, sizeof(str));
        sprintf(str, "adc%d", pin);
        p = strstr(buf, str) + strlen(str) + 1;
        sscanf(p, "%d", &ret);
    }
    else
    {
        fprintf(stderr, "%s ERROR: invalid pin, pin=%d\n", __FUNCTION__, pin);
        exit(-1);
    }      
    return ret;

}

void analogWrite(uint8_t pin, int value)
{
     int ret = -1;
     int fd = -1;
     int val = 0;
     PWM_Config pwmconfig;
         
     pwmconfig.channel = pin;
     pwmconfig.dutycycle = value;
     if ( (pin == 3 || pin == 5 || pin == 6 || pin == 9 || pin == 10 || pin == 11) && 
         (value >= 0 && value <= MAX_PWM_LEVEL) )
     {
         fd = open(pwm_dev, O_RDONLY);
         if ( fd < 0 )
             pabort("open pwm device fail");
                
         switch (pin) 
         {
         case 5:
         case 6:
            ret = ioctl(fd, HWPWM_DUTY, &pwmconfig);
            if (ret < 0)
               pabort("can't set HWPWM_DUTY");
            break;
         case 3:
         case 9:    
         case 10:   
         case 11:   
            if (value <= 32) {
               //ret = ioctl(fd, PWMTMR_STOP, &val);
               //if (ret < 0)
               //   pabort("can't set PWMTMR_STOP");

               //ret = ioctl(fd, PWMTMR_FUNC, &val);
               //if (ret < 0)
               //   pabort("can't set PWMTMR_FUNC");    
                       
               ret = ioctl(fd, PWM_CONFIG, &pwmconfig);
               if (ret < 0)
                  pabort("can't set PWM_CONFIG");   

               ret = ioctl(fd, PWMTMR_START, &val);
               if (ret < 0)
                  pabort("can't set PWMTMR_START");                     
            }else
               fprintf(stderr, "%s ERROR: invalid dutycycle[0,32], pin=%d\n", __FUNCTION__, pin);

            break;

         default:   
            break;
         }
         if(fd)
            close(fd);
     }
     else
     {
         fprintf(stderr, "%s ERROR: invalid pin, pin=%d\n", __FUNCTION__, pin);
         exit(-1);
     }      
}

