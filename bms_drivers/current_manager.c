/**
 * @file current_manager.h
 * @author notwe
 * @date 2026-05-03
 * @brief current sensor manager source
 */

#include "current_manager.h"
#include "adc_manager.h"
#include <stdint.h>

#define MV_TO_UV 1000L

void current_manager_init(current_manager_t *p_inst, current_manager_cfg_t *p_cfg, adc_manager_t *p_adc_inst)
{
    if((p_inst != NULL) && (p_cfg != NULL) && (p_adc_inst != NULL))
    {
        p_inst->cfg = p_cfg;

        p_inst->elapsed = 0;
        p_inst->cal_acc = 0;
        p_inst->cal_count = 0;
        p_inst->raw_offset = 0;
    }
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

uint8_t current_manager_ready(current_manager_t *p_inst, adc_manager_t *p_adc_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL) && (p_adc_inst != NULL))
    {
        if(p_adc_inst->data_ready_flag)
        {
            return 1;
        }
    }
    return 0; // return safe value
}

uint8_t current_manager_calibrate(current_manager_t *p_inst, adc_manager_t *p_adc_inst)
{

    uint32_t out = 0;

    if((p_inst != NULL) && (p_inst->cfg != NULL) && (p_adc_inst != NULL))
    {
        if(p_inst->cal_count < 32)
        {
            if(current_manager_ready(p_inst, p_adc_inst))
            {
                out = adc_manager_get_val(p_adc_inst);

                if(out <= p_inst->cfg->raw_cutoff)
                {
                    p_inst->cal_acc += out;

                    p_inst->cal_count++;
                }
            }
            else if(adc_manager_check(p_adc_inst))
            {
                current_manager_task(p_inst, p_adc_inst);
            }
        }

        if(p_inst->cal_count >= 31)
        {
            p_inst->raw_offset = p_inst->cal_acc / (uint32_t)p_inst->cal_count;

            return 1; // calibration done
        }
    }
    return 0; 
}

int32_t current_manager_get_val(current_manager_t *p_inst, adc_manager_t *p_adc_inst)
{
    int32_t current_mA = 0; // initialize at safe value;

    if((p_inst != NULL) && (p_inst->cfg != NULL) && (p_adc_inst != NULL))
    {
        const uint32_t raw = current_manager_get_raw(p_inst, p_adc_inst);

        const uint16_t vref = adc_manager_get_vref(p_adc_inst);

        const uint16_t scale = adc_manager_get_scale(p_adc_inst);

        const int32_t offset = adc_manager_get_offset(p_adc_inst);

        const int32_t gain_uV = p_inst->cfg->sensor_gain_uV;

        if((scale > 0U) && (gain_uV != 0))
        {
            uint32_t voltage_mV = (raw * vref) / scale;

            int32_t delta_mV = (int32_t)voltage_mV - offset;

            current_mA = (delta_mV * MV_TO_UV) / gain_uV;
        }
    }

    return current_mA;
}

uint32_t current_manager_get_wait(current_manager_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        return p_inst->cfg->wait_ms;
    }
    return 0;
}

int32_t current_manager_get_gain(current_manager_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        return p_inst->cfg->sensor_gain_uV;
    }
    return 0;
}

uint32_t current_manager_get_raw(current_manager_t *p_inst, adc_manager_t *p_adc_inst)
{
    if((p_inst != NULL) && (p_adc_inst != NULL))
    {
        int64_t raw = (int64_t)adc_manager_get_val(p_adc_inst);

        raw -= p_inst->raw_offset; 

        if(raw < 0)
        {
            raw = 0;
        }

        return (uint32_t)raw;
    }
    return 0;
}