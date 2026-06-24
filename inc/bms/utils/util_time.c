/**
 * @file util_time.c
 * @author notwe
 * @date 2026-05-03
 * @brief utility time source
 */

#include "util_time.h"

util_irq_cfg_t util_irq;

static volatile uint32_t tick = 0;

void UTIL_Time_Init(void)
{
    tick = 0;
}

void UTIL_Time_Tick_Up(void)
{
    tick++;
}

uint32_t UTIL_Time_Get_Tick(void)
{
    uint32_t interrupt_state = 0U;

    irq_status_t irq_status;

    uint32_t t = 0U;
    
    irq_status = UTIL_IRQ_Enter_Critical(&util_irq, &interrupt_state);
    
    if(irq_status == IRQ_STATUS_OK)
    {
        t = tick;
    }

    irq_status = UTIL_IRQ_Exit_Critical(&util_irq, interrupt_state);

    return t;
}
