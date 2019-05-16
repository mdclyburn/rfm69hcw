#include <SPI.h>

#include "rfm69.h"
#include "rfm69_registers.h"

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
            write(registers::LNA, 0x88);
            write(registers::RxBW, 0x55);
            write(registers::AFCBW, 0x8B);
            write(registers::RSSIThresh, 0xE4);
            write(registers::TestDAGC, 0x30);

            // Node addressing.
            // Listen for both unique and broadcast addresses.
            write(registers::NodeAdrs, RFM_CONFIG_NODEADDRESS);
            modify(registers::PacketConfig1, registers::mask::AddressFiltering, 2);

            // CRC
            // Enable CRCs.
            modify(registers::PacketConfig1, registers::mask::CRCOn, 1);

            // CRC auto-clear preference.
            // Clear the FIFO and do not set PayloadReady.
            modify(registers::PacketConfig1, registers::mask::CRCAutoClearOff, 0);

            // Variable-length packet mode.
            modify(registers::PacketConfig1, registers::mask::PacketFormat, 1);
            write(registers::PayloadLength, 255);

            // Set data rate.
            write(registers::BitRateMSB, RFM_CONFIG_BITRATE >> 8);
            write(registers::BitRateLSB, RFM_CONFIG_BITRATE & 0x00FF);

            // Sync word
            // TODO: extract the sync word back out into a configuration.
            const uint8_t sync_word[] = RFM_CONFIG_SYNCWORD;

            // Enable use of the sync word.
            // Size of the sync word is taken to be 7 = 6 + 1.
            // Write the 56-bit sync word.
            modify(registers::SyncConfig, registers::mask::SyncOn, 1);
            modify(registers::SyncConfig, registers::mask::SyncSize, RFM_CONFIG_SYNCWORDLENGTH);
            write(registers::SyncValue1, sync_word, RFM_CONFIG_SYNCWORDLENGTH+1);

            // AES encryption
            const uint8_t aes_key[] = RFM_CONFIG_ENCRYPTIONKEY;

            write(registers::AESKey01, aes_key, 16);
            modify(registers::PacketConfig2, registers::mask::AESOn, 1);

            // Clear the FIFO.
            modify(registers::IRQFlags2, registers::mask::FIFOOverrun, 1);

            // Transmit as soon as a byte is available.
            modify(registers::FIFOThresh, registers::mask::TxStartCondition, 1);

            // Preamble size
            write(registers::PreambleMSB, 0);
            write(registers::PreambleLSB, 0x40);

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

        uint8_t read(const uint8_t address)
        {
            const uint8_t register_access = address & 127;

            select();
            SPI.transfer(register_access);
            const uint8_t value = SPI.transfer(0);
            deselect();

            return value;
        }

        uint8_t write(const uint8_t address,
                      const uint8_t value)
        {
            const uint8_t register_access = address | 128;

            select();
            SPI.transfer(register_access);
            const uint8_t old_value = SPI.transfer(value);
            deselect();

            return old_value;
        }

        void write(const uint8_t begin_address,
                   const uint8_t* const values,
                   const uint8_t length)
        {
            select();
            SPI.transfer(begin_address | 128);
            for(uint8_t offset = 0; offset < length; offset++)
                SPI.transfer(values[offset]);
            deselect();

            return;
        }

        void modify(const uint8_t address,
                    const uint8_t mask,
                    const uint8_t value)
        {
            uint8_t offset = 0;
            while(!((mask >> offset) & 1))
                offset++;

            write(address,
                  (read(address) & ~mask) | (value << offset));

            return;
        }

        void read_fifo(uint8_t* const buffer)
        {
            uint8_t i = 0;
            while(!fifo_is_empty())
            {
                buffer[i++] = read(registers::FIFO);
            }

            return;
        }

        uint8_t write_fifo(const uint8_t* const buffer,
                           const uint8_t size)
        {
            // Limit for the library is at 66 bytes (at least for now).
            if (size > 66)
                return 1;

            // FIFO full
            if (read(registers::IRQFlags2) & 128)
                return 2;

            // TODO: turn this into a single burst-write.
            uint8_t i = 0;
            while(i < size && !(read(registers::IRQFlags2) & 128))
                write(registers::FIFO, buffer[i++]);

            return 0;
        }

        uint8_t mode()
        {
            return (read(registers::OpMode) & registers::mask::Mode) >> 2;
        }

        void mode(const uint8_t mode)
        {
            modify(
                registers::OpMode,
                registers::mask::Mode,
                mode);

            while(!(read(registers::IRQFlags1) & registers::mask::ModeReady))
                delayMicroseconds(100);

            return;
        }

        #ifdef RFM_FEATURE_LISTEN

        void start_listen_mode()
        {
            __rfm_operating_mode(RFM_OPMODE_STANDBY);
            modify(registers::OpMode,
                   registers::mask::Listen,
                   1);

            return;
        }

        void abort_listen_mode(const uint8_t mode)
        {
            const uint8_t value =
                (read(registers::OpMode) & 128) | 32 | (mode << 2);

            write(registers::OpMode, value);
            write(registers::OpMode, value & ~registers::mask::ListenAbrt);

            return;
        }

        #endif

        // ===== Temperature ===========================================================
        // =============================================================================
        #ifdef RFM_FEATURE_TEMPERATURE

        uint8_t temperature(const int8_t offset)
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

            write(registers::Temp1, registers::mask::TempMeasStart);

            #ifndef RFM_CONFIG_COMPACT
            delayMicroseconds(100 - 10);
            #endif

            while(read(registers::Temp1) & registers::mask::TempMeasRunning);

            return ~read(registers::Temp2) - 94 + offset;
        }
    }
}

#endif
