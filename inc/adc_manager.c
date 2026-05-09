/**
 * @file adc_manager.c
 * @author notwe
 * @date 2026-05-03
 * @brief adc manager source
 */

#include "adc_manager.h"

void adc_manager_init(adc_manager_t *p_inst, adc_manager_cfg_t *p_cfg)
{
    p_inst->state = ADC_READY;
    p_inst->data_ready_flag = 0;
    p_inst->set_offset = p_inst->cfg->adc_offset;
    p_inst->cfg = p_cfg;
}

void adc_manager_task(adc_manager_t *p_inst)
{
    switch(p_inst->state)
    {
        case ADC_READY:

        // indicates that the adc is not busy

        break;

        case ADC_START:

        // select adc channel (defined in instance configuration in main.c)
        p_inst->cfg->ADC_channel_select(p_inst->cfg->channel_id);

        // start adc measurement on selected channel
        p_inst->cfg->ADC_start();

        p_inst->state = ADC_WAIT;

        break;

        case ADC_WAIT:

        // wait for adc to finish measurement, comment out if selected mcu does not have support for this
        if(p_inst->cfg->ADC_done) p_inst->state = ADC_GET;

        break;

        case ADC_GET:

        // stores measurement
        p_inst->last_raw = p_inst->cfg->ADC_get_result();

        p_inst->data_ready_flag = 1;

        p_inst->state = ADC_READY;

        break;
    }
}

uint8_t adc_manager_check(adc_manager_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL)) 
    {
        if(p_inst->state == ADC_READY)
        {
            p_inst->state = ADC_START;
            return 1;
        }
        return 0;
    }
    return 0; // returns safe value, indicates adc isn't read if null pointer
}

uint8_t adc_manager_get_flag(adc_manager_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        return p_inst->data_ready_flag;
    }
    return 0; // return safe value
}

uint32_t adc_manager_get_val(adc_manager_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL)) 
    {
        // stores adc measurement in temp value accessible by external functions
        uint32_t val = p_inst->last_raw;

        p_inst->data_ready_flag = 0;

        return val;
    }
    return 0; // return safe value
}

uint16_t adc_manager_get_scale(adc_manager_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL)) 
    {
        return p_inst->cfg->adc_vref_mV;
    }
    return 0; // returns safe value
}

uint16_t adc_manager_get_offset(adc_manager_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        return p_inst->set_offset;
    } 
    return 0; // returns safe value  
}

void adc_manager_set_offset(adc_manager_t *p_inst, int16_t new_val)
{
    if(p_inst != NULL)
    {
        p_inst->set_offset = new_val;
    }
}

uint16_t adc_manager_get_vref(adc_manager_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        return p_inst->cfg->adc_vref_mV;
    }
    return 0; // returns safe value
}