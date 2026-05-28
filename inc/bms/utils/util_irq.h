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

typedef struct
{
    void(*Disable)(void);

    void(*Enable)(void);

    uint32_t(*Get_State)(void);

    void(*Set_State)(uint32_t state);

} util_irq_cfg_t;

static inline void UTIL_IRQ_Init(util_irq_cfg_t *p_cfg)
{
    if(p_cfg == NULL)
    {
        while(1);
    }
}

static inline uint32_t UTIL_IRQ_Enter_Critical(util_irq_cfg_t *p_cfg)
{
    if(p_cfg != NULL)
    {
        uint32_t last_state = p_cfg->Get_State(); // fetch current intterupt state

        p_cfg->Disable(); // disable interrupts
    }
    else 
    {
        while(1); // freeze cpu
    }
}

static inline void UTIL_IRQ_Exit_Critical(util_irq_cfg_t *p_cfg, uint32_t state)
{
    if(p_cfg != NULL)
    {
         p_cfg->Set_State(state); // set interrupt state to saved state

        p_cfg->Enable(); // enable interrupts
    }
    else 
    {
        while(1); // freeze cpu
    }
}

#endif