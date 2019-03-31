#include <SPI.h>

#include "rfm69.h"

void rfm_initialize(rfm_state_t& rfm)
{
    SPI.begin();
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);

    rfm.operating_mode = RFM_OPMODE_STANDBY;

    return;
}

void rfm_select()
{
    digitalWrite(RFM_CONFIG_SSPIN, LOW);
    return;
}

void rfm_deselect()
{
    digitalWrite(RFM_CONFIG_SSPIN, HIGH);
    return;
}

uint8_t rfm_register_read(const uint8_t rfm_register)
{
    const uint8_t register_access =
        rfm_register & 127;

#ifdef RFM_DEBUG
    Serial.print("RFM: read register ");
    Serial.print(rfm_register);
#endif

    rfm_select();
    SPI.transfer(register_access);
    const uint8_t value = SPI.transfer(0);
    rfm_deselect();

#ifdef RFM_DEBUG
    Serial.print("RFM: register read returned: ");
    Serial.println(value);
#endif

    return value;
}

uint8_t rfm_register_write(const uint8_t rfm_register,
                           const uint8_t value)
{
    const uint8_t register_access =
        rfm_register | 128;

#ifdef RFM_DEBUG
    Serial.print("RFM: write register ");
    Serial.println(rfm_register);
#endif

    rfm_select();
    SPI.transfer(register_access);
    const uint8_t old_value = SPI.transfer(value);
    rfm_deselect();

#ifdef RFM_DEBUG
    Serial.print("RFM: register write returned old value: ");
    Serial.println(old_value);
#endif

    return old_value;
}

uint8_t rfm_operating_mode()
{
    return rfm_register_read(RFM_REG_OPMODE);
}

uint16_t rfm_bitrate()
{
    return (rfm_register_read(RFM_REG_BITRATEMSB) << 8)
        | rfm_register_read(RFM_REG_BITRATELSB);
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

    // According to the datasheet, reading the temperature takes < 100us.
    // Impose a wait here instead of pestering the radio immediately.

    rfm_register_write(RFM_REG_TEMP1, (1 << 3));

#ifndef RFM_CONFIG_COMPACT
    delayMicroseconds(100 - 10);
#endif

    while(rfm_register_read(RFM_REG_TEMP1) & (1 << 2));

    return ~rfm_register_read(RFM_REG_TEMP2) - 94 + offset;
}

#endif
