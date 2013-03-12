#
# Assume standlone toolchain
#
#CC  = arm-linux-gnueabihf-gcc
#CXX = arm-linux-gnueabihf-g++
#AR  = arm-linux-gnueabihf-ar

DIR=$(shell pwd)

INCLUDES = \
	-I$(DIR) \
	-I$(DIR)/hardware \
	-I$(DIR)/hardware/arduino \
	-I$(DIR)/hardware/arduino/cores \
	-I$(DIR)/hardware/arduino/cores/arduino \
	-I$(DIR)/hardware/arduino/variants \
	-I$(DIR)/hardware/arduino/variants/sunxi \
	-I$(DIR)/libraries \
	-I$(DIR)/libraries/Serial \
	-I$(DIR)/libraries/SPI \
	-I$(DIR)/libraries/Wire 

CFLAGS =
#CFLAGS = $(INCLUDES)
#CFLAGS += -march=armv7-a -mfpu=neon

SRCS = \
	hardware/arduino/cores/arduino/main.cpp \
	hardware/arduino/cores/arduino/platform.cpp \
	hardware/arduino/cores/arduino/Print.cpp \
	hardware/arduino/cores/arduino/Stream.cpp \
	hardware/arduino/cores/arduino/Tone.cpp \
	hardware/arduino/cores/arduino/WInterrupts.c \
	hardware/arduino/cores/arduino/wiring.c \
	hardware/arduino/cores/arduino/wiring_analog.c \
	hardware/arduino/cores/arduino/wiring_digital.c \
	hardware/arduino/cores/arduino/wiring_pulse.c \
	hardware/arduino/cores/arduino/wiring_shift.c \
	hardware/arduino/cores/arduino/WMath.cpp \
	hardware/arduino/cores/arduino/WString.cpp \
	libraries/Serial/Serial.cpp \
	libraries/Wire/Wire.cpp \
	libraries/SPI/SPI.cpp 
	
#OBJS = $(SRCS:%.c=%.o)
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SRCS)))

%.o: %.cpp
	@rm -f $@ 
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@ -Wno-deprecated-declarations

%.o: %.c
	@rm -f $@ 
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ -Wno-deprecated-declarations

LIB = libarduino.a 

all: $(LIB)
	make -C sample/


$(LIB): $(OBJS)
	$(AR) cq $(LIB) $(OBJS)

clean:
	rm -f $(LIB) $(OBJS)


