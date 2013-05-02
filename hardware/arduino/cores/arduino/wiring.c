#include "Arduino.h"
#include "wiring_private.h"

#define GPIO_MODE_DIR "/sys/devices/virtual/misc/gpio/mode/"
#define GPIO_PIN_DIR "/sys/devices/virtual/misc/gpio/pin/"
#define GPIO_IF_PREFIX "gpio"

#define ADC_IF "/proc/adc"

//#define PWM_IF_PREFIX "/sys/class/leds/pwm"
//#define PWM_IF_MAX "max_brightness"
//#define PWM_IF "brightness"


int gpio_pin_fd[MAX_GPIO_NUM+1];
int gpio_mode_fd[MAX_GPIO_NUM+1];
int adc_fd[MAX_ADC_NUM+1];
//int pwm_fd[MAX_PWM_NUM+1];



void init()
{
    int i;
     char path[1024];
     for( i = 0; i<= MAX_GPIO_NUM; ++i)
     {
         memset(path, 0, sizeof(path));
         sprintf(path, "%s%s%d", GPIO_PIN_DIR, GPIO_IF_PREFIX, i);
         gpio_pin_fd[i] = open(path, O_RDWR);
         if ( gpio_pin_fd[i] < 0 )
         {
             fprintf(stderr, "open %s failed\n", path);
             return;
         }
         
         memset(path, 0, sizeof(path));
         sprintf(path, "%s%s%d", GPIO_MODE_DIR, GPIO_IF_PREFIX, i);
         gpio_mode_fd[i] = open(path, O_RDWR);
         if ( gpio_mode_fd[i] < 0 )
         {
             fprintf(stderr, "open %s failed\n", path);
             return;
         } 
     }
     
     for( i = 0; i<= 5; ++i)
     {
         memset(path, 0, sizeof(path));
         sprintf(path, "%s%d", ADC_IF, i);
         adc_fd[i] = open(path, O_RDONLY);
         if ( adc_fd[i] < 0 )
         {
             fprintf(stderr, "open %s failed\n", path);
             return;
         }
     }

}
