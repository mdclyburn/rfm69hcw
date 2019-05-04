#ifndef RFM69_REGISTERS_H
#define RFM69_REGISTERS_H

#define RFM_REG_FIFO          0x00 // FIFO R/W access
#define RFM_REG_OPMODE        0x01 // Operating modes
#define RFM_REG_DATAMODUL     0x02 // Data operation mode and modulation setting
#define RFM_REG_BITRATEMSB    0x03 // Bitrate setting, MSB
#define RFM_REG_BITRATELSB    0x04 // Bitrate setting, LSB
#define RFM_REG_FDEVMSB       0x05 // Frequency deviation, MSB
#define RFM_REG_FDEVLSB       0x06 // Frequency deviation, LSB
#define RFM_REG_FRFMSB        0x07 // Carrier frequency, MSB
#define RFM_REG_FRFMID        0x08 // Carrier frequency, MidSB
#define RFM_REG_FRFLSB        0x09 // Carrier frequency, LSB
#define RFM_REG_OSC1          0x0A // Oscillator settings
#define RFM_REG_AFCCTRL       0x0B // AFC control
#define RFM_REG_RESERVED0C    0x0C // Reserved
#define RFM_REG_LISTEN1       0x0D // Listen mode settings
#define RFM_REG_LISTEN2       0x0E // Listen mode idle duration
#define RFM_REG_LISTEN3       0x0F // Listen mode Rx duration
#define RFM_REG_VERSION       0x10 // ???
#define RFM_REG_PALEVEL       0x11 // PA selection and output power control
#define RFM_REG_PARAMP        0x12 // PA ramp time control (FSK mode)
#define RFM_REG_OCP           0x13 // Over-current protection control
#define RFM_REG_RESERVED14    0x14 // Reserved
#define RFM_REG_RESERVED15    0x15 // Reserved
#define RFM_REG_RESERVED16    0x16 // Reserved
#define RFM_REG_RESERVED17    0x17 // Reserved
#define RFM_REG_LNA           0x18 // LNA settings
#define RFM_REG_RXBW          0x19 // Channel filter bandwidth control
#define RFM_REG_AFCBW         0x1A // Channel filter bandwidth control (AFC routine)
#define RFM_REG_OOKPEAK       0x1B // OOK demodulator selection and control (peak mode)
#define RFM_REG_OOKAVG        0x1C // Average threshold control of OOK demodulator
#define RFM_REG_OOKFIX        0x1D // Fixed threshold control of OOK demodulator
#define RFM_REG_AFCFEI        0x1E // AFC and FEI control, status
#define RFM_REG_AFCMSB        0x1F // Frequency correction for AFC, MSB
#define RFM_REG_AFCLSB        0x20 // Frequency correction for AFC, LSB
#define RFM_REG_FEIMSB        0x21 // Calculated frequency error, MSB
#define RFM_REG_FEILSB        0x22 // Calculated frequency error, LSB
#define RFM_REG_RSSICONFIG    0x23 // RSSI-related settings
#define RFM_REG_RSSIVALUE     0x24 // RSSI value in dBm
#define RFM_REG_DIOMAPPING1   0x25 // Pin mapping for DIO0-DIO3
#define RFM_REG_DIOMAPPING2   0x26 // Pin mapping for DIO4, DIO5 and ClkOut frequency
#define RFM_REG_IRQFLAGS1     0x27 // Status register: PLL lock, timeout, RSSI detection
#define RFM_REG_IRQFLAGS2     0x28 // Status register: FIFO handling flags
#define RFM_REG_RSSITHRESH    0x29 // RSSI threshold control
#define RFM_REG_RXTIMEOUT1    0x2A // Timeout duration between Rx request and RSSI detection
#define RFM_REG_RXTIMEOUT2    0x2B // Timeout duration between RSSI detection and 'PayloadReady'
#define RFM_REG_PREAMBLEMSB   0x2C // Preamble length, MSB
#define RFM_REG_PREAMBLELSB   0x2D // Preamble length, LSB
#define RFM_REG_SYNCCONFIG    0x2E // Sync word recognition control
#define RFM_REG_SYNCVALUE1    0x2F // Sync word bytes 1 - 8
#define RFM_REG_SYNCVALUE2    0x30
#define RFM_REG_SYNCVALUE3    0x31
#define RFM_REG_SYNCVALUE4    0x32
#define RFM_REG_SYNCVALUE5    0x33
#define RFM_REG_SYNCVALUE6    0x34
#define RFM_REG_SYNCVALUE7    0x35
#define RFM_REG_SYNCVALUE8    0x36
#define RFM_REG_PACKETCONFIG1 0x37 // Packet mode settings, part 1
#define RFM_REG_PAYLOADLENGTH 0x38 // Payload length settings
#define RFM_REG_NODEADRS      0x39 // Node address
#define RFM_REG_BROADCASTADRS 0x3A // Broadcast address
#define RFM_REG_AUTOMODES     0x3B // Auto modes settings
#define RFM_REG_FIFOTHRESH    0x3C // FIFO threshold, Tx start condition
#define RFM_REG_PACKETCONFIG2 0x3D // Packet mode settings, part 2
#define RFM_REG_AESKEY01      0x3E // AES cipher key (128-bit)
#define RFM_REG_AESKEY02      0x3F
#define RFM_REG_AESKEY03      0x40
#define RFM_REG_AESKEY04      0x41
#define RFM_REG_AESKEY05      0x42
#define RFM_REG_AESKEY06      0x43
#define RFM_REG_AESKEY07      0x44
#define RFM_REG_AESKEY08      0x45
#define RFM_REG_AESKEY09      0x46
#define RFM_REG_AESKEY10      0x47
#define RFM_REG_AESKEY11      0x48
#define RFM_REG_AESKEY12      0x49
#define RFM_REG_AESKEY13      0x4A
#define RFM_REG_AESKEY14      0x4B
#define RFM_REG_AESKEY15      0x4C
#define RFM_REG_AESKEY16      0x4D
#define RFM_REG_TEMP1         0x4E // Temperature sensor control
#define RFM_REG_TEMP2         0x4F // Temperature sensor readout
#define RFM_REG_TESTLNA       0x58 // Sensitivity boost
#define RFM_REG_TESTPA1       0x5A // High-power PA settings, part 1
#define RFM_REG_TESTPA2       0x5C // High-power PA settings, part 2
#define RFM_REG_TESTDAGC      0x6F // Fading margin improvement
#define RFM_REG_TESTAFC       0x71 // AFC offset for low modulation index AFC

// RegOpMode
#define RFM_REG_MASK_OPMODE_MODE       28   // 00011100
#define RFM_REG_MASK_OPMODE_LISTENABRT 32
#define RFM_REG_MASK_OPMODE_LISTEN     64
#define RFM_REG_MASK_OPMODE_SEQOFF     128

// RegListen1
#define RFM_REG_MASK_LISTEN1_LISTENEND 6    // 00000110

// RegDioMapping1
#define RFM_REG_MASK_DIOMAPPING1_DIO0MAPPING 192 // 11000000

// RegIrqFlags1
#define RFM_REG_MASK_IRQFLAGS1_SAMATCH   1
#define RFM_REG_MASK_IRQFLAGS1_AUTO      2
#define RFM_REG_MASK_IRQFLAGS1_TIMEOUT   4
#define RFM_REG_MASK_IRQFLAGS1_RSSI      8
#define RFM_REG_MASK_IRQFLAGS1_PLLLOCK   16
#define RFM_REG_MASK_IRQFLAGS1_TXREADY   32
#define RFM_REG_MASK_IRQFLAGS1_RXREADY   64
#define RFM_REG_MASK_IRQFLAGS1_MODEREADY 128

// RegIrqFlags2
#define RFM_REG_MASK_IRQFLAGS2_CRCOK        2
#define RFM_REG_MASK_IRQFLAGS2_PAYLOADREADY 4
#define RFM_REG_MASK_IRQFLAGS2_PACKETSENT   8
#define RFM_REG_MASK_IRQFLAGS2_FIFOOVERRUN  16
#define RFM_REG_MASK_IRQFLAGS2_FIFOLEVEL    32
#define RFM_REG_MASK_IRQFLAGS2_FIFONOTEMPTY 64
#define RFM_REG_MASK_IRQFLAGS2_FIFOFULL     128

// RegSyncConfig
#define RFM_REG_MASK_SYNCCONFIG_SYNCTOL           7    // 00000111
#define RFM_REG_MASK_SYNCCONFIG_SYNCSIZE          56   // 00111000
#define RFM_REG_MASK_SYNCCONFIG_FIFOFILLCONDITION 64
#define RFM_REG_MASK_SYNCCONFIG_SYNCON            128

// RegPacketConfig
#define RFM_REG_MASK_PACKETCONFIG1_ADDRESSFILTERING 6    // 00000110
#define RFM_REG_MASK_PACKETCONFIG1_CRCAUTOCLEAROFF  8
#define RFM_REG_MASK_PACKETCONFIG1_CRCON            16
#define RFM_REG_MASK_PACKETCONFIG1_DCFREE           96   // 01100000
#define RFM_REG_MASK_PACKETCONFIG1_PACKETFORMAT     128

// RegFifoThresh
#define RFM_REG_MASK_FIFOTHRESH_FIFOTHRESHOLD      127 // 01111111
#define RFM_REG_MASK_FIFOTHRESH_TXSTARTCONDITION   128

// RegPacketConfig2
#define RFM_REG_MASK_PACKETCONFIG2_AESON 1

#endif
