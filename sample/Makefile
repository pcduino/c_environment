DIR=$(shell pwd)/../

INCS = -I.\
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
	-I$(DIR)/libraries/Wire \
	-I$(DIR)/libraries/LiquidCrystal \
	-I$(DIR)/libraries/ADXL345  \
        -I$(DIR)/libraries/TFTtouch
	
LIBS=../libarduino.a
TARGET=../output/test

OBJS =  paint Motor_shield xbee_relay xbee_shield relay_shield nfc_test  drawCircle  test sim900 smg \
        io_test adc_test pwm_test spi_test adxl345_test serial_test liquidcrystal_i2c liquidcrystal_spi

all: 
	@mkdir -p $(TARGET)
	for i in $(OBJS); do echo "$(CXX) $(INCS) $$i.c -o $(TARGET)/$$i $(LIBS)"; done
	@for i in $(OBJS); do $(CXX) $(INCS) $$i.c -o $(TARGET)/$$i $(LIBS); done

clean:
	@for i in $(OBJS); do rm -f $(TARGET)/$$i; done
