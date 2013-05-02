/*
  TwoWire.cpp - TWI/I2C library for Wiring 

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
*/

extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
}
#include "Arduino.h"
#include "Wire.h"
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#define I2CCLOCK_CHANGE 0x0740

// Initialize Class Variables //////////////////////////////////////////////////
int TwoWire::i2c_handle = 0;
    
uint8_t TwoWire::rxBuffer[BUFFER_LENGTH];
uint8_t TwoWire::rxBufferIndex = 0;
uint8_t TwoWire::rxBufferLength = 0;

uint8_t TwoWire::txBuffer[BUFFER_LENGTH];
uint8_t TwoWire::txBufferIndex = 0;
uint8_t TwoWire::txBufferLength = 0;

uint8_t TwoWire::transmitting = 0;
void (*TwoWire::user_onRequest)(void);
void (*TwoWire::user_onReceive)(int);

// Constructors ////////////////////////////////////////////////////////////////

TwoWire::TwoWire()
{
}


// Public Methods //////////////////////////////////////////////////////////////

void TwoWire::begin(void)
{
  rxBufferIndex = 0;
  rxBufferLength = 0;

  txBufferIndex = 0;
  txBufferLength = 0;
 
  if (!i2c_handle) {
     if ((i2c_handle = open("/dev/i2c-2", O_RDWR)) < 0) 
        pabort("can't open device");       
  }
}

void TwoWire::begin(uint8_t address)
{
  /* not support slave mode */
  return;
}

void TwoWire::begin(int address)
{
  /* not support slave mode */
  return;
}

// bus freq range 10kHz-400kHz
void TwoWire::setBusFreq(unsigned int speed_hz)
{
  int libi2cdev;

  if ((speed_hz > 400000) || (speed_hz < 10000))
  {
     printf("invalid bus freq, range[10000,400000]\r\n");
     return;
  }

  if ((libi2cdev = open("/dev/hwi2c", O_RDWR)) < 0) 
    pabort("can't open device");       
  
  if (ioctl(libi2cdev, I2CCLOCK_CHANGE, &speed_hz) < 0) 
    pabort("change I2C bus freq fail");

  close(libi2cdev);
}

uint8_t TwoWire::requestFrom(int address, int quantity)
{
  uint8_t read_out = 0;

  if(!i2c_handle) 
    return 0;

  // clamp to buffer length
  if(quantity > BUFFER_LENGTH){
    quantity = BUFFER_LENGTH;
  }

  // 7 or 10 bits address 
  if (address <= 0x7F) {
     if (ioctl(i2c_handle, I2C_TENBIT, 0) < 0) 
       pabort("Set I2C 7-bits addres flag fail");
  }else if(address <= 0x3FF){
     if (ioctl(i2c_handle, I2C_TENBIT, 1) < 0) 
       pabort("Set I2C 10-bits addres flag fail");
  }
  
  if (ioctl(i2c_handle, I2C_SLAVE, address) < 0) 
    pabort("Set I2C slave addres fail");
  
  // perform blocking read into buffer
  int ret = ::read(i2c_handle, rxBuffer, quantity);
  if (ret > 0) 
     read_out = ret;
  else
     read_out = 0;

  // set rx buffer iterator vars
  rxBufferIndex = 0;
  rxBufferLength = read_out;

  return read_out;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity)
{
  return requestFrom((int)address, (int)quantity);
}

void TwoWire::beginTransmission(int address)
{
  if(!i2c_handle) 
    return;

  // indicate that we are transmitting
  transmitting = 1;

  // reset tx buffer iterator vars
  txBufferIndex = 0;
  txBufferLength = 0;

  // 7 or 10 bits address 
  if (address <= 0x7F) {
     if (ioctl(i2c_handle, I2C_TENBIT, 0) < 0) 
       pabort("Set I2C 7-bits addres flag fail");
  }else if(address <= 0x3FF){
     if (ioctl(i2c_handle, I2C_TENBIT, 1) < 0) 
       pabort("Set I2C 10-bits addres flag fail");
  }

  if (ioctl(i2c_handle, I2C_SLAVE, address) < 0) 
    pabort("Set I2C slave addres fail");
}

void TwoWire::beginTransmission(uint8_t address)
{
  beginTransmission((int)address);
}

uint8_t TwoWire::endTransmission(void)
{
  if(!i2c_handle) 
    return 0;

  // transmit buffer (blocking)
  int8_t ret = ::write(i2c_handle, txBuffer, txBufferLength);
  if (ret != txBufferLength) {
     printf("i2c transaction failed\r\n");
     ret = 4;
  }else
     ret = 0;

  // reset tx buffer iterator vars
  txBufferIndex = 0;
  txBufferLength = 0;
  // indicate that we are done transmitting
  transmitting = 0;
  return ret;
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
int TwoWire::write(byte data)
{
  if(transmitting){
  // in master transmitter mode
    // don't bother if buffer is full
    if(txBufferLength >= BUFFER_LENGTH){
      setWriteError();
      return 0;
    }
    // put byte in tx buffer
    txBuffer[txBufferIndex] = data;
    ++txBufferIndex;
    // update amount in buffer   
    txBufferLength = txBufferIndex;
  }else{
  // in slave send mode
    // reply to master
  }
  return 1;
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
  if(transmitting){
  // in master transmitter mode
    for(size_t i = 0; i < quantity; ++i){
      write(data[i]);
    }
  }else{
  // in slave send mode
    // reply to master
  }
  return quantity;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::available(void)
{
  return rxBufferLength - rxBufferIndex;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::read(void)
{
  int value = -1;
  
  // get each successive byte on each call
  if(rxBufferIndex < rxBufferLength){
    value = rxBuffer[rxBufferIndex];
    ++rxBufferIndex;
  }

  return value;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::peek(void)
{
  return 0;  
}

void TwoWire::flush(void)
{
  // XXX: to be implemented.
}

// behind the scenes function that is called when data is received
void TwoWire::onReceiveService(uint8_t* inBytes, int numBytes)
{
}

// behind the scenes function that is called when data is requested
void TwoWire::onRequestService(void)
{
}

// sets function called on slave write
void TwoWire::onReceive( void (*function)(int) )
{
}

// sets function called on slave read
void TwoWire::onRequest( void (*function)(void) )
{
}

// Preinstantiate Objects //////////////////////////////////////////////////////

TwoWire Wire;

