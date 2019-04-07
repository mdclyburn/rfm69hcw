#include <SPI.h>

#include "rfm69.h"

void rfm_initialize()
{
    SPI.begin();
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);

    // Set up digital pins.
    pinMode(RFM_CONFIG_PINSS, OUTPUT);

#ifdef RFM_FEATURE_LISTEN
    __rfm_register_modify(RFM_REG_OPMODE,
                          RFM_REG_MASK_OPMODE_LISTEN,
                          1);
#else
    __rfm_register_modify(RFM_REG_OPMODE,
                          RFM_REG_MASK_OPMODE_LISTEN,
                          0);
#endif

    // Node addressing.
#ifdef RFM_FEATURE_ADDRESSING
    __rfm_register_write(RFM_REG_NODEADRS, RFM_CONFIG_NODEADDRESS);
    __rfm_register_modify(RFM_REG_PACKETCONFIG1,
                          RFM_REG_MASK_PACKETCONFIG1_ADDRESSFILTERING,
                          2);
#endif

    // CRC
#ifdef RFM_FEATURE_CRC
    __rfm_register_modify(RFM_REG_PACKETCONFIG1,
                          RFM_REG_MASK_PACKETCONFIG1_CRCON,
                          1);

    // Clear the FIFO and restart new packet reception, PayloadReady is not issued.
    // Perhaps create a config #define to keep corrupted packets?
    __rfm_register_modify(RFM_REG_PACKETCONFIG1,
                          RFM_REG_MASK_PACKETCONFIG1_CRCAUTOCLEAROFF,
                          0);
#else
    __rfm_register_modify(RFM_REG_PACKETCONFIG1,
                          RFM_REG_MASK_PACKETCONFIG1_CRCON,
                          0);
#endif

    // Set packet mode options.
#ifdef RFM_CONFIG_PACKETFIXED
    __rfm_register_modify(RFM_REG_PACKETCONFIG1,
                          RFM_REG_MASK_PACKETCONFIG1_PACKETFORMAT,
                          0);
    _rfm_register_write(RFM_REG_PAYLOADLENGTH, RFM_CONFIG_PACKETSIZE);
#endif

#ifdef RFM_CONFIG_PACKETVARIABLE
    __rfm_register_modify(RFM_REG_PACKETCONFIG1,
                          RFM_REG_MASK_PACKETCONFIG1_PACKETFORMAT,
                          1);
    __rfm_register_write(RFM_REG_PAYLOADLENGTH, RFM_CONFIG_PACKETSIZE);
#endif

    // Set up reset.
#ifdef RFM_FEATURE_RESET
    pinMode(RFM_CONFIG_PINRESET, OUTPUT);
#endif

    __rfm_operating_mode(RFM_OPMODE_SLEEP);

    // Set data rate.
    __rfm_register_write(RFM_REG_BITRATEMSB, RFM_CONFIG_BITRATE >> 8);
    __rfm_register_write(RFM_REG_BITRATELSB, RFM_CONFIG_BITRATE & 0x00FF);

#ifdef RFM_FEATURE_SYNCWORD
    // Write the sync word and enable sync word use if enabled.
    const uint8_t sync_word[] = RFM_CONFIG_SYNCWORD;

    __rfm_register_burst_write(RFM_REG_SYNCVALUE1, sync_word, RFM_CONFIG_SYNCWORDLENGTH+1);
    __rfm_register_write(RFM_REG_SYNCCONFIG,
                         __rfm_register_read(RFM_REG_SYNCCONFIG | 1));

//     Serial.println("Sync word:");
//     for(uint8_t i = 0; i <= RFM_CONFIG_SYNCWORDLENGTH; i++)
//         Serial.println(__rfm_register_read(RFM_REG_SYNCVALUE1+i), HEX);
#else
    // Ensure sync word detection is disabled.
    __rfm_register_write(RFM_REG_SYNCCONFIG,
                         __rfm_register_read(RFM_REG_SYNCCONFIG) & ~128);
#endif

#ifdef RFM_FEATURE_ENCRYPTION
    // Write the AES key and enable encryption if enabled.
    const uint8_t aes_key[] = RFM_CONFIG_ENCRYPTIONKEY;

    __rfm_register_burst_write(RFM_REG_AESKEY01, aes_key, 16);
    __rfm_register_write(RFM_REG_PACKETCONFIG2,
                         __rfm_register_read(RFM_REG_PACKETCONFIG2 | 1));
#else
    // Ensure encryption is disabled.
    __rfm_register_write(RFM_REG_PACKETCONFIG2,
                         __rfm_register_read(RFM_REG_PACKETCONFIG2 & 254));
#endif

    // Clear the FIFO.
    __rfm_register_modify(RFM_REG_IRQFLAGS2,
                          RFM_REG_MASK_IRQFLAGS2_FIFOOVERRUN,
                          1);

    // Transmit as soon as a byte is available.
    __rfm_register_modify(RFM_REG_FIFOTHRESH,
                          RFM_REG_MASK_FIFOTHRESH_TXSTARTCONDITION,
                          1);

    return;
}

#ifdef RFM_FEATURE_RESET
void rfm_reset()
{
    digitalWrite(RFM_CONFIG_PINRESET, HIGH);
    delayMicroseconds(100);
    digitalWrite(RFM_CONFIG_PINRESET, LOW);
    delay(5);

    return;
}
#endif

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

    while(__rfm_register_read(!(RFM_REG_IRQFLAGS1 & RFM_REG_MASK_IRQFLAGS1_MODEREADY)))
        delayMicroseconds(100);

    return;
}

void rfm_fifo_read(uint8_t* const buffer)
{
    uint8_t i = 0;
    while(!rfm_fifo_empty())
        buffer[i++] = __rfm_register_read(RFM_REG_FIFO);

    return;
}

uint8_t rfm_fifo_write(const uint8_t* const buffer, const uint8_t size)
{
    if (size > RFM_CONFIG_PACKETSIZE)
        return 1;

    if (rfm_fifo_full())
        return 2;

#if RFM_CONFIG_PACKETSIZE <= 66
    __rfm_register_burst_write(RFM_REG_FIFO, buffer, size);
#else
    uint8_t i = 0;
    while(i < size && !rfm_fifo_full())
        __rfm_register_write(RFM_REG_FIFO, buffer[i++]);
#endif

    return 0;
}

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
