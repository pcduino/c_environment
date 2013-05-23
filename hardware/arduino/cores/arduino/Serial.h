#ifndef Serial_h
#define Serial_h

#include <inttypes.h>
#include "Stream.h"


// Define config for Serial.begin(baud, config);
#define SERIAL_5N1 0x00
#define SERIAL_6N1 0x02
#define SERIAL_7N1 0x04
#define SERIAL_8N1 0x06
#define SERIAL_5N2 0x08
#define SERIAL_6N2 0x0A
#define SERIAL_7N2 0x0C
#define SERIAL_8N2 0x0E
#define SERIAL_5E1 0x20
#define SERIAL_6E1 0x22
#define SERIAL_7E1 0x24
#define SERIAL_8E1 0x26
#define SERIAL_5E2 0x28
#define SERIAL_6E2 0x2A
#define SERIAL_7E2 0x2C
#define SERIAL_8E2 0x2E
#define SERIAL_5O1 0x30
#define SERIAL_6O1 0x32
#define SERIAL_7O1 0x34
#define SERIAL_8O1 0x36
#define SERIAL_5O2 0x38
#define SERIAL_6O2 0x3A
#define SERIAL_7O2 0x3C
#define SERIAL_8O2 0x3E

#define SERIAL_BUFFER_SIZE 1024
struct ring_buffer
{
  unsigned char buffer[SERIAL_BUFFER_SIZE];
  volatile unsigned int head;
  volatile unsigned int tail;
};

class HwSerial : public Stream
{
private:
 	int _fd;
	struct ring_buffer _rx_buffer;
	struct ring_buffer _tx_buffer;
	
public:
    HwSerial();
	~HwSerial();
    void begin(unsigned long, byte config = SERIAL_8N1);
    void end();
    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    virtual void flush(void);
    virtual int write(byte);
    inline int write(unsigned long n) { return write((byte)n); }
    inline int write(long n) { return write((byte)n); }
    inline int write(unsigned int n) { return write((byte)n); }
    inline int write(int n) { return write((byte)n); }
    //using Print::write; // pull in write(str) and write(buf, size) from Print
    operator bool();

public:
	int process_recv();		
};

extern void serialEventRun(void) __attribute__((weak));
extern HwSerial Serial;

#endif
