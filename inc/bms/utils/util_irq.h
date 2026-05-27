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
    uint32_t last_state = p_cfg->Get_State();

    p_cfg->Disable();
}

static inline void UTIL_IRQ_Exit_Critical(util_irq_cfg_t *p_cfg, uint32_t state)
{
    p_cfg->Set_State(state);

    p_cfg->Enable();
}

#endif