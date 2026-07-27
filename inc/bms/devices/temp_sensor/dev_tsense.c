/**
 * @file dev_temp_sensor.c
 * @author notwe
 * @date 2026-07-03
 * @brief generic temp sensor device driver wrapper source
 * @todo 
*/

#include "dev_tsense.h"

tsense_status_t DEV_Tsense_Init(tsense_t *p_inst, 
                                const tsense_cfg_t *p_cfg,
                                const tsense_func_t *p_func,
                                const adc_t *p_adc,
                                const util_time_t *p_time)
{
    tsense_status_t status = TSENSE_NOT_INIT;

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
        
        p_inst->state = TSENSE_STATE_IDLE;

        for(uint8_t i = 0U; i < SMALL_ARR_16; i++)
        {
            for(uint8_t j = 0U; j < LARGE_ARR_32; j++)
            {
                p_inst->sensor.tsense_raw_vals[i].temps_C[j] = 0.0f;

                p_inst->sensor.tsense_process_vals[i].temps_C[j] = 0.0f;
            }
        }

        if((p_inst->func->temp_start != NULL) &&
            (p_inst->func->temp_state != NULL) &&
            (p_inst->func->temp_stop != NULL) &&
            (p_inst->func->temp_get_result != NULL))
        {
            p_inst->is_init = true;
            
            status = TSENSE_OK;
        }
        else
        {
            status = TSENSE_NULL_FUNC;
        }
    }
    else
    {
        status = TSENSE_NULL_PTR;
    }

    return status;
}

tsense_status_t DEV_Tsense_Task(tsense_t *p_inst)
{
    tsense_status_t status = TSENSE_OK;
    adc_status_t adc_status;
    time_status_t time_status;

    uint32_t now_time = 0U;

    if((p_inst == NULL) ||
        (p_inst->sensor.cfg == NULL) ||
        (p_inst->func == NULL) ||
        (p_inst->time == NULL))
    {
        status = TSENSE_NULL_PTR;
    }

    if(status == TSENSE_OK)
    {
        if(p_inst->is_init)
        {
            if(!p_inst->sensor.cfg->gives_real_val)
            {
                adc_status = IO_ADC_Task(p_inst->adc);

                if(adc_status != ADC_OK)
                {
                    status = TSENSE_ADC_FAULT;
                }
            }

            if(p_inst->state >= TSENSE_STATE_MAX)
            {
                status = TSENSE_UNDEF_STATE;

                p_inst->state = TSENSE_STATE_ERROR;
            }

            switch(p_inst->state)
            {
                case TSENSE_STATE_IDLE:
                {
                    if(p_inst->start_time != 0)
                    {
                        time_status = UTIL_Time_Get_Tick(p_inst->time, &now_time);

                        if(time_status == TIME_STATUS_OK)
                        {
                            if(p_inst->tsense_timeout_ms)
                            {
                                if(now_time - p_inst->start_time >= p_inst->tsense_timeout_ms)
                                {
                                    status = TSENSE_TIMEOUT;

                                    p_inst->state = TSENSE_STATE_ERROR;
                                }
                            }
                        }
                        else
                        {
                            status = TSENSE_TIME_FAULT;

                            p_inst->state = TSENSE_STATE_ERROR;
                        }
                    }

                    break;
                }

                case TSENSE_STATE_START:
                {
                    p_inst->func->temp_start(&p_inst->sensor);

                    p_inst->state = TSENSE_STATE_WAIT;

                    break;
                }

                case TSENSE_STATE_WAIT:
                {
                    if(p_inst->func->temp_state(&p_inst->sensor))
                    {
                        p_inst->state = TSENSE_STATE_GET;
                    }

                    break;
                }

                case TSENSE_STATE_GET:
                {
                    if(p_inst->func->temp_get_result(&p_inst->sensor))
                    {
                        if(!p_inst->sensor.cfg->gives_real_val)
                        {
                            for(uint8_t i = 0U; i < p_inst->sensor.cfg->ic_num; i++)
                            {
                                for(uint8_t j = 0U; j < p_inst->sensor.cfg->temp_num; j++)
                                {
                                    status = DEV_Tsense_Process_Raw(p_inst, p_inst->sensor.tsense_raw_vals[i].temps_C[j], &p_inst->sensor.tsense_process_vals[i].temps_C[j]);

                                    if(status != TSENSE_OK)
                                    {
                                        p_inst->state = TSENSE_STATE_ERROR;
                                    }       
                                }
                            }

                            p_inst->state = TSENSE_STATE_READY;
                        }
                        else
                        {
                            for(uint8_t i = 0U; i < p_inst->sensor.cfg->ic_num; i++)
                            {
                                for(uint8_t j = 0U; j < p_inst->sensor.cfg->temp_num; j++)
                                {
                                    p_inst->sensor.tsense_process_vals[i].temps_C[j] = 
                                    p_inst->sensor.tsense_raw_vals[i].temps_C[j];    
                                }
                            }

                            p_inst->state = TSENSE_STATE_READY;
                        }
                    }
                    else
                    {
                        status = TSENSE_CONVERSION_FAIL;

                        p_inst->state = TSENSE_STATE_READY;
                    }
                    
                    break;
                }

                case TSENSE_STATE_READY:
                {
                    p_inst->is_ready = true;

                    p_inst->state = TSENSE_STATE_IDLE;

                    break;
                }

                case TSENSE_STATE_ERROR:
                {
                    /*intentionally left blank*/
                    break;
                }

                default:
                {
                    p_inst->state = TSENSE_STATE_ERROR;

                    break;
                }
            }
        }
        else
        {
            status = TSENSE_NOT_INIT;
        }
    }

    return status;
}

tsense_status_t DEV_Tsense_Start(tsense_t *p_inst)
{
    tsense_status_t status = TSENSE_OK;

    if(p_inst != NULL)
    {
        if(p_inst->is_init)
        {
            tsense_state_t state = TSENSE_STATE_ERROR;

            status = DEV_Tsense_Get_State(p_inst, &state);

            if(status == TSENSE_OK)
            {
                if(state == TSENSE_STATE_IDLE)
                {
                    p_inst->state = TSENSE_STATE_START;
                }
                else
                {
                    status = TSENSE_MISMATCH_STATE;
                }
            }
        }
        else
        {
            status = TSENSE_NOT_INIT;
        }
    }
    else
    {
        status = TSENSE_NULL_PTR;
    }

    return status;
}

tsense_status_t DEV_Tsense_Process_Raw(tsense_t *p_inst, uint16_t p_in, float *p_out)
{
    tsense_status_t status = TSENSE_OK;

    if((p_inst != NULL) && (p_inst->sensor.cfg != NULL) && 
        (p_inst->adc != NULL) && (p_out != NULL))
    {
        if(p_inst->is_init)
        {
            uint16_t raw = p_in;

            uint16_t vref = 0U;

            uint16_t resolution = 0U;

            int16_t offset = 0;

            float temp_C = (float)raw;

            if(status == TSENSE_OK)
            {
                if(IO_ADC_Get_Vref(p_inst->adc, &vref) != ADC_OK)
                {
                    status = TSENSE_ADC_FAULT;
                } 
            }

            if(status == TSENSE_OK)
            {
                if(IO_ADC_Get_Resolution(p_inst->adc, &resolution) != ADC_OK)
                {
                    status = TSENSE_ADC_FAULT;
                } 
            }

            if(status == TSENSE_OK)
            {
                if(IO_ADC_Get_Offset(p_inst->adc, &offset) != ADC_OK)
                {
                    status = TSENSE_ADC_FAULT;
                } 
            }

            if(raw >= vref)
            {
                temp_C = -999.0f;
            }
            else
            {
                float r_temp = (float)p_inst->sensor.cfg->temp_rs * 
                                ((float)raw / ((float)vref - (float)raw));
                float temp_K = r_temp / (float)p_inst->sensor.cfg->temp_rn;
                temp_K = log(temp_K);
                temp_K /= (float)p_inst->sensor.cfg->beta;
                temp_K += 1.0 / p_inst->sensor.cfg->temp_nom_K;
                temp_K = 1.0 / temp_K;

                temp_C = temp_K - 273.15;
            }

            *p_out = temp_C;
        }
        else
        {
            status = TSENSE_NOT_INIT;
        }
    }
    else
    {
        status = TSENSE_NULL_PTR;
    }

    return status;
}

tsense_status_t DEV_Tsense_Get_Val(tsense_t *p_inst, tsense_val_t (*p_out)[SMALL_ARR_16])
{
    tsense_status_t status = TSENSE_OK;

    if((p_inst != NULL) && (p_inst->sensor.cfg != NULL) && (p_out != NULL))
    {
        if(p_inst->is_init)
        {
            for(uint8_t i = 0U; i < p_inst->sensor.cfg->ic_num; i++)
            {
                (*p_out)[i] = p_inst->sensor.tsense_process_vals[i];
            }
        }
        else
        {
            status = TSENSE_NOT_INIT;
        }
    }
    else
    {
        status = TSENSE_NULL_PTR;
    }

    return status;
}

tsense_status_t DEV_Tsense_Get_State(tsense_t *p_inst, tsense_state_t *p_out)
{
    tsense_status_t status = TSENSE_OK;

    tsense_state_t state = TSENSE_STATE_ERROR;

    if((p_inst != NULL) && (p_out))
    {
        if(p_inst->is_init)
        {
            state = p_inst->state;
        }

        *p_out = state;
    }
    else
    {
        status = TSENSE_NULL_PTR;
    }

    return status;
}

tsense_status_t DEV_Tsense_Get_Ready_Flag(tsense_t *p_inst, bool *p_out)
{
    tsense_status_t status = TSENSE_OK;

    bool flag = false;

    if((p_inst != NULL) && (p_out))
    {
        if(p_inst->is_init)
        {
            flag = p_inst->is_ready;
        }
        else
        {
            status = TSENSE_NOT_INIT;
        }

        *p_out = flag;
    }
    else
    {
        status = TSENSE_NULL_PTR;
    }

    return status;
}