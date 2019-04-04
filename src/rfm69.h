#ifndef RFM69_H
#define RFM69_H

#include <stdint.h>

#include "rfm69_configuration.h"
#include "rfm69_registers.h"
#include "rfm69_control.h"

/** Initialize the RFM69HCW to a known state.
 *
 * Writes configuration to the RFM69's registers.
 * The configured settings depend on what has been specified through #defines.
 */
void rfm_initialize();

/** Begin a transaction with the radio module.
 */
void __rfm_select();

/** End a transaction with the radio module.
 */
void __rfm_deselect();

/** Read from a register.
 *
 * Reads value of the specified register from the radio.
 * See rfm_registers.h for a list of registers.
 *
 * \param rfm_register Address of the register to read the value of.
 */
uint8_t __rfm_register_read(const uint8_t rfm_register);

/** Write to a register.
 *
 * Writes the specified value to the specified register.
 * See rfm_registers.h for a list of registers.
 *
 * \param rfm_register Address of the register to write to.
 * \param value Value to write to the register.
 */
uint8_t __rfm_register_write(const uint8_t rfm_register,
                           const uint8_t value);

/** Write to registers sequentially.
 *
 * Writes the provided values to registers beginning with begin_register.
 *
 * \param begin_register First register to write to.
 * \param values Values to write to registers.
 * \param length Number of values provided in values.
 */
void __rfm_register_burst_write(const uint8_t begin_register,
                              const uint8_t* const values,
                              const uint8_t length);

/** Returns the radio's current operating mode.
 */
uint8_t __rfm_operating_mode();

/** Set the radio's operating mode.
 *
 * Transitions the radio to the specified operating mode.
 * This function will not return until the operating mode is ready (ModeReady is set).
 *
 * \param mode Operating mode to switch to.
 */
void __rfm_operating_mode(const uint8_t mode);

// ===== Reset Pin =============================================================
// =============================================================================
#ifdef RFM_FEATURE_RESET

/** Reset the radio.
 *
 * Performs a POR on the radio.
 * When the function returns, the radio is immediately ready when this function returns.
 * This function requires RFM_FEATURE_RESET.
 */
void rfm_reset();

#endif

// ===== Temperature ===========================================================
// =============================================================================
#ifdef RFM_FEATURE_TEMPERATURE

/** Return the reading of the CMOS temperature sensor.
 *
 * Returns the temperature value read by the CMOS temperature sensor.
 * The temperature cannot be read while the receiver block is in use or while the radio is in sleep mode.
 * If this function is called while in either of the aforementioned modes, a 0 will be returned.
 * This function requires RFM_FEATURE_TEMPERATURE.
 *
 * \param offset adjustment value to add to the returned value
 */
uint8_t rfm_temperature(const int8_t offset = 0);

#endif

#endif
