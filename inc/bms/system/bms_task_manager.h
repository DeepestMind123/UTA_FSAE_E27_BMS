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
#include "bms_eval.h"

typedef enum
{
   BMS_TM_OK = 0,
   BMS_TM_NOT_INIT,
   BMS_TM_DBL_INIT,
   BMS_TM_UNDEF_STATE,
   BMS_TM_NULL_PTR,
   BMS_TM_TIME_FAULT,
   BMS_TM_DAQ_FAULT,
   BMS_TM_EVAL_FAULT,
   BMS_TM_STATUS_MAX
} bms_tm_status_t;

typedef enum
{
   BMS_TM_STATE_IDLE = 0,
   BMS_TM_STATE_PRECHG,
   BMS_TM_STATE_CHG,
   BMS_TM_STATE_DCHG,
   BMS_TM_STATE_FAULT,
   BMS_TM_STATE_ERROR,
   BMS_TM_STATE_MAX
} bms_tm_state_t;

typedef struct
{
   daq_system_ctx_t timeout_cfg;
   util_time_t *time_cfg;
} bms_tm_cfg_t;

bms_tm_status_t BMS_TM_Init(const bms_tm_cfg_t *p_cfg);            // intialize system manager with relevant constants and functions, also initializes sub-modules

bms_tm_status_t BMS_TM_Task(void);            // overall system state machine

#endif