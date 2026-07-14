/**
 * @file util_time.c
 * @author notwe
 * @date 2026-05-03
 * @brief utility time source
 */

#include "util_time.h"

time_status_t UTIL_Time_Init(util_time_t *p_time, const util_irq_t *p_irq)
{
    time_status_t status = TIME_STATUS_ERROR_NOT_INIT;

    if((p_time != NULL) && (p_irq != NULL))
    {
        p_time->irq = p_irq;

        p_time->tick = 0;
        p_time->is_init = true;

        status = TIME_STATUS_OK;
    }
    else
    {
        status = TIME_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}

time_status_t UTIL_Time_Tick_Up(util_time_t *p_time)
{
    time_status_t status = TIME_STATUS_OK;

    if(p_time != NULL)
    {
        if(p_time->is_init)
        {
            p_time->tick++;
        }
        else
        {
            status = TIME_STATUS_ERROR_NOT_INIT;
        }
    }
    else
    {
        status = TIME_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}

time_status_t UTIL_Time_Get_Tick(util_time_t *p_time, uint32_t *p_out)
{
    time_status_t status = TIME_STATUS_OK;
    irq_status_t irq_status;

    uint32_t interrupt_state = 0U;

    uint32_t t = 0U;

    if((p_time != NULL) && (p_time->irq != NULL) && (p_out != NULL))
    {
        if(p_time->is_init)
        {
            irq_status = UTIL_IRQ_Enter_Critical(p_time->irq, &interrupt_state);

            if(irq_status == IRQ_STATUS_OK)
            {
                t = p_time->tick;
            }
            else
            {
                status = TIME_STATUS_ERROR_IRQ_ERROR;
            }

            irq_status = UTIL_IRQ_Exit_Critical(p_time->irq, interrupt_state);
            
            if(irq_status != IRQ_STATUS_OK)
            {
                status = TIME_STATUS_ERROR_IRQ_ERROR;
            }

            *p_out = t;
        }
        else
        {
            status = TIME_STATUS_ERROR_NOT_INIT;
        }
    }
    else
    {
        status = TIME_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}
