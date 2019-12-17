#include "fifo.h"

namespace mardev::rfm69
{
    void read_fifo(uint8_t* const buffer)
    {
        uint8_t i = 0;
        while(!fifo_is_empty())
        {
            buffer[i++] = read(registers::FIFO);
        }

        return;
    }

    uint8_t read_fifo(uint8_t* const buffer,
                      const uint8_t max_bytes)
    {
        uint8_t i = 0;
        while(!fifo_is_empty() && i < max_bytes)
        {
            buffer[i++] = read(registers::FIFO);
        }

        return i;
    }

    uint8_t write_fifo(const uint8_t* const buffer,
                       const uint8_t size)
    {
        // Limit for the library is at 66 bytes (at least for now).
        if (size > 66)
            return 1;

        // FIFO full
        if (read(registers::IRQFlags2) & 128)
            return 2;

        // TODO: turn this into a single burst-write.
        uint8_t i = 0;
        while(i < size && !(read(registers::IRQFlags2) & 128))
            write(registers::FIFO, buffer[i++]);

        return 0;
    }
}
