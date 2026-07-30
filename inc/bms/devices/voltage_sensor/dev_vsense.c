/**
 * @file dev_voltage_sensor.c
 * @author notwe
 * @date 2026-07-03
 * @brief generic voltage sensor device driver wrapper source
 * @todo 
*/

#include "dev_vsense.h"

vsense_status_t DEV_Vsense_Init(vsense_t *p_inst,
                                const vsense_cfg_t *p_cfg)
{
    vsense_status_t status = VSENSE_NOT_INIT;

    if((p_inst != NULL) &&
        (p_cfg != NULL))
    {
        p_inst->p_func = p_cfg->func_cfg;
        p_inst->p_adc = p_cfg->adc_cfg;
        p_inst->p_time = p_cfg->time_cfg;
        p_inst->p_ctx = p_cfg->init_ctx;

        p_inst->is_ready = false;
        p_inst->val_diff = false;
        p_inst->is_balance = false;

        p_inst->state = VSENSE_STATE_IDLE;

        for(uint8_t i = 0U; i < SMALL_ARR_16; i++)
        {
            for(uint8_t j = 0U; j < LARGE_ARR_32; j++)
            {
                p_inst->sensor.vsense_raw_vals[i].cells_mV[j] = 0;
            
                p_inst->sensor.vsense_process_vals[i].cells_mV[j] = 0;
            }
        }

        if((p_inst->p_func->voltage_start_open_wire != NULL) &&
            (p_inst->p_func->voltage_start_closed_wire != NULL) &&
            (p_inst->p_func->vsense_get_result != NULL))
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
    adc_status_t adc_status;
    time_status_t time_status;
    uint32_t now_time = 0U;

    if(p_inst != NULL)
    {
        if(p_inst->is_init)
        {
            if(!p_inst->p_ctx->gives_real_val)
            {
                adc_status = IO_ADC_Task(p_inst->p_adc);

                if(adc_status != ADC_OK)
                {
                    status = VSENSE_ADC_FAULT;
                }
            }

            if((p_inst->state >= VSENSE_STATE_MAX) || (p_inst->state == VSENSE_STATE_UNDEF))
            {
                status = VSENSE_UNDEF_STATE;

                p_inst->state = VSENSE_STATE_ERROR;
            }

            switch(p_inst->state)
            {
                case VSENSE_STATE_IDLE:
                {
                    /* intentionally left blank*/

                    break;
                }

                case VSENSE_STATE_START:
                {
                    if(p_inst->is_balance)
                    {
                        if(p_inst->p_func->voltage_start_open_wire(&p_inst->sensor))
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
                        if(p_inst->p_func->voltage_start_closed_wire(&p_inst->sensor))
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
                    time_status = UTIL_Time_Get_Tick(p_inst->p_time, &now_time);

                    if(time_status == TIME_STATUS_OK)
                    {
                        if((now_time - p_inst->start_time <= p_inst->p_ctx->timeout_ms) || (p_inst->p_ctx->timeout_ms == 0U))
                        {
                            if(p_inst->p_func->voltage_state(&p_inst->sensor))
                            {
                                p_inst->start_time = now_time;

                                p_inst->state = VSENSE_STATE_GET;
                            }
                        }
                        else
                        {
                            status = VSENSE_TIMEOUT;

                            p_inst->state = VSENSE_STATE_ERROR;
                        }
                    }
                    else
                    {
                        status = VSENSE_TIMEOUT;

                        p_inst->state = VSENSE_STATE_ERROR;
                    }

                    break;
                }

                case VSENSE_STATE_GET:
                {
                    time_status = UTIL_Time_Get_Tick(p_inst->p_time, &now_time);

                    if(time_status == TIME_STATUS_OK)
                    {
                        if((now_time - p_inst->start_time <= p_inst->p_ctx->timeout_ms) || (p_inst->p_ctx->timeout_ms == 0U))
                        {
                            if(p_inst->p_func->vsense_get_result(&p_inst->sensor))
                            {
                                if(!p_inst->p_ctx->gives_real_val)
                                {
                                    for(uint8_t i = 0U; i < p_inst->p_ctx->ic_num; i++)
                                    {
                                        for(uint8_t j = 0U; j < p_inst->p_ctx->cell_num; j++)
                                        {
                                            status = DEV_Vsense_Process_Raw(p_inst, p_inst->sensor.vsense_raw_vals[i].cells_mV[j],
                                                                                    &p_inst->sensor.vsense_process_vals[i].cells_mV[j]);

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
                                    for(uint8_t i = 0U; i < p_inst->p_ctx->ic_num; i++)
                                    {
                                        for(uint8_t j = 0U; j < p_inst->p_ctx->cell_num; j++)
                                        {
                                            p_inst->sensor.vsense_process_vals[i].cells_mV[j] = 
                                            p_inst->sensor.vsense_raw_vals[i].cells_mV[j];
                                        }
                                    }
                                    
                                    p_inst->state = VSENSE_STATE_READY;

                                }
                            }
                            else
                            {
                                status = VSENSE_CONVERSION_FAIL;

                                p_inst->state = VSENSE_STATE_ERROR;
                            }
                        }
                        else
                        {
                            status = VSENSE_TIMEOUT;

                            p_inst->state = VSENSE_STATE_ERROR;
                        }
                    }
                    else
                    {
                        status = VSENSE_TIME_FAULT;

                        p_inst->state = VSENSE_STATE_ERROR;
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
                    p_inst->state = VSENSE_STATE_ERROR;

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
            if(p_inst->state == VSENSE_STATE_IDLE)
            {
                p_inst->state = VSENSE_STATE_START;
            }
            else
            {
                status = VSENSE_MISMATCH_STATE;
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

vsense_status_t DEV_Vsense_Process_Raw(vsense_t *p_inst, uint16_t val, uint16_t *p_out)
{
    vsense_status_t status = VSENSE_OK;

    adc_status_t adc_status;

    uint16_t voltage_mV = val;

    if((p_inst != NULL) && (p_out != NULL))
    {
        if(p_inst->is_init)
        {
            uint16_t vref = 0U;

            uint16_t resolution = 0U;

            int16_t offset = 0;

            adc_status = IO_ADC_Get_Vref(p_inst->p_adc, &vref);

            if(adc_status != ADC_OK)
            {
                status = VSENSE_ADC_FAULT;
            }

            adc_status = IO_ADC_Get_Resolution(p_inst->p_adc, &resolution);

            if(adc_status != ADC_OK)
            {
                status = VSENSE_ADC_FAULT;
            }

            adc_status = IO_ADC_Get_Offset(p_inst->p_adc, &offset);
            
            if(adc_status != ADC_OK)
            {
                status = VSENSE_ADC_FAULT;
            }

            if((resolution > 0U))
            {
                uint16_t voltage_mV = ((uint16_t)val * (uint16_t)vref) / (uint16_t)resolution;

                voltage_mV -= (uint16_t)offset;
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

vsense_status_t DEV_Vsense_Get_Val(vsense_t *p_inst, vsense_val_t (*p_out)[SMALL_ARR_16])
{
    vsense_status_t status = VSENSE_OK;

    if((p_inst != NULL) && (p_out != NULL))
    {
        if(p_inst->is_init)
        {
            for(uint8_t i = 0U; i < p_inst->p_ctx->ic_num; i++)
            {
                (*p_out)[i] = p_inst->sensor.vsense_process_vals[i];
            }

            p_inst->is_ready = false;
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

    if((p_inst != NULL) && (p_out != NULL))
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

    if((p_inst != NULL) && (p_out))
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