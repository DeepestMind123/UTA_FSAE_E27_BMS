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

#include "io_adc.h"
#include "util_time.h"

const uint16_t MV_TO_UV = 1000;

typedef struct
{
    uint32_t wait_ms;                   // timer in ms between current sensor checks
    int32_t sensor_gain_uV;             // gain of sensor in uV / A
    uint16_t raw_cutoff;                // clamp value for sensor calibration

} dev_current_cfg_t;

typedef struct
{
    uint32_t elapsed;                   // time in ms since last current measurement
    uint32_t cal_acc;
    uint32_t cal_count;
    uint32_t raw_offset;                // calibrated offset value

    int32_t val_buffer;

    bool busy_flag;
    bool ready_flag;

    io_adc_t *adc_inst;

    const dev_current_cfg_t *cfg;

} dev_current_t;

void DEV_Current_Init(dev_current_t *p_inst, dev_current_cfg_t *p_cfg, io_adc_t *p_adc_inst);

void DEV_Current_Task(dev_current_t *p_inst);    // state switch function

bool DEV_Current_Val_Ready(dev_current_t *p_inst);

bool DEV_Current_Cal(dev_current_t *p_inst);

void DEV_Current_Process_Raw(dev_current_t *p_inst);      // process raw current value to get current in mA

uint32_t DEV_Current_Get_Wait(dev_current_t *p_isnt);

int32_t DEV_Current_Get_Gain(dev_current_t *p_inst);

uint32_t DEV_Current_Get_Raw(dev_current_t *p_inst);

int32_t DEV_Current_Get_Val(dev_current_t *p_isnt);

bool DEV_Current_Get_Ready(dev_current_t *p_inst);

#endif 