#ifndef RFM69_CORE_H
#define RFM69_CORE_H

#include <stdint.h>

namespace mardev
{
    namespace rfm69
    {
        /** Initialize the RFM69HCW to a known state.
         *
         * Writes configuration to the RFM69's registers.
         * The configured settings depend on what has been specified through defines.
         */
        void initialize();

        /** Reset the radio.
         *
         * Performs a POR on the radio.
         * When the function returns, the radio is immediately ready when this function returns.
         * This function requires RFM_FEATURE_RESET.
         */
        void reset();

        // ===== Register Operations ===================================================
        // =============================================================================

        /** Read from a register.
         *
         * Reads value of the specified register from the radio.
         * See rfm_registers.h for a list of registers.
         *
         * \param address Address of the register to read the value of.
         */
        uint8_t read(const uint8_t address);

        /** Write to a register.
         *
         * Writes the specified value to the specified register.
         * See rfm_registers.h for a list of registers.
         *
         * \param address Address of the register to write to.
         * \param value Value to write to the register.
         *
         * \returns Previous value stored in the register.
         */
        uint8_t write(const uint8_t address,
                      const uint8_t value);

        /** Write to registers sequentially.
         *
         * Writes the provided values to registers beginning with begin_register.
         *
         * \param begin_address First register to write to.
         * \param values Values to write to registers.
         * \param length Number of values provided in values.
         */
        void write(const uint8_t begin_address,
                   const uint8_t* const values,
                   const uint8_t length);

        /** Set the specified option.
         *
         * Inserts value into rfm_register without losing the state of the unmasked bits.
         *
         * \param rfm_register Address of the register to modify.
         * \param mask Bit mask of the bits to be set.
         * \param value Value to set in the mask.
         */
        void modify(const uint8_t address,
                    const uint8_t mask,
                    const uint8_t value);
    }
}

#endif
