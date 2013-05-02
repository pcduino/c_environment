#include "Arduino.h"
#include "wiring_private.h"
#include <linux/spi/spidev.h>

static const char *pwm_dev = "/dev/pwmtimer";
static const char *spi1_dev = "/dev/spidev1.0";
static const char *spi2_dev = "/dev/spidev2.0";

typedef struct tagPWM_Config {
    int channel;
    int dutycycle;
} PWM_Config,*pPWM_Config;

typedef struct tagPWM_Freq {
    int channel;
    int step;
    int pre_scale;
    unsigned int freq;
} PWM_Freq,*pPWM_Freq;

#define PWMTMR_START    (0x101)
#define PWMTMR_STOP     (0x102)
#define PWMTMR_FUNC     (0x103)
#define PWMTMR_TONE     (0x104)
#define PWM_CONFIG      (0x105)
#define HWPWM_DUTY      (0x106)
#define PWM_FREQ        (0x107)

#define MAX_PWMTMR_FREQ (2000)   //2kHz pin 3,9,10,11
#define MIN_PWMTMR_FREQ (126)    //126Hz pin 3,9,10,11
#define MAX_PWMHW_FREQ  (20000)  //20kHz pin 5,6

//under construct
uint8_t analog_reference = 0;
void analogReference(uint8_t mode)
{
    analog_reference = mode;
}

static int spi_adc_read_data(int channel)
{
    int adc_val = 0, reg_val = 0, mode = 0;
    int _fd = 0, ret = 0;
    char rxbuf[2], txbuf[2];
    struct spi_ioc_transfer tr;

    pinMode(SPIEX_CS, OUTPUT); //CS
    //hw_pinMode(SPIEX_CS, IO_SPIEX_FUNC);   //CS
    hw_pinMode(SPIEX_MOSI, IO_SPIEX_FUNC); //MOSI
    hw_pinMode(SPIEX_MISO, IO_SPIEX_FUNC); //MISO
    hw_pinMode(SPIEX_CLK, IO_SPIEX_FUNC);  //CLK

    _fd = open(spi2_dev, O_RDWR);
    if (_fd < 0)
       _fd = open(spi1_dev, O_RDWR);
    if (_fd < 0)
        pabort("can't open spi device");

    //MODE0
    ret = ioctl(_fd, SPI_IOC_RD_MODE, &mode);
    if (ret == -1)
        pabort("can't get spi mode");
    mode &= ~0x3;
    ret = ioctl(_fd, SPI_IOC_WR_MODE, &mode);
    if (ret == -1)
        pabort("can't set spi mode");

    //MSBFIRST  
    mode = 0;
    ret = ioctl(_fd, SPI_IOC_WR_LSB_FIRST, &mode);
    if (ret == -1)
        pabort("can't set bits order");

    digitalWrite(SPIEX_CS, 0);
    reg_val = ( 0xc300 | ((channel&0x7) << 11) );
    txbuf[0] = (reg_val>>8)&0xFF;
    txbuf[1] = reg_val&0xFF;

    memset(&tr, 0x0, sizeof(tr));
    tr.tx_buf = (unsigned long)txbuf;
    tr.len = 2;
    tr.speed_hz = 1000000;
    tr.bits_per_word = 8;
    tr.delay_usecs = 0xFFFF;

    ret = ioctl(_fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
        pabort("can't send spi message");
    digitalWrite(SPIEX_CS, 1);

    usleep(10);
    digitalWrite(SPIEX_CS, 0);
    usleep(180);
    digitalWrite(SPIEX_CS, 1);
    usleep(10);

    digitalWrite(SPIEX_CS, 0);
    memset(rxbuf, 0x0, sizeof(rxbuf));
    memset(&tr, 0x0, sizeof(tr));
    tr.rx_buf = (unsigned long)rxbuf;
    tr.len = 2;
    tr.speed_hz = 1000000;
    tr.bits_per_word = 8;
    tr.delay_usecs = 0xFFFF;
    ret = ioctl(_fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
        pabort("can't send spi message");

    adc_val = ( (rxbuf[0] << 8) | rxbuf[1] );
    digitalWrite(SPIEX_CS, 1);

    if (_fd) 
       close(_fd);

    return adc_val>>4;
}

int analogRead(uint8_t pin)
{
    char str[10];
    char buf[32];    
    int ret = -1;
    int fd = -1;
    char *p = NULL;
        
    if ( pin >= 0 && pin <= 5 )
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
    else if ( pin >= 6 && pin <= MAX_ADC_NUM )
    {
       ret = spi_adc_read_data(pin-6);
    }
    else
    {
        fprintf(stderr, "%s ERROR: invalid pin, pin=%d\n", __FUNCTION__, pin);
        exit(-1);
    }      
    return ret;

}

/*  
 * pin(3/9/10/11) support frequency[125-2000]Hz @different dutycycle
 * pin(5/6) support frequency[195,260,390,520,781] @256 dutycycle
 */
int pwmfreq_set(uint8_t pin, unsigned int freq)
{
     int ret = -1;
     int fd = -1;
     PWM_Freq pwmfreq;
         
     if ( (pin == 3 || pin == 5 || pin == 6 || pin == 9 || pin == 10 || pin == 11) && (freq > 0) )
     {
         pwmfreq.channel = pin;
         pwmfreq.freq = freq;
         pwmfreq.step = 0;
         fd = open(pwm_dev, O_RDONLY);
         if ( fd < 0 )
             pabort("open pwm device fail");

         switch (pin) 
         {
         case 5:
         case 6:
            if ( (freq == 195) || (freq == 260) || (freq == 390) 
                || (freq == 520) || (freq == 781) )
            {
               ret = ioctl(fd, PWM_FREQ, &pwmfreq);
               if (ret < 0)
                  pabort("can't set PWM_FREQ");
            }else
               fprintf(stderr, "%s ERROR: invalid frequency, should be [195,260,390,520,781], pin=%d\n", __FUNCTION__, pin);

            break;
         case 3:
         case 9:    
         case 10:   
         case 11:   
            if ((freq >= MIN_PWMTMR_FREQ) && (freq <= MAX_PWMTMR_FREQ)){
               pin = 
               ret = ioctl(fd, PWMTMR_STOP, &pwmfreq.channel);
               if (ret < 0)
                  pabort("can't set PWMTMR_STOP");  
                          
               ret = ioctl(fd, PWM_FREQ, &pwmfreq);
               if (ret < 0)
                  pabort("can't set PWM_FREQ");   
            }else
               fprintf(stderr, "%s ERROR: invalid frequency[%d,%d], pin=%d\n", __FUNCTION__, MIN_PWMTMR_FREQ, MAX_PWMTMR_FREQ, pin);

            break;

         default:   
            break;
         }

         if(fd)
             close(fd);
         return pwmfreq.step;
     }
     else
     {
         fprintf(stderr, "%s ERROR: invalid pin, pin=%d\n", __FUNCTION__, pin);
         exit(-1);
     } 
     return 0;
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
            //ret = ioctl(fd, PWMTMR_STOP, &pwmconfig.channel);
            //if (ret < 0)
            //   pabort("can't set PWMTMR_STOP");  
                       
            ret = ioctl(fd, PWM_CONFIG, &pwmconfig);
            if (ret < 0)
               pabort("can't set PWM_CONFIG");   

            ret = ioctl(fd, PWMTMR_START, &val);
            if (ret < 0)
               pabort("can't set PWMTMR_START");
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

