/**
 * @file main.c
 * @author notwe
 * @date 2026-05-03
 * @brief system time header
 */

#ifndef SYS_TIME_H
#define SYS_TIME_H

#include <stdint.h>

typedef struct
{

    // external function pointers
    void(*disable_interrupts)(void);
    void(*enable_interrupts)(void);

} sys_time_cfg_t;

void sys_time_init(sys_time_cfg_t *p_time_cfg);

void tick_up(void);

uint32_t get_tick(void);

#endif