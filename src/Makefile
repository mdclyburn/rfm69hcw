MSP430PATH = /usr/local/msp430
MSP430MCU = msp430g2553

MDLPATH = ../../msp430/src

# RFM69 Configuration #########
RFM_ENCRYPTIONKEY = { 0x4D, 0x22, 0x28, 0xAE, \
                      0x54, 0x90, 0x80, 0x20, \
                      0x00, 0x02, 0xFE, 0x10, \
                      0x09, 0x00, 0x59, 0x11  }

RFM_SYNCWORD = { 0xAC, 0xDC, 0xFF, 0x06, 0x05, 0x04, 0x03 }

RFM_CONFIG = \
	-DRFM_ENCRYPTIONKEY="${RFM_ENCRYPTIONKEY}" \
	-DRFM_SYNCWORD="${RFM_SYNCWORD}"

GCCVERSION = $(shell ls $(MSP430PATH)/lib/gcc/msp430-elf | head -n1)
CC = $(MSP430PATH)/bin/msp430-elf-gcc
CFLAGS = -I $(MDLPATH) -Os -std=c++17 -nostdlib -ffreestanding -ffunction-sections -fdata-sections -fno-asynchronous-unwind-tables -fno-unwind-tables -fomit-frame-pointer -fno-exceptions -mmcu=$(MSP430MCU) $(RFM_CONFIG)

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
