/**
 * @file io_adc.c
 * @author notwe
 * @date 2026-05-03
 * @brief abstract adc i/o source
 */

#include "io_adc.h"
#include "adc_io/io_adc.h"
#include "sys_fault.h"

adc_status_t IO_ADC_Init(io_adc_t *p_inst, io_adc_cfg_t *p_cfg)
{
    adc_status_t status = ADC_STATUS_ERROR_NOT_INIT;

    if((p_inst != NULL) && (p_cfg != NULL))
    {
        if((p_cfg->ADC_channel_select != NULL) &&
            (p_cfg->ADC_start != NULL) &&
            (p_cfg->ADC_done != NULL) &&
            (p_cfg->ADC_get_result != NULL) &&
            (p_cfg->ADC_stop != NULL))
        {
            p_inst->cfg = p_cfg;

            p_inst->start_time = 0U;

            p_inst->set_offset = p_inst->cfg->adc_offset;

            p_inst->is_init = true;

            status = ADC_STATUS_OK;
        }
        else 
        {
            status = ADC_STATUS_ERROR_NULL_POINTER;

            BMS_Fault_Update(BMS_FAULT_NULL_POINTER, true);
        }
        
    }
    else
    {
        status = ADC_STATUS_ERROR_NULL_POINTER;

        BMS_Fault_Update(BMS_FAULT_NULL_POINTER, true);
    }

    return status;
    
}

adc_status_t IO_ADC_Task(io_adc_t *p_inst)
{
    adc_status_t status = ADC_STATUS_OK;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        if(p_inst->is_init)
        {
            switch(p_inst->state)
            {
                case ADC_STATE_IDLE:

                // indicates that the adc is not busy

                break;

                case ADC_STATE_START:

                // select adc channel (defined in instance configuration in main.c)
                p_inst->cfg->ADC_channel_select(p_inst->cfg->channel_id);

                // start adc measurement on selected channel
                p_inst->cfg->ADC_start();

                p_inst->start_time = UTIL_Time_Get_Tick();

                p_inst->state = ADC_STATE_WAIT;

                break;

                case ADC_STATE_WAIT:

                if(UTIL_Time_Get_Tick() - p_inst->start_time <= p_inst->cfg->adc_timeout)
                {
                    // wait for adc to finish measurement, comment out if selected mcu does not have support for this
                    if(p_inst->cfg->ADC_done()) 
                    {
                        p_inst->state = ADC_STATE_GET;
                    }
                }
                else
                {
                    BMS_Fault_Update(BMS_FAULT_ADC_TIMEOUT, true);

                    p_inst->state = ADC_STATE_IDLE;
                }

                break;

                case ADC_STATE_GET:

                // stores measurement
                p_inst->last_raw = p_inst->cfg->ADC_get_result();

                p_inst->start_time = UTIL_Time_Get_Tick();

                p_inst->state = ADC_STATE_READY;

                break;

                case ADC_STATE_READY:

                break;
            }
        }
        else
        {
            status = ADC_STATUS_ERROR_NOT_INIT;
            
            BMS_Fault_Update(BMS_FAULT_NOT_INIT, true);
        }

    }
    else 
    {
        status = ADC_STATUS_ERROR_NULL_POINTER;

        BMS_Fault_Update(BMS_FAULT_NULL_POINTER, true);
    }

    return status;

}

adc_status_t IO_ADC_Start(io_adc_t *p_inst)
{
    adc_status_t status = ADC_STATUS_OK;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        if(p_inst->is_init)
        {
            p_inst->state = ADC_STATE_START;
        }
        else 
        {
            status = ADC_STATUS_ERROR_NOT_INIT;

            BMS_Fault_Update(BMS_FAULT_NOT_INIT, true);
        }
    }
    else 
    {
        status = ADC_STATUS_ERROR_NULL_POINTER;

        BMS_Fault_Update(BMS_FAULT_NULL_POINTER, true);
    }

    return status;
}

uint32_t IO_ADC_Get_Val(io_adc_t *p_inst)
{
    uint32_t val = 0U;

    if((p_inst != NULL) && (p_inst->cfg != NULL)) 
    {
        if(p_inst->is_init)
        {
            // stores adc measurement in temp value accessible by external functions
            val = p_inst->last_raw;

            if(p_inst->state == ADC_STATE_READY)
            {
                p_inst->state = ADC_STATE_IDLE;
            }
        }
        else 
        {
            BMS_Fault_Update(BMS_FAULT_NOT_INIT, true);
        }
    }
    else
    {
        BMS_Fault_Update(BMS_FAULT_NULL_POINTER, true);
    }

    return val;
}

uint16_t IO_ADC_Get_Scale(io_adc_t *p_inst)
{
    uint16_t scale = 0U;

    if((p_inst != NULL) && (p_inst->cfg != NULL)) 
    {
        if(p_inst->is_init)
        {
            scale = p_inst->cfg->adc_scale;
        }
        else 
        {
            BMS_Fault_Update(BMS_FAULT_NOT_INIT, true);
        }
    }
    else
    {
        BMS_Fault_Update(BMS_FAULT_NULL_POINTER, true);
    }

    return scale;
}

uint16_t IO_ADC_Get_Offset(io_adc_t *p_inst)
{
    uint16_t offset = 0U;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        if(p_inst->is_init)
        {
            offset = p_inst->set_offset;
        }
        else 
        {
            BMS_Fault_Update(BMS_FAULT_NOT_INIT, true);
        }
    } 
    else
    {
        BMS_Fault_Update(BMS_FAULT_NULL_POINTER, true);
    }

    return offset;  
}

adc_status_t IO_ADC_Set_Offset(io_adc_t *p_inst, int16_t new_val)
{
    adc_status_t status = ADC_STATUS_OK;

    if(p_inst != NULL)
    {
        if(p_inst->is_init)
        {
            p_inst->set_offset = new_val;
        }
        else 
        {
            status = ADC_STATUS_ERROR_NOT_INIT;

            BMS_Fault_Update(BMS_FAULT_NOT_INIT, true);
        }
    }
    else 
    {
        status = ADC_STATUS_ERROR_NULL_POINTER;

        BMS_Fault_Update(BMS_FAULT_NULL_POINTER, true);
    }
}

uint16_t IO_ADC_Get_Vref(io_adc_t *p_inst)
{
    uint16_t vref = 0U;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        if(p_inst->is_init)
        {
            vref = p_inst->cfg->adc_vref_mV;
        }
        else 
        {
            BMS_Fault_Update(BMS_FAULT_NOT_INIT, true);
        }
    }
    else
    {
        BMS_Fault_Update(BMS_FAULT_NULL_POINTER, true);
    }

    return vref;
}