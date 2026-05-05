/**
 * @file main.c
 * @author notwe
 * @date 2026-05-03
 * @brief system time source
 */

#include "sys_time.h"

static volatile uint32_t tick = 0;
static sys_time_cfg_t cfg;

void sys_time_init(sys_time_cfg_t *p_cfg)
{
    cfg = *p_cfg;
    tick = 0;

    /*set ISR, will vary per device*/
}

void tick_up(void)
{
    tick++;
}

uint32_t get_tick(void)
{
    cfg.disable_interrupts();
    
    uint32_t t = tick;

    cfg.enable_interrupts();

    return t;
}
