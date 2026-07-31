/**
 * @file util_time.h
 * @author notwe
 * @date 2026-05-03
 * @brief utility time header
 */

#ifndef UTIL_TIME_H
#define UTIL_TIME_H

#include <stdint.h>
#include "util_irq.h"

typedef enum
{
    TIME_STATUS_OK = 0,
    TIME_STATUS_ERROR_NULL_POINTER,
    TIME_STATUS_ERROR_NOT_INIT,
    TIME_STATUS_ERROR_IRQ_ERROR
} time_status_t;

typedef struct
{
    bool is_init;
    volatile uint32_t tick;

    const util_irq_t *irq;
} util_time_t;


time_status_t UTIL_Time_Init(util_time_t *p_time, const util_irq_t *p_irq);

time_status_t UTIL_Time_Tick_Up(util_time_t *p_time);

time_status_t UTIL_Time_Get_Tick(util_time_t *p_time, uint32_t *p_out);

#endif