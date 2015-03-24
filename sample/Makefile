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
	-I$(DIR)/libraries/PN532_SPI 

LIBS=../libarduino.a
TARGET=../output/test

OBJS = io_test adc_test pwm_test spi_test adxl345_test serial_test liquidcrystal_i2c liquidcrystal_spi interrupt_test tone_test
OBJS += linker_led_test linker_potentiometer_test linker_tilt_test linker_light_sensor_test linker_button_test
OBJS += linker_touch_sensor_test linker_magnetic_sensor_test linker_temperature_sensor_test linker_joystick_test
OBJS += linker_rtc_test linker_sound_sensor_test linker_buzzer_test linker_hall_sensor_test linker_led_bar_test linker_relay_test
OBJS += pn532_readAllMemoryBlocks pn532readMifareMemory pn532readMifareTargetID pn532writeMifareMemory
OBJS += NRF24L01_RX 
all: 
	@mkdir -p $(TARGET)
	@for i in $(OBJS); do echo "$(CXX) $(INCS) $$i.c -o $(TARGET)/$$i $(LIBS)" && $(CXX) $(INCS) $$i.c -o $(TARGET)/$$i $(LIBS); done

clean:
	@for i in $(OBJS); do rm -f $(TARGET)/$$i; done
