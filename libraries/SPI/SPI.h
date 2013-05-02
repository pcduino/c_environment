/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef _SPI_H_INCLUDED
#define _SPI_H_INCLUDED

#define SPI_CONTINUE 0
#define SPI_LAST     1

#define SPI_MODE0 0x00
#define SPI_MODE1 0x01
#define SPI_MODE2 0x02
#define SPI_MODE3 0x03

#define SPI_CLOCK_DIV1   0x00
#define SPI_CLOCK_DIV2   0x01
#define SPI_CLOCK_DIV4   0x02
#define SPI_CLOCK_DIV8   0x03
#define SPI_CLOCK_DIV16  0x04
#define SPI_CLOCK_DIV32  0x05
#define SPI_CLOCK_DIV64  0x06
#define SPI_CLOCK_DIV128 0x07


class SPIClass {
public:
  SPIClass(char bus);
  ~SPIClass();
  void begin();
  void end();

  void setBitOrder(int);
  void setDataMode(int);
  void setClockDivider(int);
  char transfer(char val, int transferMode);

private:
    int _devid;
    int _fd;
    int _speed;
};

extern SPIClass SPI;
extern SPIClass SPIEX;

#endif
