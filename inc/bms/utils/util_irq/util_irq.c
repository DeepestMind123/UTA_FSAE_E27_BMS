/**
 * @file util_irq.c
 * @author notwe
 * @date 2026-07-02
 * @brief utility interrupt source
 */

#include "util_irq.h"

irq_status_t UTIL_IRQ_Init(util_irq_t *p_irq, util_irq_cfg_t *p_cfg)
{
    irq_status_t status = IRQ_STATUS_ERROR_NOT_INIT;

    if((p_irq != NULL) && (p_cfg != NULL))
    {
        p_irq->cfg = p_cfg;

        if((p_irq->cfg->Disable != NULL) &&
            (p_irq->cfg->Enable != NULL) &&
            (p_irq->cfg->Get_State != NULL) &&
            (p_irq->cfg->Set_State != NULL))
            {
                p_irq->is_init = true;

                status = IRQ_STATUS_OK;
            }
            else
            {
                status = IRQ_STATUS_ERROR_NULL_POINTER;
            }
    }
    else
    {
        status = IRQ_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}

irq_status_t UTIL_IRQ_Enter_Critical(util_irq_t *p_irq, uint32_t *p_out)
{
    irq_status_t status = IRQ_STATUS_OK;

    uint32_t last_state;

    if((p_irq != NULL) && 
        (p_irq->cfg != NULL) && 
        (p_irq->cfg->Get_State != NULL) &&
        (p_irq->cfg->Disable) &&
        (p_out != NULL))
    {
        if(p_irq->is_init)
        {
            last_state = p_irq->cfg->Get_State(); // fetch current intterupt state

            p_irq->cfg->Disable(); // disable interrupts
        }
        else 
        {
            status = IRQ_STATUS_ERROR_NOT_INIT;
        }

        *p_out = last_state;
    }
    else 
    {
        status = IRQ_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}

irq_status_t UTIL_IRQ_Exit_Critical(util_irq_t *p_irq, uint32_t state)
{
    irq_status_t status = IRQ_STATUS_OK;

    if((p_irq != NULL) && 
        (p_irq->cfg != NULL) &&
        (p_irq->cfg->Set_State) &&
        (p_irq->cfg->Enable))
    {
        if(p_irq->is_init)
        {
            p_irq->cfg->Set_State(state); // set interrupt state to saved state

            p_irq->cfg->Enable(); // enable interrupts
        }
        else 
        {
            status = IRQ_STATUS_ERROR_NOT_INIT;
        }
    }
    else 
    {
        status = IRQ_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}