/**
 * @file util_irq.h
 * @author notwe
 * @date 2026-05-03
 * @brief utility interrupt header
 */

#ifndef UTIL_IRQ_H
#define UTIL_IRQ_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef enum
{
    IRQ_STATUS_OK = 0,
    IRQ_STATUS_ERROR_NOT_INIT,
    IRQ_STATUS_ERROR_NULL_POINTER
} irq_status_t;

typedef struct
{
    void(*Disable)(void);

    void(*Enable)(void);

    uint32_t(*Get_State)(void);

    void(*Set_State)(uint32_t state);

} util_irq_cfg_t;

bool is_init;

static inline irq_status_t UTIL_IRQ_Init(util_irq_cfg_t *p_cfg)
{
    irq_status_t status = IRQ_STATUS_ERROR_NOT_INIT;

    if(p_cfg == NULL)
    {
        status = IRQ_STATUS_ERROR_NULL_POINTER;
    }
    else
    {
        is_init = true;

        status = IRQ_STATUS_OK;
    }

    return status;
}

static inline irq_status_t UTIL_IRQ_Enter_Critical(util_irq_cfg_t *p_cfg, uint32_t *p_out)
{
    irq_status_t status = IRQ_STATUS_OK;

    if((p_cfg != NULL) && (p_out != NULL))
    {
        if(is_init)
        {
            uint32_t last_state = p_cfg->Get_State(); // fetch current intterupt state

            p_cfg->Disable(); // disable interrupts
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

static inline irq_status_t UTIL_IRQ_Exit_Critical(util_irq_cfg_t *p_cfg, uint32_t state)
{
    irq_status_t status = IRQ_STATUS_OK;

    if(p_cfg != NULL)
    {
        if(is_init)
        {
            p_cfg->Set_State(state); // set interrupt state to saved state

            p_cfg->Enable(); // enable interrupts
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

#endif