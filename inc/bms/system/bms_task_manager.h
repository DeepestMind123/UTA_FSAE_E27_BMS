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

#include "dev_current_sensor.h"

#define SENSOR_HANDOFF_MIN 7000L
#define SENSOR_HANDOFF_MAX 7500L
#define MAX_CELLS_PER_MODULE 255
#define MAX_TEMPS_PER_MODULE 255


typedef enum
{
   BMS_STATUS_OK = 0,
   BMS_STATUS_ERROR_NOT_INIT,
   BMS_STATUS_ERROR_FAULT,
   BMS_STATUS_ERROR_UNDEFINED_STATE,
   BMS_STATUS_ERROR_NULL_POINTER,
   BMS_STATUS_ERROR_CURRENT_SENSOR_FAILURE,
   BMS_STATUS_MAX
} bms_status_t;

typedef enum
{
   BMS_STATE_UNDEFINED = 0,
   BMS_STATE_IDLE,    // battery inactive, mcu and peripheral components enter sleep mode
   BMS_STATE_DISCHARGE,   // battery discharging; bms actively checking current draw, cell voltage, thermistors, and updating soc
   BMS_STATE_CHARGE,      // battery charging; bms balancing cells and confirming CC-CV scheme
   BMS_STATE_BALANCE,     // battery balancing; bms switching between targeted and general balancing
   BMS_STATE_FAULT,         // battery fault occured; bms attempts to diagnosis and fix or fails the pack
   BMS_STATE_MAX
} bms_state_t;

typedef enum
{
   BMS_DISCHARGE_STATE_UNDEFINED = 0,
   BMS_DISCHARGE_STATE_IDLE,
   BMS_DISCHARGE_STATE_REPORT,
   BMS_DISCHARGE_STATE_ERROR,
   BMS_DISCHARGE_STATE_MAX
} bms_discharge_state_t;

typedef struct
{
   uint16_t module_cells_mV[MAX_CELLS_PER_MODULE];
   uint16_t module_temps_dC[MAX_TEMPS_PER_MODULE];
} bms_module_state_t;

typedef struct
{
   int16_t last_current_mA;
   uint32_t last_time_ms;
} bms_pack_state_t;


bms_status_t BMS_Manager_Init(void);            // intialize system manager with relevant constants and functions, also initializes sub-modules
bms_status_t BMS_Manager_Task(void);            // overall system state machine
bms_status_t BMS_Manager_Discharge_Task(void);  // state machine for when bms detects discharging
bms_status_t BMS_Manager_Current_Sensor_Task(void); // decides which current sensor to use and limits hysteresis 

#endif