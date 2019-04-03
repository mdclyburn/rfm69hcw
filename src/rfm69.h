#ifndef RFM69_H
#define RFM69_H

#include <stdint.h>

#include "rfm69_configuration.h"
#include "rfm69_registers.h"
#include "rfm69_control.h"

void rfm_initialize();

void __rfm_select();

void __rfm_deselect();

uint8_t __rfm_register_read(const uint8_t rfm_register);

uint8_t __rfm_register_write(const uint8_t rfm_register,
                           const uint8_t value);

void __rfm_register_burst_write(const uint8_t begin_register,
                              const uint8_t* const values,
                              const uint8_t length);

uint8_t __rfm_operating_mode();

void __rfm_operating_mode(const uint8_t mode);

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
