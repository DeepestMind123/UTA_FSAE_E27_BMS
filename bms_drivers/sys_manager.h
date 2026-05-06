/**
 * @file sys_manager.h
 * @author notwe
 * @date 2026-05-03
 * @brief system manager header
 */

#ifndef SYS_MANAGER_H
#define SYS_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
   SYS_IDLE = 0,    // battery inactive, mcu and peripheral components enter sleep mode
   SYS_DISCHARGE,   // battery discharging; bms actively checking current draw, cell voltage, thermistors, and updating soc
   SYS_CHARGE,      // battery charging; bms balancing cells and confirming CC-CV scheme
   SYS_BALANCE,     // battery balancing; bms switching between targeted and general balancing
   SYS_FAULT        // battery fault occured; bms attempts to diagnosis and fix or fails the pack

} sys_state_t;

typedef struct
{
   sys_state_t state; // define state variable

   //variables


    
} sys_manager_t;

typedef struct
{
   // external function pointers
   uint32_t(*sys_tick)(void);

} sys_manager_cfg_t;

void sys_manager_init(sys_manager_t *p_sys, sys_manager_cfg_t *p_sys_cfg);   // intialize system manager with relevant constants and functions

void sys_manager_task(sys_manager_t *p_sys);                              // state switch function

#endif