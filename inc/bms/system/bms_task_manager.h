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

#include "util_time.h"
#include "bms_config.h"
#include "bms_daq.h"


typedef enum
{
   TM_OK = 0,
   TM_NOT_INIT,
   TM_DBL_INIT,
   TM_UNDEF_STATE,
   TM_NULL_PTR,
   TM_TIME_FAULT,
   TM_DAQ_FAULT,
   TM_STATUS_MAX
} tm_status_t;

typedef enum
{
   TM_STATE_IDLE = 0,    // battery inactive, mcu and peripheral components enter sleep mode
   TM_STATE_DISCHARGE,   // battery discharging; bms actively checking current draw, cell voltage, thermistors, and updating soc
   TM_STATE_CHARGE,      // battery charging; bms balancing cells and confirming CC-CV scheme
   TM_STATE_BALANCE,     // battery balancing; bms switching between targeted and general balancing
   TM_STATE_ERROR,         // battery fault occured; bms attempts to diagnosis and fix or fails the pack
   TM_STATE_MAX
} tm_state_t;

typedef struct
{
   daq_timeout_t timeout_cfg;
   util_time_t *time_cfg;
} tm_cfg_t;


tm_status_t BMS_Manager_Init(const tm_cfg_t *p_cfg);            // intialize system manager with relevant constants and functions, also initializes sub-modules
tm_status_t BMS_Manager_Task(void);            // overall system state machine

#endif