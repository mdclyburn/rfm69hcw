#ifndef RFM69_METRICS_H
#define RFM69_METRICS_H

#include "control.h"
#include "core.h"
#include "registers.h"

namespace mardev::rfm69
{
    /** Returns the RSSI value.
     */
    inline int16_t rssi()
    {
        return - read(registers::RSSIValue) / 2;
    }

    /** Return the reading of the CMOS temperature sensor.
     *
     * Returns the temperature value read by the CMOS temperature sensor.
     * The temperature cannot be read while the receiver block is in use or while the radio is in sleep mode.
     * If this function is called while in either of the aforementioned modes, a 0 will be returned.
     * This function requires RFM_FEATURE_TEMPERATURE.
     *
     * \param offset adjustment value to add to the returned value
     */
    uint8_t temperature(const int8_t offset = 0);
}

#endif
