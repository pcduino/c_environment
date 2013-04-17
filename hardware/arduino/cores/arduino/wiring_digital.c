#include "Arduino.h"
#include "wiring_private.h"

static int write_to_file(int fd, char *str, int len)
{
    int ret = -1;

    lseek(fd, 0, SEEK_SET);
    ret = write(fd, str, len);
    if ( ret <= 0 )
    {
        fprintf(stderr, "write %d failed\n", fd);
        return -1;
    }
    return ret;
}

void hw_pinMode(uint8_t pin, uint8_t mode)
{
    int fd;
     char buf[4];
     int ret = -1;
    
     if ( (pin >= 0 && pin <=  MAX_GPIO_NUM) && (mode <= MAX_GPIO_MODE_NUM) )
     {
         memset((void *)buf, 0, sizeof(buf));
         sprintf(buf, "%d", mode);
         ret = write_to_file(gpio_mode_fd[pin], buf, sizeof(buf));
         if ( ret <= 0 )
         {
             fprintf(stderr, "write gpio %d mode failed\n", pin);
             exit(-1);
         }
     }
     else
     {
         fprintf(stderr, "%s ERROR: invalid pin or mode, pin=%d, mode=%d\n",
             __FUNCTION__, pin, mode);
         exit(-1);
      }
}

void pinMode(uint8_t pin, uint8_t mode)
{
    switch (pin)
    {         
    case 3:
    case 9:
    case 10:
    case 11:
        {
          int ret = -1;
          int fd = -1;
          unsigned long val = pin;
         
          fd = open("/dev/pwmtimer", O_RDONLY);
          if ( fd < 0 )
             pabort("open pwm device fail");
                
          ret = ioctl(fd, 0x102, &val); //PWMTMR_STOP
          if (ret < 0)
               pabort("can't set PWMTMR_STOP");

          if(fd)
            close(fd);
        }
        break;

    default:
        break; 
    }
        
    switch (mode)
    {         
    case INPUT:
    case OUTPUT:
        hw_pinMode(pin, mode);  
        break;  
    case INPUT_PULLUP:
        hw_pinMode(pin, 8);  
        break;

    default:
        break; 
    }
}

void digitalWrite(uint8_t pin, uint8_t value)
{
    char buf[4]; 
     int ret = -1;
     
     if ( (pin >= 0 && pin <=  MAX_GPIO_NUM) && (value == HIGH || value == LOW) )
     {
         memset((void *)buf, 0, sizeof(buf));
         sprintf(buf, "%d", value);
         ret = write_to_file(gpio_pin_fd[pin], buf, sizeof(buf));
         if ( ret <= 0 )
         {
             fprintf(stderr, "write gpio %d  failed\n", pin);
             exit(-1);
         }
     }
     else
     {
         fprintf(stderr, "%s ERROR: invalid pin or mode, pin=%d, value=%d\n",
             __FUNCTION__, pin, value);
         exit(-1);
      }    
}

int digitalRead(uint8_t pin)
{
    char path[128];
    char buf[4];    
    int ret = -1;
    int fd = -1;     
        
    if ( pin >= 0 && pin <= MAX_GPIO_NUM )
    {
        memset((void *)buf, 0, sizeof(buf));
        lseek(gpio_pin_fd[pin], 0, SEEK_SET);
        ret = read(gpio_pin_fd[pin], buf, sizeof(buf));

        if ( ret <= 0 )
        {
            fprintf(stderr, "read gpio %d failed\n", pin);
            exit(-1);
        }
        
        ret = buf[0] - '0';
        switch( ret )
        {
            case LOW:
            case HIGH:
                break;
            default:
                ret = -1;
                break;
        }
    }
    else
    {
        fprintf(stderr, "%s ERROR: invalid pin, pin=%d\n", __FUNCTION__, pin);
        exit(-1);
    }      
    return ret;

}
