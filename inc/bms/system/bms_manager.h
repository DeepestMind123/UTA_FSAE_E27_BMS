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
#include <stddef.h>
#include <stdarg.h>

#include "bms_fault.h"
#include "util_time.h"
#include "bms_config.h"

typedef enum
{
   BMS_STATUS_OK = 0,
   BMS_STATUS_ERROR_NOT_INIT,
   BMS_STATUS_ERROR_FAULT,
   BMS_STATUS_ERROR_UNDEFINED_STATE,
   BMS_STATUS_MAX
} bms_status_t;

typedef enum
{
   BMS_STATE_IDLE = 0,    // battery inactive, mcu and peripheral components enter sleep mode
   BMS_STATE_DISCHARGE,   // battery discharging; bms actively checking current draw, cell voltage, thermistors, and updating soc
   BMS_STATE_CHARGE,      // battery charging; bms balancing cells and confirming CC-CV scheme
   BMS_STATE_BALANCE,     // battery balancing; bms switching between targeted and general balancing
   BMS_STATE_UNDEFINED,
   BMS_STATE_FAULT         // battery fault occured; bms attempts to diagnosis and fix or fails the pack

} bms_state_t;

bms_status_t BMS_Manager_Init(void);    // intialize system manager with relevant constants and functions, also initializes sub-modules

bms_status_t BMS_Manager_Task(void);                                  // state switch function

#endif