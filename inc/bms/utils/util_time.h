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

extern util_irq_cfg_t util_irq;

void UTIL_Time_Init(void);

void UTIL_Time_Tick_Up(void);

uint32_t UTIL_Time_Get_Tick(void);

#endif