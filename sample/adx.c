/***************************************************************************
spi.cpp

Example code for SPI access on the pcDuino via C++. This code *should* work
in the future, but as of release, only SPI output works. Data presented on
the MISO pin does not register with the device!

26 Mar 2013 - Mike Hord, SparkFun Electronics

This code is beerware- if you find it useful, please by me (or, for that
matter, any other SparkFun employee you met) a pint next time you meet us at
the local.

***************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

// The SPI mode defines the resting polarity of the clock signal and the
//   clock edge upon which data is latched.
#define SPI_MODE0 0x00  // rest = 0, latch on rise
#define SPI_MODE1 0x01  // rest = 0, latch on fall
#define SPI_MODE2 0x02  // rest = 1, latch on fall
#define SPI_MODE3 0x03  // rest = 1, latch on rise


#define INPUT 0
#define OUTPUT 1
#define SPI 2


#define GPIO_MODE_PATH "/sys/devices/virtual/misc/gpio/mode/"
#define GPIO_FILENAME "gpio"


void writeFile(int fileID, int value);
void setPinMode(int pinID, int mode);
void configurePins(void);

static const char *spi_name = "/dev/spidev0.0";

int main(void)
{
  int res = 0;  // We can use this to monitor the results of any operation.

  // The very first thing we need to do is make sure that the pins are set
  //   to SPI mode, rather than, say, GPIO mode.
  char path[256];
  
  for (int i = 10; i<=13; i++)
  {
    // Clear the path variable...
    memset(path,0,sizeof(path));
    // ...then assemble the path variable for the current pin mode file...
    sprintf(path, "%s%s%d", GPIO_MODE_PATH, GPIO_FILENAME, i);
    // ...and create a file descriptor...
    int pinMode = open(path, O_RDWR);
    // ...which we then use to set the pin mode to SPI...
    setPinMode(pinMode, SPI);
    // ...and then, close the pinMode file.
    close(pinMode);
  }
    
  // As usual, we begin the relationship by establishing a file object which
  //   points to the SPI device.
  int spiDev = open(spi_name, O_RDWR);
  
  // We'll want to configure our SPI hardware before we do anything else. To do
  //   this, we use the ioctl() function. Calls to this function take the form
  //   of a file descriptor, a "command", and a value. The returned value is
  //   always the result of the operation; pass it a pointer to receive a value
  //   requested from the SPI peripheral.
  
  // Start by setting the mode. If we wanted to *get* the mode, we could
  //   use SPI_IOC_RD_MODE instead. In general, the "WR" can be replaced by
  //   "RD" to fetch rather than write. Also note the somewhat awkward
  //   setting a variable rather than passing the constant. *All* data sent
  //   via ioctl() must be passed by reference!
  int mode = SPI_MODE0;
  ioctl(spiDev, SPI_IOC_WR_MODE, &mode);
  
  // The maximum speed of the SPI bus can be fetched. You'll find that, on the
  // it's 12MHz.
  int maxSpeed = 0;
  ioctl(spiDev, SPI_IOC_RD_MAX_SPEED_HZ, &maxSpeed);
  printf("Max speed: %dHz\n", maxSpeed);
  
  // In rare cases, you may find that a device expects data least significant
  //   bit first; in that case, you'll need to set that up. Writing a 0
  //   indicates MSb first; anything else indicates LSb first.
  int lsb_setting = 0;
  ioctl(spiDev, SPI_IOC_WR_LSB_FIRST, &lsb_setting);
  
  // Some devices may require more than 8 bits of data per transfer word. The
  //   SPI_IOC_WR_BITS_PER_WORD command allows you to change this; the default,
  //   0, corresponds to 8 bits per word.
  int bits_per_word = 0;
  ioctl(spiDev, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word);
  
  // Okay, now that we're all set up, we can start thinking about transferring
  //   data. This, too, is done through ioctl(); in this case, there's a special
  //   struct (spi_ioc_transfer) defined in spidev.h which holds the needful
  //   info for completing a transfer. Its members are:
  //    * tx_buf - a pointer to the data to be transferred
  //    * rx_buf - a pointer to storage for received data
  //    * len - length in bytes of tx and rx buffers
  //    * speed_hz - the clock speed, in Hz
  //    * delay_usecs - delay between last bit and deassertion of CS
  //    * bits_per_word - override global word length for this transfer
  //    * cs_change - strobe chip select between transfers?
  //    * pad - ??? leave it alone.
  
  // For this example, we'll be reading the address location of an ADXL362
  //   accelerometer, then writing a value to a register and reading it back.
  //   We'll do two transfers, for ease of data handling: the first will
  //   transfer the "read register" command (0x0B) and the address (0x02), the
  //   second will dump the response back into the same buffer.
#if 0
  struct spi_ioc_transfer xfer[2];
  memset(xfer, 0, sizeof(xfer));
  char dataBuffer[2];
  char rxBuffer;
  dataBuffer[0] = 0x0B;
  dataBuffer[1] = 0x02;
  xfer[0].tx_buf = (unsigned long)dataBuffer;
  xfer[0].len = 2;
  xfer[0].speed_hz = 500000;
  xfer[0].cs_change = 0;
  xfer[0].bits_per_word = 8;
  xfer[1].rx_buf = (unsigned long)&rxBuffer;
  xfer[1].len = 1;
  xfer[1].speed_hz = 500000;
  xfer[1].cs_change = 1;
  xfer[1].bits_per_word = 8;
  res = ioctl(spiDev, SPI_IOC_MESSAGE(2), xfer);
  printf("SPI result: %d\n", res);
  printf("Device ID: 0x%x\n", rxBuffer);

#else
  struct spi_ioc_transfer xfer;
  memset(&xfer, 0, sizeof(xfer));
  char dataBuffer[3];
  char rxBuffer[3];
  dataBuffer[0] = 0x0B;
  dataBuffer[1] = 0x02;
  dataBuffer[2] = 0x00;
  xfer.tx_buf = (unsigned long)dataBuffer;
  xfer.rx_buf = (unsigned long)rxBuffer;
  xfer.len = 3;
  xfer.speed_hz = 500000;
  xfer.cs_change = 1;
  xfer.bits_per_word = 8;
  res = ioctl(spiDev, SPI_IOC_MESSAGE(1), &xfer);
  printf("SPI result: %d\n", res);
  printf("Device ID: 0x%x, 0x%x 0x%x\n", rxBuffer[2], rxBuffer[1], rxBuffer[0]);

#endif
}

void setPinMode(int pinID, int mode)
{
  writeFile(pinID, mode);
}

// While it seems okay to only *read* the first value from the file, you
//   seemingly must write four bytes to the file to get the I/O setting to
//   work properly. This function does that.
void writeFile(int fileID, int value)
{
  char buffer[4];  // A place to build our four-byte string.
  memset((void *)buffer, 0, sizeof(buffer)); // clear the buffer out.
  sprintf(buffer, "%d", value);
  lseek(fileID, 0, SEEK_SET);   // Make sure we're at the top of the file!
  int res = write(fileID, buffer, sizeof(buffer));
}


