#ifndef RFM69_REGISTERS_H
#define RFM69_REGISTERS_H

/** RFM69 configuration constants.
 * It is important that the values be defined in this header to save space in resulting binary.
 */

namespace mardev
{
    namespace rfm69
    {
        namespace registers
        {
            const uint8_t FIFO          = 0x00; // FIFO R/W access
            const uint8_t OpMode        = 0x01; // Operating modes
            const uint8_t DataModul     = 0x02; // Data operation mode and modulation sett
            const uint8_t BitRateMSB    = 0x03; // Bitrate setting, MSB
            const uint8_t BitRateLSB    = 0x04; // Bitrate setting, LSB
            const uint8_t FdevMSB       = 0x05; // Frequency deviation, MSB
            const uint8_t FdevLSB       = 0x06; // Frequency deviation, LSB
            const uint8_t FRFMSB        = 0x07; // Carrier frequency, MSB
            const uint8_t FRFMID        = 0x08; // Carrier frequency, MidSB
            const uint8_t FRFLSB        = 0x09; // Carrier frequency, LSB
            const uint8_t Osc1          = 0x0A; // Oscillator settings
            const uint8_t AFCCTRL       = 0x0B; // AFC control
            const uint8_t Reserved0C    = 0x0C; // Reserved
            const uint8_t Listen1       = 0x0D; // Listen mode settings
            const uint8_t Listen2       = 0x0E; // Listen mode idle duration
            const uint8_t Listen3       = 0x0F; // Listen mode Rx duration
            const uint8_t Version       = 0x10; // ???
            const uint8_t PALevel       = 0x11; // PA selection and output power control
            const uint8_t PARamp        = 0x12; // PA ramp time control (FSK mode)
            const uint8_t OCP           = 0x13; // Over-current protection control
            const uint8_t Reserved14    = 0x14; // Reserved
            const uint8_t Reserved15    = 0x15; // Reserved
            const uint8_t Reserved16    = 0x16; // Reserved
            const uint8_t Reserved17    = 0x17; // Reserved
            const uint8_t LNA           = 0x18; // LNA settings
            const uint8_t RxBW          = 0x19; // Channel filter bandwidth control
            const uint8_t AFCBW         = 0x1A; // Channel filter bandwidth control (AFC r
            const uint8_t OOKPeak       = 0x1B; // OOK demodulator selection and control (
            const uint8_t OOKAvg        = 0x1C; // Average threshold control of OOK demodu
            const uint8_t OOKFix        = 0x1D; // Fixed threshold control of OOK demodula
            const uint8_t AFCFEI        = 0x1E; // AFC and FEI control, status
            const uint8_t AFCMSB        = 0x1F; // Frequency correction for AFC, MSB
            const uint8_t AFCLSB        = 0x20; // Frequency correction for AFC, LSB
            const uint8_t FEIMSB        = 0x21; // Calculated frequency error, MSB
            const uint8_t FEILSB        = 0x22; // Calculated frequency error, LSB
            const uint8_t RSSIConfig    = 0x23; // RSSI-related settings
            const uint8_t RSSIValue     = 0x24; // RSSI value in dBm
            const uint8_t DIOMapping1   = 0x25; // Pin mapping for DIO0-DIO3
            const uint8_t DIOMapping2   = 0x26; // Pin mapping for DIO4, DIO5 and ClkOut f
            const uint8_t IRQFlags1     = 0x27; // Status register: PLL lock, timeout, RSS
            const uint8_t IRQFlags2     = 0x28; // Status register: FIFO handling flags
            const uint8_t RSSIThresh    = 0x29; // RSSI threshold control
            const uint8_t RxTimeout1    = 0x2A; // Timeout duration between Rx request and
            const uint8_t RxTimeout2    = 0x2B; // Timeout duration between RSSI detection
            const uint8_t PreambleMSB   = 0x2C; // Preamble length, MSB
            const uint8_t PreambleLSB   = 0x2D; // Preamble length, LSB
            const uint8_t SyncConfig    = 0x2E; // Sync word recognition control
            const uint8_t SyncValue1    = 0x2F; // Sync word bytes 1 - 8
            const uint8_t SyncValue2    = 0x30;
            const uint8_t SyncValue3    = 0x31;
            const uint8_t SyncValue4    = 0x32;
            const uint8_t SyncValue5    = 0x33;
            const uint8_t SyncValue6    = 0x34;
            const uint8_t SyncValue7    = 0x35;
            const uint8_t SyncValue8    = 0x36;
            const uint8_t PacketConfig1 = 0x37; // Packet mode settings, part 1
            const uint8_t PayloadLength = 0x38; // Payload length settings
            const uint8_t NodeAdrs      = 0x39; // Node address
            const uint8_t BroadCastAdrs = 0x3A; // Broadcast address
            const uint8_t AutoModes     = 0x3B; // Auto modes settings
            const uint8_t FIFOThresh    = 0x3C; // FIFO threshold, Tx start condition
            const uint8_t PacketConfig2 = 0x3D; // Packet mode settings, part 2
            const uint8_t AESKey01      = 0x3E; // AES cipher key (128-bit)
            const uint8_t AESKey02      = 0x3F;
            const uint8_t AESKey03      = 0x40;
            const uint8_t AESKey04      = 0x41;
            const uint8_t AESKey05      = 0x42;
            const uint8_t AESKey06      = 0x43;
            const uint8_t AESKey07      = 0x44;
            const uint8_t AESKey08      = 0x45;
            const uint8_t AESKey09      = 0x46;
            const uint8_t AESKey10      = 0x47;
            const uint8_t AESKey11      = 0x48;
            const uint8_t AESKey12      = 0x49;
            const uint8_t AESKey13      = 0x4A;
            const uint8_t AESKey14      = 0x4B;
            const uint8_t AESKey15      = 0x4C;
            const uint8_t AESKey16      = 0x4D;
            const uint8_t Temp1         = 0x4E; // Temperature sensor control
            const uint8_t Temp2         = 0x4F; // Temperature sensor readout
            const uint8_t TestLNA       = 0x58; // Sensitivity boost
            const uint8_t TestPA1       = 0x5A; // High-power PA settings, part 1
            const uint8_t TestPA2       = 0x5C; // High-power PA settings, part 2
            const uint8_t TestDAGC      = 0x6F; // Fading margin improvement
            const uint8_t TestAFC       = 0x71; // AFC offset for low modulation index AFC

            namespace mask
            {
                // OpMode
                const uint8_t Mode       = 28; // 00011100
                const uint8_t ListenAbrt = 32;
                const uint8_t Listen     = 64;
                const uint8_t SeqOff     = 128;

                // Listen1
                const uint8_t ListenEnd       = 6;        // 00000110
                const uint8_t ListenCriteria  = 8;
                const uint8_t ListenResolRx   = 32 + 16;  // 00110000
                const uint8_t ListenResolIdle = 128 + 64;

                // IRQFlags1
                const uint8_t SyncAddressMatch = 1;
                const uint8_t AutoMode         = 2;
                const uint8_t Timeout          = 4;
                const uint8_t RSSI             = 8;
                const uint8_t PLLLock          = 16;
                const uint8_t TxReady          = 32;
                const uint8_t RxReady          = 64;
                const uint8_t ModeReady        = 128;

                // IRQFlags2
                const uint8_t CRCOK            = 2;
                const uint8_t PayloadReady     = 4;
                const uint8_t PacketSent       = 8;
                const uint8_t FIFOOverrun      = 16;
                const uint8_t FIFOLevel        = 32;
                const uint8_t FIFONotEmpty     = 64;
                const uint8_t FIFOFull         = 128;

                // SyncConfig
                const uint8_t SyncTol           = 7;  // 00000111
                const uint8_t SyncSize          = 56; // 00111000
                const uint8_t FIFOFillCondition = 64;
                const uint8_t SyncOn            = 128;

                // PacketConfig1
                const uint8_t AddressFiltering = 6;  // 00000110
                const uint8_t CRCAutoClearOff  = 8;
                const uint8_t CRCOn            = 16;
                const uint8_t DCFree           = 96; // 01100000
                const uint8_t PacketFormat     = 128;

                // FIFOThresh
                const uint8_t FIFOThreshold    = 127; // 01111111
                const uint8_t TxStartCondition = 128;

                // PacketConfig2
                const uint8_t AESOn              = 1;
                const uint8_t AutoRxRestartOn    = 2;
                const uint8_t InterPacketRxDelay = 240; // 11110000

                // Temp1
                const uint8_t TempMeasRunning = 4;
                const uint8_t TempMeasStart   = 8;
            }
        }
    }
}

#endif
