/**
 * @file DEV_current_sensor.h
 * @author notwe
 * @date 2026-05-03
 * @brief current sensor device driver header
 * @todo 
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
    CURRENT_SENSOR_OK = 0,
    CURRENT_SENSOR_NULL_POINTER,
    CURRENT_SENSOR_NOT_INIT,
    CURRENT_SENSOR_TIMEOUT,
    CURRENT_SENSOR_ADC_ERROR,
    CURRENT_SENSOR_TIME_ERROR,
    CURRENT_SENSOR_UNDEF_STATE,
    CURRENT_SENSOR_STATE_MISMATCH,
    CURRENT_SENSOR_BUSY,
    CURRENT_SENSOR_STATUS_MAX
} current_sensor_status_t;

typedef enum
{
    CURRENT_SENSOR_STATE_UNDEF = 0,
    CURRENT_SENSOR_IDLE,
    CURRENT_SENSOR_START,
    CURRENT_SENSOR_WAIT,
    CURRENT_SENSOR_GET,
    CURRENT_SENSOR_READY,
    CURRENT_SENSOR_ERROR,
    CURRENT_SENSOR_STATE_MAX
} current_sensor_state_t;

typedef struct
{
    int32_t current_gain_uV;                    // gain of sensor in uV / A
    uint16_t current_raw_cutoff;                // clamp value for sensor calibration

} dev_current_sensor_cfg_t;

typedef struct
{
    uint32_t cal_acc;
    uint32_t cal_count;
    uint32_t raw_offset;                // calibrated offset value
    uint32_t adc_timeout_ms;            // time in ms before current wait times out
    uint32_t current_timeout;           // timer in ms between current sensor checks

    int32_t last_val;
    uint32_t start_time;

    bool is_init;
    bool val_diff;
    bool is_ready;

    io_adc_t *adc_inst;
    current_sensor_status_t status;
    current_sensor_state_t state;
    const util_time_t *time_inst;
    const dev_current_sensor_cfg_t *cfg;

} dev_current_sensor_t;

current_sensor_status_t DEV_Current_Sensor_Init(dev_current_sensor_t *p_inst, const dev_current_sensor_cfg_t *p_cfg, const io_adc_t *p_adc_inst, const util_time_t *p_time_inst);

current_sensor_status_t DEV_Current_Sensor_Task(dev_current_sensor_t *p_inst);    // state switch function

current_sensor_status_t DEV_Current_Sensor_Start(dev_current_sensor_t *p_inst);

current_sensor_status_t DEV_Current_Sensor_Process_Raw(dev_current_sensor_t *p_inst);      // process raw current value to get current in mA

current_sensor_status_t DEV_Current_Sensor_Set_Timeout(dev_current_sensor_t *p_inst, uint32_t new_val);

current_sensor_status_t DEV_Current_Sensor_Get_Wait(dev_current_sensor_t *p_inst, uint32_t *p_out);

current_sensor_status_t DEV_Current_Sensor_Get_Gain(dev_current_sensor_t *p_inst, int32_t *p_out);

current_sensor_status_t DEV_Current_Sensor_Get_Val(dev_current_sensor_t *p_inst, int16_t *p_out);

current_sensor_status_t DEV_Current_Sensor_Get_State(dev_current_sensor_t *p_inst, current_sensor_state_t *p_out);

current_sensor_status_t DEV_Current_Sensor_Get_Data_Diff(dev_current_sensor_t *p_inst, bool *p_out);

current_sensor_status_t DEV_Current_Sensor_Get_Ready_Flag(dev_current_sensor_t *p_inst, bool *p_out);

current_sensor_status_t DEV_Current_Sensor_Get_Timeout(dev_current_sensor_t *p_inst, uint32_t *p_out);

#endif 