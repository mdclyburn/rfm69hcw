#include "metrics.h"

#include "comm.h"
#include "registers.h"

namespace mardev::rfm69
{
    uint8_t temperature(const int8_t offset)
    {
        /* RFM_REG_TEMP1
         * bit 7 - 4: unused
         * bit 3: triggers temperature measurement when set to 1
         * bit 2: will be set to 1 while measurement is running
         * bit 1, 0: unused
         */

        // The temperature sensor can only be used in standby or frequency
        // synthesizer modes but not in receive mode.
        const uint8_t current_mode = mode();
        if(current_mode == RFM_OPMODE_SLEEP || current_mode == RFM_OPMODE_RECEIVE)
            return 0;

        // According to the datasheet, reading the temperature takes < 100us.
        // Impose a wait here instead of pestering the radio immediately.

        write(registers::Temp1, registers::mask::TempMeasStart);

        // TODO: insert small wait here?

        while(read(registers::Temp1) & registers::mask::TempMeasRunning);

        return ~read(registers::Temp2) - 94 + offset;
    }
}
