/**
 * @file bms_manager.h
 * @author notwe
 * @date 2026-05-03
 * @brief system manager header
 */

#ifndef BMS_MANAGER_H
#define BMS_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
   BMS_STATE_IDLE = 0,    // battery inactive, mcu and peripheral components enter sleep mode
   BMS_STATE_DISCHARGE,   // battery discharging; bms actively checking current draw, cell voltage, thermistors, and updating soc
   BMS_STATE_CHARGE,      // battery charging; bms balancing cells and confirming CC-CV scheme
   BMS_STATE_BALANCE,     // battery balancing; bms switching between targeted and general balancing
   BMS_STATE_FAULT        // battery fault occured; bms attempts to diagnosis and fix or fails the pack

} bms_state_t;

typedef struct
{
   bms_state_t state; // define state variable

   //variables


    
} bms_manager_t;

typedef struct
{
   // external function pointers
   uint32_t(*sys_tick)(void);

} bms_manager_cfg_t;

void BMS_Manager_Init(bms_manager_t *p_mgr, const bms_manager_cfg_t *p_mgr_cfg);    // intialize system manager with relevant constants and functions, also initializes sub-modules

void BMS_Manager_Task(bms_manager_t *p_mgr);                                  // state switch function

#endif