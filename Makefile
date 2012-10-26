TARGET = readsd

# Place your Arduino libs here! It's okay to not define this.
ARDUINO_LIBS = SPI Adafruit_GFX Adafruit_ST7735 \
	Adafruit_SD Adafruit_SD/utility SD/utility

# Either set this here or type `make upload BOARD_TAG=uno`
BOARD_TAG = mega2560 

ARDUINO_UA_DIR = C:/Users/Adam/arduino-ua
include $(ARDUINO_UA_DIR)/mkfiles/ArduinoUA.mk

# This is magic that I use to define MEGA or UNO in my C/C++ files.
# Remember to `make clean` before `make upload`ing on a different type
# of board.
BOARD_DEFINE := $(shell echo $(BOARD_TAG) | tr 'a-z' 'A-Z' | tr -d [0-9])
DEFINITIONS = $(BOARD_DEFINE) # You can also define DEBUG and stuff like that here
DEFINES := ${DEFINITIONS:%=-D%}

# Define your compiler flags. Remember to `+=` the rule.
#CFLAGS += -Wall -Werror -std=c99
#CXXFLAGS += -Wall -Werror
CPPFLAGS += $(DEFINES) 

# override the default optimization levels here
# CPP_OPTIMIZE = -O0
# C_OPTIMIZE = -O0
# LD_OPTIMIZE = -O0
