/**
 * @file util_irq.c
 * @author notwe
 * @date 2026-07-02
 * @brief utility interrupt source
 */

#include "util_irq.h"

irq_status_t UTIL_IRQ_Init(util_irq_t *p_irq, const irq_func_t *p_func)
{
    irq_status_t status = IRQ_NOT_INIT;

    if((p_irq != NULL) && (p_func != NULL))
    {
        if(!p_irq->is_init)
        {
            p_irq->func = p_func;

            if((p_irq->func->Disable != NULL) &&
                (p_irq->func->Enable != NULL) &&
                (p_irq->func->Get_State != NULL) &&
                (p_irq->func->Set_State != NULL))
            {
                p_irq->has_saved_state = false;
                
                p_irq->is_init = true;

                status = IRQ_OK;
            }
            else
            {
                status = IRQ_NULL_PTR;
            }
        }
        else
        {
            status = IRQ_DBL_INIT;
        }
    }
    else
    {
        status = IRQ_NULL_PTR;
    }

    return status;
}

irq_status_t UTIL_IRQ_Enter_Critical(util_irq_t *p_irq)
{
    irq_status_t status = IRQ_OK;

    if(p_irq != NULL)
    {
<<<<<<< Updated upstream
        p_irq->last_state = 0U;;
=======
        p_irq->last_state = 0U;
>>>>>>> Stashed changes

        if(p_irq->is_init)
        {
            p_irq->last_state = p_irq->func->Get_State(); // fetch current intterupt state

            if(p_irq->last_state != 0U)
            {
                p_irq->has_saved_state = true;
            }

            p_irq->func->Disable(); // disable interrupts
        }
        else 
        {
            status = IRQ_NOT_INIT;
        }
    }
    else 
    {
        status = IRQ_NULL_PTR;
    }

    return status;
}

irq_status_t UTIL_IRQ_Exit_Critical(util_irq_t *p_irq)
{
    irq_status_t status = IRQ_OK;

    if(p_irq != NULL)
    {
        if(p_irq->is_init)
        {
            if(p_irq->has_saved_state)
            {
                p_irq->func->Set_State(p_irq->last_state); // set interrupt state to saved state

                p_irq->func->Enable(); // enable interrupts

                p_irq->has_saved_state = false;
            }
            else
            {
                status = IRQ_NO_SAVED_STATE;
            }
        }
        else 
        {
            status = IRQ_NOT_INIT;
        }
    }
    else 
    {
        status = IRQ_NULL_PTR;
    }

    return status;
}