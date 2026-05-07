/**
 * @file current_manager.h
 * @author notwe
 * @date 2026-05-03
 * @brief current sensor manager header
 */

#ifndef CURRENT_MANAGER_H
#define CURRENT_MANAGER_H

#include <stdint.h>
#include "adc_manager.h"
#include "sys_time.h"

typedef struct
{
    uint32_t wait_ms;                   // timer in ms between current sensor checks
    int32_t sensor_gain_uV;             // gain of sensor in uV / A

} current_manager_cfg_t;

typedef struct
{
    uint32_t elapsed;                // time in ms since last current measurement

    adc_manager_t *adc_inst;

    const current_manager_cfg_t *cfg;

} current_manager_t;

void current_manager_init(current_manager_t *p_inst, current_manager_cfg_t *p_cfg, adc_manager_t *p_adc_inst);

void current_manager_task(current_manager_t *p_inst, adc_manager_t *p_adc_inst);    // state switch function

int32_t current_manager_get_val(current_manager_t *p_inst, adc_manager_t *p_adc_inst);      // process raw current value to get current in mA

uint32_t current_manager_get_gain(current_manager_t *p_inst);

#endif