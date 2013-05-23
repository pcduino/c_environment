#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Arduino.h"
#include "wiring_private.h"
#include <termios.h>
#include "Serial.h"


HwSerial Serial;

void serialEvent() __attribute__((weak));
#define serialEvent_implemented
void serialEvent()
{
    Serial.process_recv();
}

void serialEventRun(void)
{
#ifdef serialEvent_implemented
    serialEvent();
#endif
}

static const char* serial_name = "/dev/ttyS1";
static inline char get_databit(byte config)
{
    switch (config)
    {  
    case SERIAL_5N1:
    case SERIAL_5N2:
    case SERIAL_5E1:
    case SERIAL_5E2:
    case SERIAL_5O1:
    case SERIAL_5O2:
        return CS5;
    case SERIAL_6N1:
    case SERIAL_6N2:
    case SERIAL_6E1:
    case SERIAL_6E2:
    case SERIAL_6O1:
    case SERIAL_6O2:
        return CS6;
    case SERIAL_7N1:
    case SERIAL_7N2:
    case SERIAL_7E1:
    case SERIAL_7E2:
    case SERIAL_7O1:
    case SERIAL_7O2:
        return CS7;
    case SERIAL_8N1:
    case SERIAL_8N2:
    case SERIAL_8E1:
    case SERIAL_8E2:
    case SERIAL_8O1:
    case SERIAL_8O2:
    default:    
        return CS8;
    }
}

static inline char get_stopbit(byte config)
{
    switch (config)
    {   
    case SERIAL_5N2:
    case SERIAL_6N2:
    case SERIAL_7N2:
    case SERIAL_8N2:
    case SERIAL_5E2:
    case SERIAL_6E2:
    case SERIAL_7E2:
    case SERIAL_8E2:
    case SERIAL_5O2:
    case SERIAL_6O2:
    case SERIAL_7O2:
    case SERIAL_8O2:
        return 2;
    case SERIAL_5N1:
    case SERIAL_6N1:
    case SERIAL_7N1:
    case SERIAL_8N1:
    case SERIAL_5E1:
    case SERIAL_6E1:
    case SERIAL_7E1:
    case SERIAL_8E1:
    case SERIAL_5O1:
    case SERIAL_6O1:
    case SERIAL_7O1:
    case SERIAL_8O1:
    default:    
        return 1;
    } 
}

static inline char get_parity(byte config)
{
    switch (config) 
    {   
    case SERIAL_5N1:
    case SERIAL_5N2:
    case SERIAL_6N1:
    case SERIAL_6N2:
    case SERIAL_7N1:
    case SERIAL_7N2:
    case SERIAL_8N1:
    case SERIAL_8N2:
    default:
        return 'N'; 
    case SERIAL_5O1:
    case SERIAL_5O2:
    case SERIAL_6O1:
    case SERIAL_6O2:
    case SERIAL_7O1:
    case SERIAL_7O2:
    case SERIAL_8O1:
    case SERIAL_8O2:
        return 'O';
    case SERIAL_5E1:
    case SERIAL_5E2:
    case SERIAL_6E1:
    case SERIAL_6E2:
    case SERIAL_7E1:
    case SERIAL_7E2:
    case SERIAL_8E1:
    case SERIAL_8E2:
        return 'E';
    }
}

static inline int get_valid_baud(unsigned long speed)
{
    switch (speed) 
    {   
    case 300:
       return B300;
    case 600:
       return B600;
    case 1200:
       return B1200;
    case 2400:
       return B2400;
    case 4800:
       return B4800;
    case 9600:
       return B9600;
    case 14400:
       return 0;
    case 19200:
       return B19200;
    case 28800:
       return 0;
    case 38400:
       return B38400;
    case 57600:
       return B57600;
    case 115200:
       return B115200;

    default:
        return 0; 
    }
}

//static void signal_handler_IO ()
//{
//  serial.process_recv();
//  serial.process_send();
//}


//under construct
HwSerial::HwSerial()
{
    _rx_buffer.head = _rx_buffer.tail = 0;
    _tx_buffer.head = _tx_buffer.tail = 0;
    _fd = -1;
}

HwSerial::~HwSerial()
{
    end();
}

void HwSerial::begin(unsigned long baud, byte config)
{
    int ret;
    struct termios   Opt;
    //struct sigaction saio;  
    hw_pinMode(GPIO0, IO_UART_FUNC); //uart_rx
    hw_pinMode(GPIO1, IO_UART_FUNC); //uart_tx

    _fd = open(serial_name, O_RDWR| O_NOCTTY | O_NONBLOCK );
    if (_fd < 0 )
    {
        pabort("can't open tty");
    }

    tcgetattr(_fd, &Opt); 
    tcflush(_fd, TCIOFLUSH);  

    int rate = get_valid_baud(baud);
    if(rate > 0)
    {
        cfsetispeed(&Opt, rate);  
        cfsetospeed(&Opt, rate);        
    }
    
    Opt.c_cflag &= ~CSIZE; 
    Opt.c_cflag |= get_databit(config);

    switch (get_parity(config))
    {   
    case 'N':
    default:
        Opt.c_cflag &= ~PARENB;  
        Opt.c_iflag &= ~INPCK;    
        break;  
    case 'O':
        Opt.c_cflag |= (PARODD | PARENB);  
        Opt.c_iflag |= INPCK;            
        break;  
    case 'E':
        Opt.c_cflag |= PARENB;         
        Opt.c_cflag &= ~PARODD;        
        Opt.c_iflag |= INPCK;     
        break;
    }  
    
    switch (get_stopbit(config))
    {   
    case 1:
    default:
        Opt.c_cflag &= ~CSTOPB;  
        break;  
    case 2:
        Opt.c_cflag |= CSTOPB;  
        break;
    }

    //Opt.c_cflag |= (CLOCAL | CREAD); 
    Opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    //Opt.c_lflag &= ~(ECHO);

    //Opt.c_cc[VTIME] = 0; 
    //Opt.c_cc[VMIN] = 0; 

    //saio.sa_handler = signal_handler_IO;
    //sigemptyset (&(saio.sa_mask));
    //saio.sa_flags = 0;
    //saio.sa_restorer = NULL;
    //sigaction (SIGIO, &saio, NULL);
    //if (-1 == fcntl (_fd, F_SETFL, O_ASYNC))
    //   pabort("SETFL SIGIO");
    //if (-1 == fcntl (_fd, F_SETOWN, getpid ()))
    //    pabort("F_SETOWN SIGIO");

    ret = tcsetattr(_fd, TCSANOW, &Opt);  
    if  (ret != 0)
        pabort("can't set tty baut");
    tcflush(_fd,TCIOFLUSH);  
}

void HwSerial::end()
{
    if(_fd)
        close(_fd);
    _fd = -1;
}

int HwSerial::available(void)
{
    if(_rx_buffer.head <= _rx_buffer.tail)
        return (_rx_buffer.tail - _rx_buffer.head);
    else
        return (SERIAL_BUFFER_SIZE - _rx_buffer.head + _rx_buffer.tail);
}

int HwSerial::peek(void)
{
    if (_rx_buffer.head == _rx_buffer.tail) {
      return -1;
    } else {
      return _rx_buffer.buffer[_rx_buffer.head];
    }
}

int HwSerial::read(void)
{
    if (_rx_buffer.head == _rx_buffer.tail) {
      return -1;
    } else {
      unsigned char c = _rx_buffer.buffer[_rx_buffer.head];
      _rx_buffer.head = (unsigned int)(_rx_buffer.head + 1) % SERIAL_BUFFER_SIZE;
      return c;
    }
}

void HwSerial::flush()
{
    _rx_buffer.head = _rx_buffer.tail = 0;
    _tx_buffer.head = _tx_buffer.tail = 0;
    //::flush(_fd);
    tcflush(_fd, TCIOFLUSH);
}

int HwSerial::write(byte c)
{
   return (::write(_fd, &c ,1));
}


int HwSerial::process_recv()
{
    int len =  SERIAL_BUFFER_SIZE - available();
    int retval = 0;

    if ( _fd < 0 )
        return -1;

    if(len > 0)
    {
        //fd_set   fs_read;
        //struct timeval tv_timeout={_timeout/1000,(_timeout%1000)*1000};
        //FD_ZERO (&fs_read);
        //FD_SET (_fd, &fs_read);
        //retval = select (_fd+1, &fs_read, NULL, NULL, &tv_timeout);    
         //if (retval>0) 
         {
             //if(FD_ISSET(_fd,&fs_read))
             {
                 char buf[SERIAL_BUFFER_SIZE];
                 retval = ::read(_fd, buf, len);
                 if(retval > 0)
                 {
                    if(retval >=  SERIAL_BUFFER_SIZE -_rx_buffer.tail)
                    {
                        memcpy(_rx_buffer.buffer + _rx_buffer.tail, buf, SERIAL_BUFFER_SIZE - _rx_buffer.tail);
                        memcpy(_rx_buffer.buffer, buf + SERIAL_BUFFER_SIZE - _rx_buffer.tail, retval - (SERIAL_BUFFER_SIZE - _rx_buffer.tail));
                        _rx_buffer.tail = retval - ( SERIAL_BUFFER_SIZE - _rx_buffer.tail);
                    }
                    else
                    {
                        memcpy(_rx_buffer.buffer + _rx_buffer.tail, buf, retval);
                        _rx_buffer.tail += retval;
                    }
                 }
             }
         }
        }
     return retval;
}

HwSerial::operator bool() {
    return true;
}
