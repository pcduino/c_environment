#include "Arduino.h"
#include "wiring_private.h"

#define PWM_IF_PREFIX "/sys/class/leds/pwm"
#define PWM_IF_MAX "max_brightness"
#define PWM_IF "brightness"

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
	char path[128];
	 char buf[32];
	 char cmd[128];
	 int ret = -1;
	 int fd = -1;
	 int max_level = 0;
	 int map_level = 0;
		 
	 if ( (pin >= 0 && pin <= MAX_PWM_NUM) && 
		 (value >= 0 && value <= MAX_PWM_LEVEL) )
	 {
		 memset((void *)path, 0, sizeof(path));
		 sprintf(path, "%s%d/%s", PWM_IF_PREFIX, pin, PWM_IF_MAX);
		 fd = open(path, O_RDONLY);
		 if ( fd < 0 )
		 {
			 fprintf(stderr, "open %s failed\n", path);
			 exit(-1);
		 }
		 
		 ret = read(fd, buf, sizeof(buf));
		 close(fd);
		 
		 if ( ret <= 0 )
		 {
			 fprintf(stderr, "read %s failed\n", path);
			 exit(-1);
		 }
		 
		 max_level = atoi(buf);
		 map_level = (max_level * value)/MAX_PWM_LEVEL;
		 memset((void *)cmd, 0, sizeof(cmd));
		 sprintf(cmd, "echo %d > %s%d/%s", 
			 map_level, PWM_IF_PREFIX, pin, PWM_IF);
		 system(cmd);
	 }
	 else
	 {
		 fprintf(stderr, "%s ERROR: invalid pin, pin=%d\n", __FUNCTION__, pin);
		 exit(-1);
	 }		
}

