/**
 * @file main.c
 * @author notwe
 * @date 2026-05-03
 * @brief system manager source
 */

#include "sys_manager.h"

static sys_manager_t sys;
static sys_manager_cfg_t sys_cfg;

void sys_manager_init(sys_manager_t *p_sys, sys_manager_cfg_t *p_sys_cfg)
{
    // alias pointers
    sys = *p_sys;
    sys_cfg = *p_sys_cfg;

    sys.state = SYS_IDLE;

}

void sys_manager_task(sys_manager_t *p_sys)
{
    switch(sys.state)
    {
        case SYS_IDLE:

        break;

        case SYS_CHARGE:

        break;

        case SYS_DISCHARGE:

        break;

        case SYS_BALANCE:

        break;

        case SYS_FAULT:

        break;
    }   
}
