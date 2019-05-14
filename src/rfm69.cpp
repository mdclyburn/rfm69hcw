#include <SPI.h>

#include "rfm69.h"
#include "rfm69_registers.h"

void rfm_initialize()
{
    SPI.begin();
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);

    // Set up digital pins.
    pinMode(RFM_CONFIG_PINSS, OUTPUT);

    // Set up reset.
    pinMode(RFM_CONFIG_PINRESET, OUTPUT);
    rfm_reset();
    delay(100);

    // Recommended Settings
    __rfm_register_write(RFM_REG_LNA, 0x88);
    __rfm_register_write(RFM_REG_RXBW, 0x55);
    __rfm_register_write(RFM_REG_AFCBW, 0x8B);
    __rfm_register_write(RFM_REG_RSSITHRESH, 0xE4);
    __rfm_register_write(RFM_REG_TESTDAGC, 0x30);

    // Node addressing.
    __rfm_register_write(RFM_REG_NODEADRS, RFM_CONFIG_NODEADDRESS);
    __rfm_register_modify(RFM_REG_PACKETCONFIG1,
                          RFM_REG_MASK_PACKETCONFIG1_ADDRESSFILTERING,
                          2);

    // CRC
    __rfm_register_modify(RFM_REG_PACKETCONFIG1,
                          RFM_REG_MASK_PACKETCONFIG1_CRCON,
                          1); // Enable CRCs.

    // Clear the FIFO and restart new packet reception when CRC check fails.
    // PayloadReady is not issued.
    __rfm_register_modify(RFM_REG_PACKETCONFIG1,
                          RFM_REG_MASK_PACKETCONFIG1_CRCAUTOCLEAROFF,
                          0);

    // Variable-length packet mode
    __rfm_register_modify(RFM_REG_PACKETCONFIG1,
                          RFM_REG_MASK_PACKETCONFIG1_PACKETFORMAT,
                          1);
    __rfm_register_write(RFM_REG_PAYLOADLENGTH, 255);

    // Set data rate.
    __rfm_register_write(RFM_REG_BITRATEMSB, RFM_CONFIG_BITRATE >> 8);
    __rfm_register_write(RFM_REG_BITRATELSB, RFM_CONFIG_BITRATE & 0x00FF);

    // Sync word
    const uint8_t sync_word[] = { 0xAC, 0xDC, 0xFF, 0x06, 0x05, 0x04, 0x03 };
    __rfm_register_modify(RFM_REG_SYNCCONFIG,
                          RFM_REG_MASK_SYNCCONFIG_SYNCON,
                          1); // Enable use of the sync word.
    __rfm_register_modify(RFM_REG_SYNCCONFIG,
                          RFM_REG_MASK_SYNCCONFIG_SYNCSIZE,
                          6); // Size of the sync word is taken to be 7 = 6 + 1.
    __rfm_register_burst_write(RFM_REG_SYNCVALUE1,
                               sync_word,
                               7); // Write the 56-bit sync word.

#ifdef RFM_FEATURE_ENCRYPTION
    // Write the AES key and enable encryption if enabled.
    const uint8_t aes_key[] = RFM_CONFIG_ENCRYPTIONKEY;

    __rfm_register_burst_write(RFM_REG_AESKEY01, aes_key, 16);
    __rfm_register_modify(RFM_REG_PACKETCONFIG2, 1, 1);
#else
    // Ensure encryption is disabled.
    __rfm_register_modify(RFM_REG_PACKETCONFIG2, 1, 0);
#endif

    // Clear the FIFO.
    __rfm_register_modify(RFM_REG_IRQFLAGS2,
                          RFM_REG_MASK_IRQFLAGS2_FIFOOVERRUN,
                          1);

    // Transmit as soon as a byte is available.
    __rfm_register_modify(RFM_REG_FIFOTHRESH,
                          RFM_REG_MASK_FIFOTHRESH_TXSTARTCONDITION,
                          1);

    // Preamble size
    __rfm_register_write(RFM_REG_PREAMBLEMSB, 0);
    __rfm_register_write(RFM_REG_PREAMBLELSB, 0x40);

    return;
}

void rfm_reset()
{
    digitalWrite(RFM_CONFIG_PINRESET, HIGH);
    delayMicroseconds(100);
    digitalWrite(RFM_CONFIG_PINRESET, LOW);
    delay(5);

    return;
}

void __rfm_select()
{
    digitalWrite(RFM_CONFIG_PINSS, LOW);
    return;
}

void __rfm_deselect()
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

    __rfm_select();
    SPI.transfer(register_access);
    const uint8_t value = SPI.transfer(0);
    __rfm_deselect();

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

    __rfm_select();
    SPI.transfer(register_access);
    const uint8_t old_value = SPI.transfer(value);
    __rfm_deselect();

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

    __rfm_select();
    SPI.transfer(begin_register | 128);
    for(uint8_t offset = 0; offset < length; offset++)
        SPI.transfer(values[offset]);
    __rfm_deselect();

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

uint8_t __rfm_operating_mode()
{
    return (__rfm_register_read(RFM_REG_OPMODE) & RFM_REG_MASK_OPMODE_MODE) >> 2;
}

void __rfm_operating_mode(const uint8_t mode)
{
    __rfm_register_modify(
        RFM_REG_OPMODE,
        RFM_REG_MASK_OPMODE_MODE,
        mode);

    while(!(__rfm_register_read(RFM_REG_IRQFLAGS1) & RFM_REG_MASK_IRQFLAGS1_MODEREADY))
        delayMicroseconds(100);

    return;
}

void rfm_fifo_read(uint8_t* const buffer)
{
    uint8_t i = 0;
    while(!rfm_fifo_empty() && i < RFM_CONFIG_PACKETSIZE)
    {
        Serial.print("Saving byte to "); Serial.println(i);
        buffer[i++] = __rfm_register_read(RFM_REG_FIFO);
        Serial.print("Value: "); Serial.println(buffer[i-1], HEX);
    }

    return;
}

uint8_t rfm_fifo_write(const uint8_t* const buffer, const uint8_t size)
{
    if (size > RFM_CONFIG_PACKETSIZE)
        return 1;

    if (rfm_fifo_full())
        return 2;

// #if RFM_CONFIG_PACKETSIZE <= 66
//     __rfm_register_burst_write(RFM_REG_FIFO, buffer, size);
// #else
    uint8_t i = 0;
    while(i < size && !rfm_fifo_full())
        __rfm_register_write(RFM_REG_FIFO, buffer[i++]);
// #endif

    return 0;
}

#ifdef RFM_FEATURE_LISTEN

void __rfm_listen_mode()
{
    __rfm_operating_mode(RFM_OPMODE_STANDBY);
    __rfm_register_modify(RFM_REG_OPMODE,
                          RFM_REG_MASK_OPMODE_LISTEN,
                          1);

    return;
}

void __rfm_abort_listen_mode(const uint8_t mode)
{
    const uint8_t value =
        (__rfm_register_read(RFM_REG_OPMODE) & 128) | 32 | (mode << 2);

    __rfm_register_write(RFM_REG_OPMODE, value);
    __rfm_register_write(RFM_REG_OPMODE, value & ~RFM_REG_MASK_OPMODE_LISTENABRT);

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
    const uint8_t mode = __rfm_operating_mode();
    if(mode == RFM_OPMODE_SLEEP || mode == RFM_OPMODE_RECEIVE)
        return 0;

    // According to the datasheet, reading the temperature takes < 100us.
    // Impose a wait here instead of pestering the radio immediately.

    __rfm_register_write(RFM_REG_TEMP1, (1 << 3));

#ifndef RFM_CONFIG_COMPACT
    delayMicroseconds(100 - 10);
#endif

    while(__rfm_register_read(RFM_REG_TEMP1) & (1 << 2));

    return ~__rfm_register_read(RFM_REG_TEMP2) - 94 + offset;
}

#endif
