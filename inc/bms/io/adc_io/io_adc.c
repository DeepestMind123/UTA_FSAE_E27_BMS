/**
 * @file io_adc.c
 * @author notwe
 * @date 2026-05-03
 * @brief abstract adc i/o source
 */

#include "io_adc.h"

adc_status_t IO_ADC_Init(io_adc_t *p_inst, const io_adc_cfg_t *p_cfg, const util_time_t *p_time_inst)
{
    adc_status_t status = ADC_STATUS_ERROR_NOT_INIT;

    if((p_inst != NULL) && (p_cfg != NULL) && (p_time_inst != NULL))
    {
        if((p_cfg->ADC_channel_select != NULL) &&
            (p_cfg->ADC_start != NULL) &&
            (p_cfg->ADC_done != NULL) &&
            (p_cfg->ADC_get_result != NULL) &&
            (p_cfg->ADC_stop != NULL))
        {
            p_inst->cfg = p_cfg;
            p_inst->time_inst = p_time_inst;

            p_inst->start_time = 0U;

            p_inst->set_offset = p_inst->cfg->adc_offset;

            p_inst->state = ADC_STATE_IDLE;

            p_inst->is_ready = false;

            p_inst->is_init = true;

            status = ADC_STATUS_OK;
        }
        else 
        {
            status = ADC_STATUS_ERROR_NULL_POINTER;
        }
        
    }
    else
    {
        status = ADC_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}

adc_status_t IO_ADC_Task(io_adc_t *p_inst)
{
    adc_status_t status = ADC_STATUS_OK;
    time_status_t time_status;
    uint32_t now_time = 0U;

    if((p_inst != NULL) && (p_inst->cfg != NULL) && (p_inst->time_inst != NULL))
    {
        if(p_inst->is_init)
        {
            if(p_inst->state >= ADC_STATE_MAX)
            {
                p_inst->state = ADC_STATE_UNDEFINED;
            }

            switch(p_inst->state)
            {
                case ADC_STATE_IDLE:
                {
                // indicates that the adc is not busy, intentionally left blank
                break;
                }

                case ADC_STATE_START:
                {
                    // select adc channel (defined in instance configuration in main.c)
                    p_inst->cfg->ADC_channel_select(p_inst->cfg->channel_id);

                    // start adc measurement on selected channel
                    p_inst->cfg->ADC_start();

                    p_inst->is_ready = false;

                    time_status = UTIL_Time_Get_Tick(p_inst->time_inst, &p_inst->start_time);

                    if(time_status == TIME_STATUS_OK)
                    {
                        p_inst->state = ADC_STATE_WAIT;
                    }
                    else
                    {
                        status = ADC_STATUS_ERROR_TIME_ERROR;

                        p_inst->state = ADC_STATE_ERROR;
                    }

                    break;
                }

                case ADC_STATE_WAIT:
                {
                    time_status = UTIL_Time_Get_Tick(p_inst->time_inst, &now_time);
                    
                    if(time_status == TIME_STATUS_OK)
                    {
                        if(now_time - p_inst->start_time <= p_inst->cfg->adc_timeout)
                        {
                            // wait for adc to finish measurement, comment out if selected mcu does not have support for this
                            ///*
                            if(p_inst->cfg->ADC_done()) 
                            {
                                p_inst->state = ADC_STATE_GET;
                            }
                            //*/
                        }
                        else
                        {
                            status = ADC_STATUS_ERROR_TIMEOUT;

                            p_inst->state = ADC_STATE_IDLE;
                        }
                    }
                    else
                    {
                        status = ADC_STATUS_ERROR_TIME_ERROR;

                        p_inst->state = ADC_STATE_ERROR;
                    }

                    break;
                }

                case ADC_STATE_GET:
                {
                    // stores measurement
                    p_inst->last_raw = p_inst->cfg->ADC_get_result();

                    p_inst->state = ADC_STATE_READY;

                    break;
                }

                case ADC_STATE_READY:
                {
                    p_inst->is_ready = true;

                    p_inst->state = ADC_STATE_IDLE;

                    break;
                }

                case ADC_STATE_ERROR:
                {
                    // indicates the adc has encountered an error, intentionally left blank
                    break;
                }

                case ADC_STATE_UNDEFINED:
                {
                    p_inst->state = ADC_STATE_ERROR;

                    status = ADC_STATUS_ERROR_UNDEFINED_STATE;

                    break;
                }

                default:
                {
                    /*no action required*/

                    break;
                }
    
            }
        }
        else
        {
            status = ADC_STATUS_ERROR_NOT_INIT;
        }

    }
    else 
    {
        status = ADC_STATUS_ERROR_NULL_POINTER;
    }

    return status;

}

adc_status_t IO_ADC_Start(io_adc_t *p_inst)
{
    adc_status_t status = ADC_STATUS_OK;

    if((p_inst != NULL))
    {
        if(p_inst->is_init)
        {
            p_inst->state = ADC_STATE_START;
        }
        else 
        {
            status = ADC_STATUS_ERROR_NOT_INIT;
        }
    }
    else 
    {
        status = ADC_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}

adc_status_t IO_ADC_Get_Val(io_adc_t *p_inst, uint16_t *p_out)
{
    adc_status_t status = ADC_STATUS_OK;

    uint32_t val = 0U;

    if((p_inst != NULL) && (p_out != NULL)) 
    {
        if(p_inst->is_init)
        {
            // stores adc measurement in temp value accessible by external functions
            val = p_inst->last_raw;
        }
        else 
        {
            status = ADC_STATUS_ERROR_NOT_INIT;
        }

        *p_out = val;
    }
    else
    {
        status = ADC_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}

adc_status_t IO_ADC_Get_Resolution(io_adc_t *p_inst, uint16_t *p_out)
{
    adc_status_t status = ADC_STATUS_OK;

    uint16_t resolution = 0U;

    if((p_inst != NULL) && (p_inst->cfg != NULL) && (p_out != NULL)) 
    {
        if(p_inst->is_init)
        {
            resolution = p_inst->cfg->adc_resolution;
        }
        else 
        {
            status = ADC_STATUS_ERROR_NOT_INIT;
        }

        *p_out = resolution;
    }
    else
    {
        status = ADC_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}

adc_status_t IO_ADC_Get_Offset(io_adc_t *p_inst, int16_t *p_out)
{
    adc_status_t status = ADC_STATUS_OK;

    int16_t offset = 0;

    if((p_inst != NULL) && (p_out != NULL))
    {
        if(p_inst->is_init)
        {
            offset = p_inst->set_offset;
        }
        else 
        {
            status = ADC_STATUS_ERROR_NOT_INIT;
        }

        *p_out = offset;
    } 
    else
    {
        status = ADC_STATUS_ERROR_NULL_POINTER;
    }

    return status;  
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
        }
    }
    else 
    {
        status = ADC_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}

adc_status_t IO_ADC_Get_Vref(io_adc_t *p_inst, uint16_t *p_out)
{
    adc_status_t status = ADC_STATUS_OK;

    uint16_t vref = 0U;

    if((p_inst != NULL) && (p_inst->cfg != NULL) && (p_out != NULL))
    {
        if(p_inst->is_init)
        {
            vref = p_inst->cfg->adc_vref_mV;
        }
        else 
        {
            status = ADC_STATUS_ERROR_NOT_INIT;
        }

        *p_out = vref;
    }
    else
    {
        status = ADC_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}

adc_status_t IO_ADC_Get_State(io_adc_t *p_inst, adc_state_t *p_out)
{
    adc_status_t status = ADC_STATUS_OK;

    adc_state_t state;

    if((p_inst != NULL) && (p_out != NULL))
    {
        if(p_inst->is_init)
        {
            state = p_inst->state;
        }
        else 
        {
            status = ADC_STATUS_ERROR_NOT_INIT;
        }

        *p_out = state;
    }
    else 
    {
        status = ADC_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}

adc_status_t IO_ADC_Get_Timeout(io_adc_t *p_inst, uint32_t *p_out)
{
    adc_status_t status = ADC_STATUS_OK;

    uint32_t timeout = 0U;

    if((p_inst != NULL) && (p_inst->cfg != NULL) && (p_out != NULL))
    {
        if(p_inst->is_init)
        {
            timeout = p_inst->cfg->adc_timeout;
        }
        else 
        {
            status = ADC_STATUS_ERROR_NOT_INIT;
        }

        *p_out = timeout;
    }
    else 
    {
        status = ADC_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}

adc_status_t IO_ADC_Get_Ready_Flag(io_adc_t *p_inst, bool *p_out)
{
    adc_status_t status = ADC_STATUS_OK;

    bool flag = false;

    if((p_inst != NULL) && (p_out != NULL))
    {
        if(p_inst->is_init)
        {
            flag = p_inst->is_ready;
        }
        else
        {
            status = ADC_STATUS_ERROR_NOT_INIT;
        }
    }
    else
    {
        status = ADC_STATUS_ERROR_NULL_POINTER;
    }

    *p_out = flag;

    return status;
}
