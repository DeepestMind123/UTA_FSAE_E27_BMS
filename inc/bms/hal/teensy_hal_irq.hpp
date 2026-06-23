/**
 * @file teensy_hal_irq.hpp
 * @author notwe
 * @date 2026-06-18
 * @brief teensy 4.1 irq hal wrapper basics
 */

#ifndef TEENSY_HAL_IRQ_HPP
#define TEENSY_HAL_IRQ_HPP

#include <Arduino.h>
#include <core_pins.h>

class TEENSY_IRQ
{
    public:
    static inline bool Is_Enabled()
    {
        uint32_t primask;
        // MRS: Move to Register from Special register (reads PRIMASK)
        __asm__ volatile("mrs %0, primask\n" : "=r" (primask) : : "memory");
        return (primask == 0);
    }

    static inline void Disable()
    {
        __disable_irq();
    }

    static inline void Enable()
    {
        __enable_irq();
    }

    static inline void Restore(bool state)
    {
        if(state)
        {
            __enable_irq();
        }
        else
        {
            __disable_irq();
        }
    }

};

#endif