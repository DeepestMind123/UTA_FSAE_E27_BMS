/**
 * @file util_time.c
 * @author notwe
 * @date 2026-05-03
 * @brief utility time source
 */

#include "util_time.h"

time_status_t UTIL_Time_Init(util_time_t *p_time, util_irq_t *p_irq)
{
    time_status_t status = TIME_NOT_INIT;

    if((p_time != NULL) && (p_irq != NULL))
    {
        if(!p_time->is_init)
        {
            p_time->irq = p_irq;

            p_time->tick = 0U;
            p_time->is_init = true;

            status = TIME_OK;
        }
        else
        {
            status = TIME_DBL_INIT;
        }
    }
    else
    {
        status = TIME_NULL_PTR;
    }

    return status;
}

time_status_t UTIL_Time_Tick_Up(util_time_t *p_time)
{
    time_status_t status = TIME_OK;

    if(p_time != NULL)
    {
        if(p_time->is_init)
        {
            p_time->tick++;
        }
        else
        {
            status = TIME_NOT_INIT;
        }
    }
    else
    {
        status = TIME_NULL_PTR;
    }

    return status;
}

time_status_t UTIL_Time_Get_Tick(util_time_t *p_time, uint32_t *p_out)
{
    time_status_t status = TIME_OK;
    irq_status_t irq_status;

    uint32_t t = 0U;

    if((p_time != NULL) && (p_time->irq != NULL) && (p_out != NULL))
    {
        if(p_time->is_init)
        {
            irq_status = UTIL_IRQ_Enter_Critical(p_time->irq);

            if(irq_status == IRQ_OK)
            {
                t = p_time->tick;

                irq_status = UTIL_IRQ_Exit_Critical(p_time->irq);
            
                if(irq_status != IRQ_OK)
                {
                    status = TIME_IRQ_FAULT;
                }
            }
            else
            {
                status = TIME_IRQ_FAULT;
            }
        }
        else
        {
            status = TIME_NOT_INIT;
        }

        *p_out = t;
    }
    else
    {
        status = TIME_NULL_PTR;
    }

    return status;
}
