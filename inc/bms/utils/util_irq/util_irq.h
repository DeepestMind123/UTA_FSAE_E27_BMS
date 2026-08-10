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
    IRQ_OK = 0U,
    IRQ_NOT_INIT,
    IRQ_DBL_INIT,
    IRQ_NULL_PTR,
    IRQ_NO_SAVED_STATE,
    IRQ_STATUS_MAX
} irq_status_t;

typedef struct
{
    void(*Disable)(void);
    void(*Enable)(void);
    uint32_t(*Get_State)(void); /*NULL and 0U have to be defined as a sentinel for no saved state or an impossible return*/
    void(*Set_State)(uint32_t state);

} irq_func_t;

typedef struct
{
    bool is_init;
    bool has_saved_state;
    uint32_t last_state;

    const irq_func_t *func;
} util_irq_t;

irq_status_t UTIL_IRQ_Init(util_irq_t *p_irq, const irq_func_t *p_func);

irq_status_t UTIL_IRQ_Enter_Critical(util_irq_t *p_irq);

irq_status_t UTIL_IRQ_Exit_Critical(util_irq_t *p_irq);

#endif