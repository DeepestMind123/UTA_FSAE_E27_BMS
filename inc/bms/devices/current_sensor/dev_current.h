/**
 * @file DEV_current.h
 * @author notwe
 * @date 2026-05-03
 * @brief current sensor device driver header
 */

#ifndef DEV_CURRENT_H
#define DEV_CURRENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "util_time.h"
#include "io_adc.h"

#define MV_TO_UV 1000

typedef enum
{
    CURRENT_STATUS_OK = 0,
    CURRENT_STATUS_ERROR_NULL_POINTER,
    CURRENT_STATUS_ERROR_NOT_INIT,
    CURRENT_STATUS_ERROR_TIMEOUT,
    CURRENT_STATUS_ERROR_ADC_ERROR,
    CURRENT_STATUS_ERROR_UNDEFINED_STATE,
    CURRENT_STATUS_ERROR_RACE
} current_status_t;

typedef enum
{
    CURRENT_STATE_IDLE = 0,
    CURRENT_STATE_START,
    CURRENT_STATE_WAIT,
    CURRENT_STATE_GET,
    CURRENT_STATE_READY,
    CURRENT_STATE_ERROR,
    CURRENT_STATE_UNDEFINED
} current_state_t;

typedef struct
{
    uint32_t current_wait_ms;                   // timer in ms between current sensor checks
    int32_t current_gain_uV;             // gain of sensor in uV / A
    uint16_t current_raw_cutoff;                // clamp value for sensor calibration

} dev_current_cfg_t;

typedef struct
{
    uint32_t cal_acc;
    uint32_t cal_count;
    uint32_t raw_offset;                // calibrated offset value
    uint32_t adc_timeout_ms;     // time in ms before current wait times out

    int32_t last_val;

    bool is_init;

    uint32_t start_time;

    io_adc_t *adc_inst;

    current_status_t status;
    current_state_t state;

    const dev_current_cfg_t *cfg;

} dev_current_t;

current_status_t DEV_Current_Init(dev_current_t *p_inst, dev_current_cfg_t *p_cfg, io_adc_t *p_adc_inst);

current_status_t DEV_Current_Task(dev_current_t *p_inst);    // state switch function

current_status_t DEV_Current_Process_Raw(dev_current_t *p_inst);      // process raw current value to get current in mA

current_status_t DEV_Current_Get_Wait(dev_current_t *p_inst, uint32_t *p_out);

current_status_t DEV_Current_Get_Gain(dev_current_t *p_inst, int32_t *p_out);

current_status_t DEV_Current_Get_Raw(dev_current_t *p_inst, uint32_t *p_out);

current_status_t DEV_Current_Get_Val(dev_current_t *p_inst, int32_t *p_out);

current_status_t DEV_Current_Get_State(dev_current_t *p_inst, current_state_t *p_out);

#endif 