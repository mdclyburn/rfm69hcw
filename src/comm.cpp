#include "comm.h"

namespace mardev
{
    namespace rfm69
    {
        uint8_t mode()
        {
            return (read(registers::OpMode) & registers::mask::Mode) >> 2;
        }

        void mode(const uint8_t mode)
        {
            modify(
                registers::OpMode,
                registers::mask::Mode,
                mode);

            while(!(read(registers::IRQFlags1) & registers::mask::ModeReady));

            return;
        }
    }
}
