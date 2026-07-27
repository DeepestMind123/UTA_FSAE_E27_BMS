/**
 * @file DEV_current_sensor.h
 * @author notwe
 * @date 2026-05-03
 * @brief current sensor device driver header
 * @todo rework timeout faults
 */

#ifndef DEV_CURRENT_H
#define DEV_CURRENT_H

#include <stdint.h>
#include <stdbool.h>

#include "util_time.h"
#include "io_adc.h"

#define MV_TO_UV 1000

typedef enum
{
    ISENSE_OK = 0,
    ISENSE_NULL_PTR,
    ISENSE_NOT_INIT,
    ISENSE_DBL_INIT,
    ISENSE_TIMEOUT,
    ISENSE_ADC_FAULT,
    ISENSE_TIME_FAULT,
    ISENSE_UNDEF_STATE,
    ISENSE_MISMATCH_STATE,
    ISENSE_BUSY,
    ISENSE_STATUS_MAX
} isense_status_t;

typedef enum
{
    ISENSE_STATE_UNDEF = 0,
    ISENSE_STATE_IDLE,
    ISENSE_STATE_START,
    ISENSE_STATE_WAIT,
    ISENSE_STATE_GET,
    ISENSE_STATE_READY,
    ISENSE_STATE_ERROR,
    ISENSE_STATE_MAX
} isense_state_t;

typedef struct
{
    uint32_t timeout_ms;
    uint16_t isense_gain_uV;                    // gain of sensor in uV / A
    uint16_t isense_raw_cutoff;                // clamp value for sensor calibration
} isense_ctx_t;

typedef struct
{
    adc_t *adc_cfg;
    const util_time_t *time_cfg;
    const isense_ctx_t isense_fund_cfg;
} isense_cfg_t;

typedef struct
{
    uint32_t start_time;
    uint32_t raw_offset;                // calibrated offset value
    uint32_t adc_timeout_ms;            // time in ms before current wait times out
    uint32_t isense_timeout_ms;          // timer in ms allowed in a state
    int32_t last_val;

    bool is_init;
    bool val_diff;
    bool is_ready;

    isense_state_t state;

    adc_t *p_adc;
    util_time_t *p_time;
    isense_ctx_t isense_ctx;
    
} isense_t;

isense_status_t DEV_Isense_Init(isense_t *p_inst, const isense_cfg_t *p_cfg);

isense_status_t DEV_Isense_Task(isense_t *p_inst);    // state switch function

isense_status_t DEV_Isense_Start(isense_t *p_inst);

isense_status_t DEV_Isense_Process_Raw(isense_t *p_inst);      // process raw current value to get current in mA

isense_status_t DEV_Isense_Get_Wait(isense_t *p_inst, uint32_t *p_out); // no, this is dumb

isense_status_t DEV_Isense_Get_Gain(isense_t *p_inst, int32_t *p_out);

isense_status_t DEV_Isense_Get_Val(isense_t *p_inst, int32_t *p_out);

isense_status_t DEV_Isense_Get_State(isense_t *p_inst, isense_state_t *p_out);

isense_status_t DEV_Isense_Get_Data_Diff(isense_t *p_inst, bool *p_out);

isense_status_t DEV_Isense_Get_Ready_Flag(isense_t *p_inst, bool *p_out);

isense_status_t DEV_Isense_Set_Timeout(isense_t *p_inst, uint32_t new_val);

isense_status_t DEV_Isense_Get_Timeout(isense_t *p_inst, uint32_t *p_out);

#endif 