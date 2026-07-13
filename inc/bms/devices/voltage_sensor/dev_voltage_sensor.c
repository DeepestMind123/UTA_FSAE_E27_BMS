/**
 * @file dev_voltage_sensor.c
 * @author notwe
 * @date 2026-07-03
 * @brief generic voltage sensor device driver wrapper source
 * @todo 
*/

#include "dev_voltage_sensor.h"

vsense_status_t DEV_Vsense_Init(vsense_t *p_inst,
                                                const vsense_cfg_t *p_cfg,
                                                const vsense_func_t *p_func, 
                                                const io_adc_t *p_adc,
                                                const util_time_t *p_time)
{
    vsense_status_t status = VSENSE_NOT_INIT;

    if((p_inst != NULL) &&
        (p_adc != NULL) &&
        (p_time != NULL) &&
        (p_func != NULL) &&
        (p_cfg != NULL))
    {
        p_inst->func = p_func;
        p_inst->adc = p_adc;
        p_inst->time = p_time;
        p_inst->sensor.cfg = p_cfg;

        p_inst->is_ready = false;
        p_inst->val_diff = false;
        p_inst->is_balance = false;

        p_inst->state = VSENSE_STATE_IDLE;

        for(uint8_t i = 0U; i < SMALL_ARR_32; i++)
        {
            for(uint8_t j = 0U; j < LARGE_ARR_64; j++)
            {
                p_inst->sensor.sensor_raw_vals[i].cells_mV[j] = 0;
            
                p_inst->sensor.sensor_process_vals[i].cells_mV[j] = 0;
            }
        }

        if((p_inst->func->voltage_start_open_wire != NULL) &&
            (p_inst->func->voltage_start_closed_wire != NULL) &&
            (p_inst->func->vsense_get_result != NULL))
        {
            p_inst->is_init = true;

            status = VSENSE_OK;
        }
        else
        {
            status = VSENSE_NULL_FUNC;
        }

    }
    else
    {
        status = VSENSE_NULL_PTR;
    }

    return status;
}

vsense_status_t DEV_Vsense_Task(vsense_t *p_inst)
{
    vsense_status_t status = VSENSE_OK;

    if((p_inst != NULL) &&
        (p_inst->sensor.cfg != NULL) &&
        (p_inst->func != NULL) &&
        (p_inst->time != NULL))
    {
        if(p_inst->is_init)
        {
            if(p_inst->state >= VSENSE_STATE_MAX)
            {
                status = VSENSE_UNDEF_STATE;

                p_inst->state = VSENSE_STATE_ERROR;
            }

            switch(p_inst->state)
            {
                case VSENSE_STATE_IDLE:
                {
                    break;
                }

                case VSENSE_STATE_START:
                {
                    if(p_inst->is_balance)
                    {
                        if(p_inst->func->voltage_start_open_wire(&p_inst->sensor))
                        {
                            p_inst->state = VSENSE_STATE_WAIT;
                        }
                        else
                        {
                            status = VSENSE_CONVERSION_FAIL;

                            p_inst->state = VSENSE_STATE_ERROR;
                        }
                    }
                    else
                    {
                        if(p_inst->func->voltage_start_closed_wire(&p_inst->sensor))
                        {
                            p_inst->state = VSENSE_STATE_WAIT;
                        }
                        else
                        {
                            status = VSENSE_CONVERSION_FAIL;

                            p_inst->state = VSENSE_STATE_ERROR;
                        }
                    }

                    break;
                }

                case VSENSE_STATE_WAIT:
                {
                    if(p_inst->func->voltage_state(&p_inst->sensor))
                    {
                        p_inst->state = VSENSE_STATE_GET;
                    }

                    break;
                }

                case VSENSE_STATE_GET:
                {
                    if(p_inst->func->vsense_get_result(&p_inst->sensor))
                    {
                        if(!p_inst->sensor.cfg->gives_real_val)
                        {
                            for(uint8_t i = 0U; i < p_inst->sensor.cfg->ic_num; i++)
                            {
                                for(uint8_t j = 0U; j < p_inst->sensor.cfg->cell_num; j++)
                                {
                                    status = DEV_Vsense_Process_Raw(p_inst, p_inst->sensor.sensor_raw_vals[i].cells_mV[j],
                                                                             p_inst->sensor.sensor_process_vals[i].cells_mV[j]);

                                    if(status != VSENSE_OK)
                                    {
                                        p_inst->state = VSENSE_STATE_ERROR;
                                    }
                                }
                            }
                            
                            p_inst->state = VSENSE_STATE_READY;

                        }
                        else
                        {
                            for(uint8_t i = 0U; i < p_inst->sensor.cfg->ic_num; i++)
                            {
                                for(uint8_t j = 0U; j < p_inst->sensor.cfg->cell_num; j++)
                                {
                                    p_inst->sensor.sensor_raw_vals[i].cells_mV[j] =
                                    p_inst->sensor.sensor_process_vals[i].cells_mV[j];
                                }
                            }
                            
                            p_inst->state = VSENSE_STATE_READY;

                        }
                    }
                    else
                    {
                        status = VSENSE_CONVERSION_FAIL;

                        p_inst->state = VSENSE_STATE_READY;
                    }
                    
                    break;
                }

                case VSENSE_STATE_READY:
                {
                    p_inst->is_ready = true;

                    p_inst->state = VSENSE_STATE_IDLE;

                    break;
                }

                case VSENSE_STATE_ERROR:
                {
                    /*intentionally left blank*/
                    break;
                }

                default:
                {
                    /*intentionally left blank*/
                    break;
                }
            }
        }
        else
        {
            status = VSENSE_NOT_INIT;
        }
    }
    else
    {
        status = VSENSE_NULL_PTR;
    }

    return status;
}

vsense_status_t DEV_Vsense_Start(vsense_t *p_inst)
{
    vsense_status_t status = VSENSE_OK;

    if((p_inst != NULL))
    {
        if(p_inst->is_init)
        {
            vsense_state_t state = VSENSE_STATE_ERROR;

            status = DEV_Vsense_Get_State(p_inst, &state);

            if(status == VSENSE_OK)
            {
                if(state == VSENSE_STATE_IDLE)
                {
                    p_inst->state = ADC_STATE_START;
                }
                else
                {
                    status = VSENSE_MISMATCH_STATE;
                }
            }
        }
        else
        {
            status = VSENSE_NOT_INIT;
        }
    }
    else
    {
        status = VSENSE_NULL_PTR;
    }

    return status;
}

vsense_status_t DEV_Vsense_Process_Raw(vsense_t *p_inst, int32_t val, uint16_t *p_out)
{
    vsense_status_t status = VSENSE_OK;

    adc_status_t adc_status;

    uint16_t voltage_mV = val;

    if((p_inst != NULL) && (p_inst->sensor.cfg != NULL ) && (p_inst->adc != NULL) && (p_out != NULL))
    {
        if(p_inst->is_init)
        {
            uint16_t raw = 0U;

            uint16_t vref = 0U;

            uint16_t resolution = 0U;

            int16_t offset = 0;

            adc_status = IO_ADC_Get_Val(p_inst->adc, &raw);

            if(adc_status != ADC_STATUS_OK)
            {
                status = VSENSE_ADC_FAULT;
            }

            adc_status = IO_ADC_Get_Vref(p_inst->adc, &vref);

            if(adc_status != ADC_STATUS_OK)
            {
                status = VSENSE_ADC_FAULT;
            }

            adc_status = IO_ADC_Get_Resolution(p_inst->adc, &resolution);

            if(adc_status != ADC_STATUS_OK)
            {
                status = VSENSE_ADC_FAULT;
            }

            adc_status = IO_ADC_Get_Offset(p_inst->adc, &offset);
            
            if(adc_status != ADC_STATUS_OK)
            {
                status = VSENSE_ADC_FAULT;
            }

            if((resolution > 0U))
            {
                int16_t voltage_mV = ((int16_t)raw * (int16_t)vref) / (int16_t)resolution;

                voltage_mV -= (int16_t)offset;
            }
        }
        else
        {
            status = VSENSE_NOT_INIT;
        }

        *p_out = voltage_mV;

    }
    else
    {
        status = VSENSE_NULL_PTR;
    }

    return status;
}

vsense_status_t DEV_Vsense_Set_Balance(vsense_t *p_inst, bool val)
{
    vsense_status_t status = VSENSE_OK;

    if((p_inst != NULL))
    {
        if(p_inst->is_init)
        {
            p_inst->is_balance = val;
        }
        else
        {
            status = VSENSE_NOT_INIT;
        }
    }
    else
    {
        status = VSENSE_NULL_PTR;
    }

    return status;
}

vsense_status_t DEV_Vsense_Get_Val(vsense_t *p_inst, vsense_val_t (*p_out)[SMALL_ARR_32])
{
    vsense_status_t status = VSENSE_OK;

    if((p_inst != NULL) && (p_inst->sensor.cfg != NULL) && (p_out != NULL))
    {
        if(p_inst->is_init)
        {
            for(uint8_t i = 0U; i < p_inst->sensor.cfg->ic_num; i++)
            {
                (*p_out)[i] = p_inst->sensor.sensor_process_vals[i];
            }
        }
        else
        {
            status = VSENSE_NOT_INIT;
        }
    }
    else
    {
        status = VSENSE_NULL_PTR;
    }

    return status;
}

vsense_status_t DEV_Vsense_Get_State(vsense_t *p_inst, vsense_state_t *p_out)
{
    vsense_status_t status = VSENSE_OK;

    vsense_state_t state = VSENSE_STATE_ERROR;

    if(p_inst != NULL)
    {
        if(p_inst->is_init)
        {
            state = p_inst->state;
        }
        else
        {
            status = VSENSE_NOT_INIT;
        }

        *p_out = state;

    }
    else
    {
        status = VSENSE_NULL_PTR;
    }

    return status;
}

vsense_status_t DEV_Vsense_Get_Ready_Flag(vsense_t *p_inst, bool *p_out)
{
    vsense_status_t status = VSENSE_OK;

    bool flag = false;

    if(p_inst != NULL)
    {
        if(p_inst->is_init)
        {
            flag = p_inst->is_ready;
        }
        else
        {
            status = VSENSE_NOT_INIT;
        }

        *p_out = flag;

    }
    else
    {
        status = VSENSE_NULL_PTR;
    }

    return status;
}