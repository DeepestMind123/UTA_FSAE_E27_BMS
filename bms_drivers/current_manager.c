/**
 * @file current_manager.h
 * @author notwe
 * @date 2026-05-03
 * @brief current sensor manager source
 */

#include "current_manager.h"
#include <stdint.h>

#define MV_TO_V 1000L

static current_manager_t cur;
static current_manager_cfg_t cur_cfg;

void current_manager_init(current_manager_t *p_inst, current_manager_cfg_t *p_cfg, adc_manager_t *p_adc_inst)
{
    p_inst->cfg = p_cfg;

    p_inst->elapsed = 0;
}


void current_manager_task(current_manager_t *p_inst, adc_manager_t *p_adc_inst)
{
    if(get_tick() - p_inst->elapsed >= p_inst->cfg->wait_ms)
    {
        if(adc_manager_check(p_adc_inst))
        {
            adc_manager_task(p_adc_inst);
        }
    }
}

int32_t current_manager_get_val(current_manager_t *p_inst, adc_manager_t *p_adc_inst)
{
    int32_t current_mA = 0; // initialize at safe value;

    if((p_inst != NULL) && (p_inst->cfg != NULL) && (p_adc_inst != NULL))
    {
        const uint32_t raw = adc_manager_get_val(p_adc_inst);

        const uint16_t vref = adc_manager_get_vref(p_adc_inst);

        const uint16_t scale = adc_manager_get_scale(p_adc_inst);

        const int32_t offset = adc_manager_get_offset(p_adc_inst);

        const int32_t gain_uV = p_inst->cfg->sensor_gain_uV;

        if((scale > 0U) && (gain_uV != 0))
        {
            uint32_t voltage_mV = (raw * vref) / scale;

            int32_t delta_mV = (int32_t)voltage_mV - offset;

            current_mA = (delta_mV * MV_TO_V) / gain_uV;
        }
    }

    return current_mA;
}