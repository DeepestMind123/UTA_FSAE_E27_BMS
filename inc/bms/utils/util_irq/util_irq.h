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

typedef struct
{
    bool is_init;

    const util_irq_cfg_t *cfg;
} util_irq_t;

irq_status_t UTIL_IRQ_Init(util_irq_t *p_irq, const util_irq_cfg_t *p_cfg);

irq_status_t UTIL_IRQ_Enter_Critical(util_irq_t *p_irq, uint32_t *p_out);

irq_status_t UTIL_IRQ_Exit_Critical(util_irq_t *p_irq, uint32_t state);

#endif