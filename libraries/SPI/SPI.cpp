/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */
#include "Arduino.h"
#include "SPI.h"
#include <linux/spi/spidev.h>

SPIClass SPI(0);
SPIClass SPIEX(1);

static const char *spi_name = "/dev/spidev0.0";
static const char *spi1_name = "/dev/spidev1.0";
static const char *spi2_name = "/dev/spidev2.0";
static int _bits_per_word = 8;
SPIClass::SPIClass(char bus)
{
    _devid = bus;
    _fd = 0;
}

SPIClass::~SPIClass()
{
    end();
}

void SPIClass::begin() {
    int ret =0;
    int max_speed = 0, default_mode=0;
    int mode;
 
    if (_devid == 0) {
       hw_pinMode(SPI_CS, IO_SPI_FUNC);   //CS
       hw_pinMode(SPI_MOSI, IO_SPI_FUNC); //MOSI
       hw_pinMode(SPI_MISO, IO_SPI_FUNC); //MISO
       hw_pinMode(SPI_CLK, IO_SPI_FUNC);  //CLK
    }else if(_devid == 1) {
       hw_pinMode(SPIEX_CS, IO_SPIEX_FUNC);   //CS
       hw_pinMode(SPIEX_MOSI, IO_SPIEX_FUNC); //MOSI
       hw_pinMode(SPIEX_MISO, IO_SPIEX_FUNC); //MISO
       hw_pinMode(SPIEX_CLK, IO_SPIEX_FUNC);  //CLK
    }
        
    if (!_fd){
       if (_devid == 0) 
          _fd = open(spi_name, O_RDWR);
       else if (_devid == 1)
          _fd = open(spi2_name, O_RDWR);
          if (_fd < 0)
             _fd = open(spi1_name, O_RDWR);
    }
    if (_fd < 0)
        pabort("can't open device");
    
    _speed = 500000;
    ret = ioctl(_fd, SPI_IOC_RD_MODE, &default_mode);
    if (ret == -1)
        pabort("can't get spi mode");
    mode = default_mode;

    ret = ioctl(_fd, SPI_IOC_RD_MAX_SPEED_HZ, &max_speed);
    if (ret == -1)
        pabort("can't get max speed hz");
    _speed = max_speed;

    printf("init spi mode: 0x%x\n", mode);
    printf("bits per word: %d\n", _bits_per_word);
    printf("max speed: %d Hz (%d KHz)\n", _speed, _speed/1000);
}


void SPIClass::end() {
    if (_fd) 
        close(_fd);   
    _fd = 0;  
}

void SPIClass::setBitOrder(int bitOrder)
{
     int order = 0, ret = 0, mode = 0;
     /*
      * bits per word
      */
     if(bitOrder == LSBFIRST) 
         order = 1;
     else if(bitOrder == MSBFIRST) 
         order = 0;    
    
     ret = ioctl(_fd, SPI_IOC_WR_LSB_FIRST, &order);
     if (ret == -1)
         pabort("can't set bits order");

     ret = ioctl(_fd, SPI_IOC_RD_MODE, &mode);
     if (ret == -1)
         pabort("can't get spi mode");
     printf("setBitOrder->spi mode: 0x%x\n", mode);
}

void SPIClass::setDataMode(int mode)
{
    int ret = 0, smode = 0;
    /*
     * spi mode
     */
    ret = ioctl(_fd, SPI_IOC_RD_MODE, &smode);
    if (ret == -1)
        pabort("can't get spi mode");
    smode &= ~0x3;
    smode |= (mode&0x3);
    ret = ioctl(_fd, SPI_IOC_WR_MODE, &smode);
    if (ret == -1)
        pabort("can't set spi mode");
    ret = ioctl(_fd, SPI_IOC_RD_MODE, &smode);
    if (ret == -1)
        pabort("can't get spi mode");
    printf("setDataMode->spi mode: 0x%x\n", smode);
}

void SPIClass::setClockDivider(int rate)
{
    switch (rate)
    {
        case SPI_CLOCK_DIV1:
            break;
        case SPI_CLOCK_DIV2:
            _speed = _speed/2;
            break;
        case SPI_CLOCK_DIV4:
            _speed = _speed/4;
            break;
        case SPI_CLOCK_DIV8:
            _speed = _speed/8;
            break;
        case SPI_CLOCK_DIV16:
            _speed = _speed/16;
            break;
        case SPI_CLOCK_DIV32:
            _speed = _speed/32;
            break;
        case SPI_CLOCK_DIV64:
            _speed = _speed/64;
            break;
        case SPI_CLOCK_DIV128:
            _speed = _speed/128;
            break;

        default:
            break;
    }
}

char SPIClass::transfer(char val, int transferMode)
{
    int ret=0;
    char tx = val;
    char rx = 0;
    unsigned short delay_usecs = 0;
    struct spi_ioc_transfer tr;

    if (transferMode == SPI_CONTINUE) 
       delay_usecs = 0;
    else if (transferMode == SPI_LAST)  
       delay_usecs = 0xAA55;

    memset(&tr, 0x0, sizeof(spi_ioc_transfer));
    tr.tx_buf = (unsigned long)&tx;
    tr.rx_buf = (unsigned long)&rx;
    tr.len = 1;
    tr.speed_hz = _speed;
    tr.bits_per_word = _bits_per_word;
    tr.delay_usecs = delay_usecs;

    ret = ioctl(_fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
        pabort("can't send spi message");

    //printf("rx = %.2X \r\n", rx);

    return rx;

}

