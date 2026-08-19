/**
 * @file bms_report.h
 * @author notwe
 * @date 2026-07-21
 * @brief bms report over can header
 */

#ifndef BMS_REPORT_H
#define BMS_REPORT_H

#include <stdint.h>
#include <stddef.h>
#include <stddef.h>

#include "io_can.h"
#include "util_time.h"
#include "bms_data.h"

typedef enum
{
    REPORT_OK = 0U,
    REPORT_NOT_INIT,
    REPORT_DBL_INIT,
    REPORT_NULL_PTR,
    REPORT_CAN_FAULT,
    REPORT_UNDEF_STATE,
    REPORT_HRTBT_TIMEOUT,
    REPORT_TELEM_TIMEOUT,
    REPORT_MSG_BUILD_FAIL,
    REPORT_STATUS_MAX
} report_status_t;

typedef enum
{
    REPORT_STATE_UNDEF = 0U,
    REPORT_STATE_IDLE,
    REPORT_STATE_HRTBT,
    REPORT_STATE_TELEM,
    REPORT_STATE_DEBUG,
    REPORT_STATE_ERROR,
    REPORT_STATE_MAX
} report_state_t;

typedef enum
{
    FREQ_NONE = 0U,
    FREQ_HRTBT = 40U,
    FREQ_PLIM = 100U,
    FREQ_TELEM = 250U
} report_freq_t;

typedef struct
{
    uint32_t timeout;
} report_timeout_t;

typedef struct
{
    
} report_ctx_t;

typedef struct
{
    can_t *can_cfg;
    const util_time_t *time_cfg;
} report_cfg_t;

report_status_t BMS_Report_Init(report_cfg_t *p_cfg);

report_status_t BMS_Report_Task(void);

report_status_t BMS_Report_Idle(void);

void BMS_Report_Debug_CALLBACK(void);

#endif