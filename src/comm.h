#ifndef RFM69_COMM_H
#define RFM69_COMM_H

#include "core.h"
#include "registers.h"

namespace mardev::rfm69
{
    enum class Mode : uint8_t
    {
        Sleep = 1,
        Standby = 2,
        FrequencySynthesizer = 3,
        Transmit = 4,
        Receive = 5
    };

    /** Returns the radio's current operating mode.
     */
    uint8_t mode();

    /** Set the radio's operating mode.
     *
     * Transitions the radio to the specified operating mode.
     * This function will not return until the operating mode is ready (ModeReady is set).
     *
     * \param mode Operating mode to switch to.
     */
    void mode(const Mode mode);

    /** Returns true when the complete packet has been sent.
     */
    inline bool packet_sent()
    {
        return read(registers::IRQFlags2)
            & registers::mask::PacketSent;
    }

    /** Returns true when a payload is ready to be read from the FIFO.
     */
    inline bool packet_received()
    {
        return read(registers::IRQFlags2)
            & registers::mask::PayloadReady;
    }
}

#endif
