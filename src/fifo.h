#ifndef RFM69_FIFO_H
#define RFM69_FIFO_H

#include <stdint.h>

#include "core.h"
#include "registers.h"

namespace mardev::rfm69
{
    /** Write the contents of the FIFO to a buffer.
     *
     * Copies the data from the RFM FIFO to the given buffer.
     * Amount of space needed is up to the max length of a message.
     *
     * \param buffer Buffer to write the contents of the FIFO to. It must be large enough to hold the maximum message size.
     */
    void read_fifo(uint8_t* const buffer);

    /** Write the contents of a buffer to the FIFO.
     *
     * Copies the data from the buffer to the FIFO.
     * Returns
     *  0 = success
     *  1 = payload too large
     *  2 = FIFO is currently full and no bytes were written
     *
     * \param buffer Buffer to write to the FIFO.
     * \param size Length of the buffer.
     */
    uint8_t write_fifo(const uint8_t* const buffer,
                       const uint8_t size);

    /** Clears the contents of the FIFO.
     */
    inline void clear_fifo()
    {
        mardev::rfm69::modify(registers::IRQFlags2,
                              registers::mask::FIFOOverrun,
                              1);
        return;
    }

    /** Returns true if the FIFO is empty.
     */
    inline bool fifo_is_empty()
    {
        return !(read(registers::IRQFlags2)
                 & registers::mask::FIFONotEmpty);
    }

    /** Returns true if the FIFO is full.
     */
    inline bool fifo_is_full()
    {
        return (read(registers::IRQFlags2)
                & registers::mask::FIFOFull);
    }
}

#endif
