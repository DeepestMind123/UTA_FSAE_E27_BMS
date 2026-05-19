/**
 * @file adc_manager.c
 * @author notwe
 * @date 2026-05-03
 * @brief adc manager source
 */

#include "adc_manager.h"

void adc_manager_init(adc_manager_t *p_inst, adc_manager_cfg_t *p_cfg)
{
    if((p_inst != NULL) && (p_cfg != NULL))
    {
        p_inst->cfg = p_cfg;

        p_inst->state = ADC_STATE_READY;
        p_inst->data_ready_flag = false;
        p_inst->set_offset = p_inst->cfg->adc_offset;
    }
    else
    {
        // error handler
    }
    
}

void adc_manager_task(adc_manager_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        switch(p_inst->state)
        {
            case ADC_STATE_IDLE:

            // indicates that the adc is not busy

            break;

            case ADC_STATE_START:

            p_inst->data_ready_flag = false;

            // select adc channel (defined in instance configuration in main.c)
            p_inst->cfg->ADC_channel_select(p_inst->cfg->channel_id);

            // start adc measurement on selected channel
            p_inst->cfg->ADC_start();

            uint32_t adc_start_time = get_tick();

            p_inst->state = ADC_STATE_WAIT;

            break;

            case ADC_STATE_WAIT:

            if(get_tick() - adc_start_time <= p_inst->cfg->adc_timeout)
            {
                // wait for adc to finish measurement, comment out if selected mcu does not have support for this
                if(p_inst->cfg->ADC_done()) 
                {
                    p_inst->state = ADC_STATE_GET;
                }
            }
            else
            {
                // error handler (adc timeout)

                p_inst->state = ADC_STATE_IDLE;
            }

            break;

            case ADC_STATE_GET:

            // stores measurement
            p_inst->last_raw = p_inst->cfg->ADC_get_result();

            p_inst->state = ADC_STATE_READY;

            break;

            case ADC_STATE_READY:

            p_inst->data_ready_flag = true;

            p_inst->state = ADC_STATE_IDLE;

            break;
        }
    }
    else
    {
        // error handler
    }
}

void adc_manager_start(adc_manager_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        p_inst->state = ADC_STATE_START;
    }
}

bool adc_manager_check(adc_manager_t *p_inst)
{
    bool is_busy = true;

    if((p_inst != NULL) && (p_inst->cfg != NULL)) 
    {
        if(p_inst->state == ADC_STATE_READY)
        {   
            is_busy = false;
        }
    }
    else
    {
        // insert error handler
    }

    return is_busy; // single return point for safety
}

bool adc_manager_get_flag(adc_manager_t *p_inst)
{
    bool is_ready = false;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        is_ready = p_inst->data_ready_flag;
    }
    else
    {
        // error handler
    }

    return is_ready;
}

uint32_t adc_manager_get_val(adc_manager_t *p_inst)
{
    uint32_t val = 0U;

    if((p_inst != NULL) && (p_inst->cfg != NULL)) 
    {
        // stores adc measurement in temp value accessible by external functions
        val = p_inst->last_raw;
    }
    else
    {
        // error handler
    }

    return val;
}

uint16_t adc_manager_get_scale(adc_manager_t *p_inst)
{
    uint16_t scale = 0U;

    if((p_inst != NULL) && (p_inst->cfg != NULL)) 
    {
        scale = p_inst->cfg->adc_scale;
    }
    else
    {
        // error handler
    }

    return scale;
}

uint16_t adc_manager_get_offset(adc_manager_t *p_inst)
{
    uint16_t offset = 0U;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        offset = p_inst->set_offset;
    } 
    else
    {
        // error handler
    }

    return offset;  
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
    uint16_t vref = 0U;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        vref = p_inst->cfg->adc_vref_mV;
    }
    else
    {
        //error handler
    }

    return vref;
}