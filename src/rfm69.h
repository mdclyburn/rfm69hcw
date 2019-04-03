#ifndef RFM69_H
#define RFM69_H

#include <stdint.h>

#include "rfm69_configuration.h"
#include "rfm69_registers.h"

#define RFM_OPMODE_SLEEP      0 // Sleep mode
#define RFM_OPMODE_STANDBY    1 // Standby mode
#define RFM_OPMODE_FS         2 // Frequency synthesizer mode
#define RFM_OPMODE_TRANSMIT   3 // Transmitter mode
#define RFM_OPMODE_RECEIVE    4 // Receiver mode

void rfm_initialize();

void rfm_select();

void rfm_deselect();

uint8_t rfm_register_read(const uint8_t rfm_register);

uint8_t rfm_register_write(const uint8_t rfm_register,
                           const uint8_t value);

void rfm_register_burst_write(const uint8_t begin_register,
                              const uint8_t* const values,
                              const uint8_t length);

uint8_t rfm_operating_mode();

void rfm_operating_mode(const uint8_t mode);

uint16_t rfm_bitrate();

// ===== Reset Pin =============================================================
// =============================================================================
#ifdef RFM_FEATURE_RESET

void rfm_reset();

#endif

// ===== Temperature ===========================================================
// =============================================================================
#ifdef RFM_FEATURE_TEMPERATURE

/** Return the reading of the CMOS temperature sensor.
 *
 * \param offset adjustment value to add to the returned value
 */
uint8_t rfm_temperature(const int8_t offset = 0);

#endif

#endif
