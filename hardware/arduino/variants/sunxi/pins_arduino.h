/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: wiring.h 249 2007-02-03 16:52:51Z mellis $
*/


#undef MAX_GPIO_NUM
#define MAX_GPIO_NUM 19

#undef MAX_GPIO_MODE_NUM
#define MAX_GPIO_MODE_NUM 8

#undef MAX_PWM_NUM
#define MAX_PWM_NUM 5

#undef MAX_ADC_NUM
#define MAX_ADC_NUM 11

#define GPIO0  0 
#define GPIO1  1 
#define GPIO2  2 
#define GPIO3  3
#define GPIO4  4
#define GPIO5  5
#define GPIO6  6 
#define GPIO7  7
#define GPIO8  8 
#define GPIO9  9 
#define GPIO10  10 
#define GPIO11  11 
#define GPIO12  12 
#define GPIO13  13
#define GPIO14  14

#define IO_SPI_FUNC   2
#define IO_UART_FUNC  3

#define SPI_CS GPIO10
#define SPI_MOSI GPIO11
#define SPI_MISO GPIO12
#define SPI_CLK GPIO13


