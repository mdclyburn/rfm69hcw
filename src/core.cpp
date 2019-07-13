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
    void initialize()
    {
        spi::initialize(
            usci::Module::B0,
            usci::UCMODE::SPI3,
            spi::UCSSELx::SMCLK,
            spi::UCCKPH::P0,
            spi::UCCKPL::P1,
            spi::UCMSB::MSBFirst);

        // Set up digital pins.
        dio::set_pin_mode(RFM_CONFIG_PINSS, dio::IO::Output);

        // Set up reset.
        dio::set_pin_mode(RFM_CONFIG_PINRESET, dio::IO::Output);
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
        write(registers::PreambleMSB, 0x00);
        write(registers::PreambleLSB, 0x40);

        return;
    }

    void reset()
    {
        dio::write(RFM_CONFIG_PINRESET, dio::Logic::High);
        timer::delay(1000);
        dio::write(RFM_CONFIG_PINRESET, dio::Logic::Low);
        timer::delay(500);

        return;
    }

    void select()
    {
        dio::write(RFM_CONFIG_PINSS, dio::Logic::Low);
        return;
    }

    void deselect()
    {
        dio::write(RFM_CONFIG_PINSS, dio::Logic::High);
        return;
    }

    uint8_t read(const uint8_t address)
    {
        const uint8_t register_access = address & 127;

        select();
        spi::write(usci::Module::B0, register_access);
        const uint8_t value = spi::write(usci::Module::B0, 0);
        deselect();

        return value;
    }

    uint8_t write(const uint8_t address,
                  const uint8_t value)
    {
        const uint8_t register_access = address | 128;

        select();
        spi::write(usci::Module::B0, register_access);
        const uint8_t old_value = spi::write(usci::Module::B0, value);
        deselect();

        return old_value;
    }

    void write(const uint8_t begin_address,
               const uint8_t* const values,
               const uint8_t length)
    {
        select();
        spi::write(usci::Module::B0, begin_address | 128);
        for(uint8_t offset = 0; offset < length; offset++)
            spi::write(usci::Module::B0, values[offset]);
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
