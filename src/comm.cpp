#include "comm.h"

namespace mardev::rfm69
{
    uint8_t mode()
    {
        return (read(registers::OpMode) & registers::mask::Mode) >> 2;
    }

    void mode(const Mode mode)
    {
        modify(
            registers::OpMode,
            registers::mask::Mode,
            (uint8_t) mode);

        while(!(read(registers::IRQFlags1) & registers::mask::ModeReady));

        return;
    }
}
