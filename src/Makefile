MSP430PATH = /usr/local/msp430
MSP430MCU = msp430g2553
GCCVERSION = $(shell ls $(MSP430PATH)/lib/gcc/msp430-elf | head -n1)

MDLPATH = ../../msp430/src

CC = $(MSP430PATH)/bin/msp430-elf-gcc
CFLAGS = -I $(MDLPATH) -Os -nostdlib -std=c++17 -ffunction-sections -fdata-sections -mmcu=$(MSP430MCU)

CXX = $(MSP430PATH)/bin/msp430-elf-g++
CXXFLAGS = $(CFLAGS)

AR = $(MSP430PATH)/bin/msp430-elf-ar

objects = $(addsuffix .o, $(basename $(shell ls *.cpp)))

libmdlrfm69hcw.a: $(objects)
	$(AR) rvs $@ $^

$(objects):

.PHONY: clean

clean:
	$(RM) *.o libmdlmsp430.a
