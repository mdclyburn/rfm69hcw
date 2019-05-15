#include <SPI.h>

#include "rfm69.h"
#include "rfm69_registers.h"

using namespace mardev::rfm69;

namespace mardev
{
    namespace rfm69
    {
        void initialize()
        {
            SPI.begin();
            SPI.setDataMode(SPI_MODE0);
            SPI.setBitOrder(MSBFIRST);

            // Set up digital pins.
            pinMode(RFM_CONFIG_PINSS, OUTPUT);

            // Set up reset.
            pinMode(RFM_CONFIG_PINRESET, OUTPUT);
            reset();
            delay(100);

            // Recommended Settings
            __rfm_register_write(registers::LNA, 0x88);
            __rfm_register_write(registers::RxBW, 0x55);
            __rfm_register_write(registers::AFCBW, 0x8B);
            __rfm_register_write(registers::RSSIThresh, 0xE4);
            __rfm_register_write(registers::TestDAGC, 0x30);

            // Node addressing.
            __rfm_register_write(registers::NodeAdrs, RFM_CONFIG_NODEADDRESS);
            __rfm_register_modify(registers::PacketConfig1,
                                  registers::mask::AddressFiltering,
                                  2);

            // CRC
            __rfm_register_modify(registers::PacketConfig1,
                                  registers::mask::CRCOn,
                                  1); // Enable CRCs.

            // Clear the FIFO and restart new packet reception when CRC check fails.
            // PayloadReady is not issued.
            __rfm_register_modify(registers::PacketConfig1,
                                  registers::mask::CRCAutoClearOff,
                                  0);

            // Variable-length packet mode
            __rfm_register_modify(registers::PacketConfig1,
                                  registers::mask::PacketFormat,
                                  1);
            __rfm_register_write(registers::PayloadLength, 255);

            // Set data rate.
            __rfm_register_write(registers::BitRateMSB, RFM_CONFIG_BITRATE >> 8);
            __rfm_register_write(registers::BitRateLSB, RFM_CONFIG_BITRATE & 0x00FF);

            // Sync word
            const uint8_t sync_word[] = { 0xAC, 0xDC, 0xFF, 0x06, 0x05, 0x04, 0x03 };
            __rfm_register_modify(registers::SyncConfig,
                                  registers::mask::SyncOn,
                                  1); // Enable use of the sync word.
            __rfm_register_modify(registers::SyncConfig,
                                  registers::mask::SyncSize,
                                  6); // Size of the sync word is taken to be 7 = 6 + 1.
            __rfm_register_burst_write(registers::SyncValue1,
                                       sync_word,
                                       7); // Write the 56-bit sync word.

            #ifdef RFM_FEATURE_ENCRYPTION
            // Write the AES key and enable encryption if enabled.
            const uint8_t aes_key[] = RFM_CONFIG_ENCRYPTIONKEY;

            __rfm_register_burst_write(registers::AESKey01, aes_key, 16);
            __rfm_register_modify(registers::PacketConfig2, 1, 1);
            #else
            // Ensure encryption is disabled.
            __rfm_register_modify(registers::PacketConfig2, 1, 0);
            #endif

            // Clear the FIFO.
            __rfm_register_modify(registers::IRQFlags2,
                                  registers::mask::FIFOOverrun,
                                  1);

            // Transmit as soon as a byte is available.
            __rfm_register_modify(registers::FIFOThresh,
                                  registers::mask::TxStartCondition,
                                  1);

            // Preamble size
            __rfm_register_write(registers::PreambleMSB, 0);
            __rfm_register_write(registers::PreambleLSB, 0x40);

            return;
        }

        void reset()
        {
            digitalWrite(RFM_CONFIG_PINRESET, HIGH);
            delayMicroseconds(100);
            digitalWrite(RFM_CONFIG_PINRESET, LOW);
            delay(5);

            return;
        }

        void select()
        {
            digitalWrite(RFM_CONFIG_PINSS, LOW);
            return;
        }

        void deselect()
        {
            digitalWrite(RFM_CONFIG_PINSS, HIGH);
            return;
        }

        uint8_t __rfm_register_read(const uint8_t rfm_register)
        {
            const uint8_t register_access =
                rfm_register & 127;

            #ifdef RFM_DEBUG
            Serial.print("RFM: read register ");
            Serial.println(rfm_register);
            #endif

            select();
            SPI.transfer(register_access);
            const uint8_t value = SPI.transfer(0);
            deselect();

            #ifdef RFM_DEBUG
            Serial.print("RFM: register read returned: ");
            Serial.println(value);
            #endif

            return value;
        }

        uint8_t __rfm_register_write(const uint8_t rfm_register,
                                     const uint8_t value)
        {
            const uint8_t register_access =
                rfm_register | 128;

            #ifdef RFM_DEBUG
            Serial.print("RFM: write register ");
            Serial.println(rfm_register);
            #endif

            select();
            SPI.transfer(register_access);
            const uint8_t old_value = SPI.transfer(value);
            deselect();

            #ifdef RFM_DEBUG
            Serial.print("RFM: register write returned old value: ");
            Serial.println(old_value);
            #endif

            return old_value;
        }

        void __rfm_register_burst_write(const uint8_t begin_register,
                                        const uint8_t* const values,
                                        const uint8_t length)
        {
            #ifdef RFM_DEBUG
            Serial.print("Burst register write from ");
            Serial.print(begin_register);
            Serial.print(" length ");
            Serial.println(length);
            #endif

            select();
            SPI.transfer(begin_register | 128);
            for(uint8_t offset = 0; offset < length; offset++)
                SPI.transfer(values[offset]);
            deselect();

            return;
        }

        void __rfm_register_modify(const uint8_t rfm_register,
                                   const uint8_t mask,
                                   const uint8_t value)
        {
            #ifdef RFM_DEBUG
            Serial.print("Modify register by inserting ");
            Serial.print(value);
            Serial.print(" into ");
            Serial.println(rfm_register);
            #endif

            uint8_t offset = 0;
            while(!((mask >> offset) & 1))
                offset++;

            __rfm_register_write(
                rfm_register,
                (__rfm_register_read(rfm_register) & ~mask) | (value << offset));

            return;
        }

        uint8_t mode()
        {
            return (__rfm_register_read(registers::OpMode) & registers::mask::Mode) >> 2;
        }

        void mode(const uint8_t mode)
        {
            __rfm_register_modify(
                registers::OpMode,
                registers::mask::Mode,
                mode);

            while(!(__rfm_register_read(registers::IRQFlags1) & registers::mask::ModeReady))
                delayMicroseconds(100);

            return;
        }

        void rfm_fifo_read(uint8_t* const buffer)
        {
            uint8_t i = 0;
            while((__rfm_register_read(registers::IRQFlags2) & 64) && i < RFM_CONFIG_PACKETSIZE)
            {
                Serial.print("Saving byte to "); Serial.println(i);
                buffer[i++] = __rfm_register_read(registers::FIFO);
                Serial.print("Value: "); Serial.println(buffer[i-1], HEX);
            }

            return;
        }

        uint8_t rfm_fifo_write(const uint8_t* const buffer, const uint8_t size)
        {
            if (size > RFM_CONFIG_PACKETSIZE)
                return 1;

            // FIFO full
            if (__rfm_register_read(registers::IRQFlags2) & 128)
                return 2;

            // #if RFM_CONFIG_PACKETSIZE <= 66
            // __rfm_register_burst_write(registers::FIFO, buffer, size);
            // #else
            uint8_t i = 0;
            while(i < size && !(__rfm_register_read(registers::IRQFlags2) & 128))
                __rfm_register_write(registers::FIFO, buffer[i++]);
            // #endif

            return 0;
        }

        #ifdef RFM_FEATURE_LISTEN

        void __rfm_listen_mode()
        {
            __rfm_operating_mode(RFM_OPMODE_STANDBY);
            __rfm_register_modify(registers::OpMode,
                                  registers::mask::Listen,
                                  1);

            return;
        }

        void __rfm_abort_listen_mode(const uint8_t mode)
        {
            const uint8_t value =
                (__rfm_register_read(registers::OpMode) & 128) | 32 | (mode << 2);

            __rfm_register_write(registers::OpMode, value);
            __rfm_register_write(registers::OpMode, value & ~registers::mask::ListenAbrt);

            return;
        }

        #endif

        // ===== Temperature ===========================================================
        // =============================================================================
        #ifdef RFM_FEATURE_TEMPERATURE

        uint8_t rfm_temperature(const int8_t offset)
        {
            /* RFM_REG_TEMP1
             * bit 7 - 4: unused
             * bit 3: triggers temperature measurement when set to 1
             * bit 2: will be set to 1 while measurement is running
             * bit 1, 0: unused
             */

            // The temperature sensor can only be used in standby or frequency
            // synthesizer modes but not in receive mode.
            const uint8_t current_mode = mode();
            if(current_mode == RFM_OPMODE_SLEEP || current_mode == RFM_OPMODE_RECEIVE)
                return 0;

            // According to the datasheet, reading the temperature takes < 100us.
            // Impose a wait here instead of pestering the radio immediately.

            __rfm_register_write(registers::Temp1, registers::mask::TempMeasStart);

            #ifndef RFM_CONFIG_COMPACT
            delayMicroseconds(100 - 10);
            #endif

            while(__rfm_register_read(registers::Temp1) & registers::mask::TempMeasRunning);

            return ~__rfm_register_read(registers::Temp2) - 94 + offset;
        }

    }
}
#endif
