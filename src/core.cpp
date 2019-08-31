#include <digital-io.h>
#include <timer.h>
#include <usci-spi.h>

#include "configuration.h"
#include "core.h"
#include "registers.h"

namespace usci  = mardev::msp430::usci;
namespace spi   = usci::spi;
namespace dio   = mardev::msp430::digital_io;
namespace timer = mardev::msp430::timer;

namespace mardev::rfm69
{
    /** 128-bit AES encryption key.
     *
     * The 128-bit (16 bytes) key used to encrypt each packet being sent.
     * This value must be the same for each radio expected to communicate.
     * Configure this value with the RFM_ENCRYPTIONKEY variable in the rfm-config.mk file.
     */
    const uint8_t AESKey[] = RFM_ENCRYPTIONKEY;

    /** Synchronization sequence value.
     *
     * Sequence of one to eight bytes used to synchronize messages.
     * Configure this value with the RFM_SYNCWORD variable in the rfm-config.mk file.
     * Update the RFM_SYNCWORDLENGTH value accordingly as well.
     * RFM_SYNCWORDLENGTH should be set to the number of byes provided for RFM_SYNCWORD minus 1.
     */
    const uint8_t SyncWord[] = RFM_SYNCWORD;

    void initialize()
    {
        spi::initialize(
            RFM_UCMODULE,
            spi::UCMODE::SPI3,
            spi::UCSSELx::SMCLK,
            spi::UCCKPH::P0,
            spi::UCCKPL::P0,
            spi::UCMSB::MSBFirst);

        // Set up digital pins.
        dio::set_pin_mode(RFM_PINSS, dio::IO::Output);
        dio::write(RFM_PINSS, dio::Logic::High);

        // Set up reset.
        dio::set_pin_mode(RFM_PINRESET, dio::IO::Output);
        reset();
        timer::delay(250);

        // Recommended Settings
        write(registers::LNA, 0x88);
        write(registers::RxBW, 0x55);
        write(registers::AFCBW, 0x8B);
        write(registers::RSSIThresh, 0xE4);
        write(registers::TestDAGC, 0x30);

        // Node addressing.
        // Listen for both unique and broadcast addresses.
        write(registers::NodeAdrs, RFM_NODEADDRESS);
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
        write(registers::BitRateMSB, RFM_BITRATE >> 8);
        write(registers::BitRateLSB, RFM_BITRATE & 0x00FF);

        // Sync word
        // TODO: extract the sync word back out into a configuration.
        // const uint8_t sync_word[] = RFM_SYNCWORD;

        // Enable use of the sync word.
        // Size of the sync word is taken to be 7 = 6 + 1.
        // Write the 56-bit sync word.
        modify(registers::SyncConfig, registers::mask::SyncOn, 1);
        modify(registers::SyncConfig, registers::mask::SyncSize, RFM_SYNCWORDLENGTH);
        write(registers::SyncValue1, SyncWord, RFM_SYNCWORDLENGTH+1);

        // AES encryption
        write(registers::AESKey01, AESKey, 16);
        modify(registers::PacketConfig2, registers::mask::AESOn, 1);

        // Clear the FIFO.
        modify(registers::IRQFlags2, registers::mask::FIFOOverrun, 1);

        // Transmit as soon as a byte is available.
        modify(registers::FIFOThresh, registers::mask::TxStartCondition, 1);

        // Preamble size
        write(registers::PreambleMSB, 0x00);
        write(registers::PreambleLSB, 0x40);

        return;
    }

    void reset()
    {
        dio::write(RFM_PINRESET, dio::Logic::High);
        timer::delay(1000);
        dio::write(RFM_PINRESET, dio::Logic::Low);
        timer::delay(500);

        return;
    }

    /** Begin an SPI transaction with the radio.
     */
    void select()
    {
        dio::write(RFM_PINSS, dio::Logic::Low);
        return;
    }

    /** End and SPI transaction with the radio.
     */
    void deselect()
    {
        dio::write(RFM_PINSS, dio::Logic::High);
        return;
    }

    uint8_t read(const uint8_t address)
    {
        const uint8_t register_access = address & 127;

        select();
        spi::write(RFM_UCMODULE, register_access);
        const uint8_t value = spi::write(RFM_UCMODULE, 0);
        deselect();

        return value;
    }

    void read(const uint8_t begin_address,
              uint8_t* const out,
              const uint8_t length)
    {
        select();
        spi::write(RFM_UCMODULE, begin_address & 127);
        for(uint8_t i = 0; i < length; i++)
            out[i] = spi::write(RFM_UCMODULE, 0);
        deselect();

        return;
    }

    uint8_t write(const uint8_t address,
                  const uint8_t value)
    {
        const uint8_t register_access = address | 128;

        select();
        spi::write(RFM_UCMODULE, register_access);
        const uint8_t old_value = spi::write(RFM_UCMODULE, value);
        deselect();

        return old_value;
    }

    void write(const uint8_t begin_address,
               const uint8_t* const values,
               const uint8_t length)
    {
        select();
        spi::write(RFM_UCMODULE, begin_address | 128);
        for(uint8_t offset = 0; offset < length; offset++)
            spi::write(RFM_UCMODULE, values[offset]);
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
}
