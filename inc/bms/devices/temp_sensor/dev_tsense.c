/**
 * @file dev_temp_sensor.c
 * @author notwe
 * @date 2026-07-03
 * @brief generic temp sensor device driver wrapper source
 * @todo 
*/

#include "dev_tsense.h"

tsense_status_t DEV_Tsense_Init(tsense_t *p_inst, 
                                const tsense_cfg_t *p_cfg)
{
    tsense_status_t status = TSENSE_NOT_INIT;

    if((p_inst != NULL) && (p_cfg != NULL))
    {
        if(!p_inst->is_init)
        {
            if((p_cfg->adc_cfg != NULL) &&
            (p_cfg->time_cfg != NULL) &&
            (p_cfg->func_cfg != NULL) &&
            (p_cfg->init_ctx != NULL))
            {
                p_inst->p_func = p_cfg->func_cfg;
                p_inst->p_adc = p_cfg->adc_cfg;
                p_inst->p_time = p_cfg->time_cfg;
                p_inst->p_ctx = p_cfg->init_ctx;

                if((p_inst->p_func->temp_start != NULL) &&
                (p_inst->p_func->temp_state != NULL) &&
                (p_inst->p_func->temp_get_result != NULL))
                {
                    p_inst->is_ready = false;

                    p_inst->start_time = 0U;
                    
                    p_inst->state = TSENSE_STATE_IDLE;
                    p_inst->status = TSENSE_OK;

                    for(uint8_t i = 0U; i < SMALL_ARR_16; i++)
                    {
                        for(uint8_t j = 0U; j < LARGE_ARR_32; j++)
                        {
                            p_inst->sensor.tsense_raw_vals[i].temps_C[j] = 0.0f;

                            p_inst->sensor.tsense_process_vals[i].temps_C[j] = 0.0f;
                        }
                    }

                    if((p_inst->p_ctx->ic_num > SMALL_ARR_16) || (p_inst->p_ctx->temp_num > LARGE_ARR_32))
                    {
                        status = TSENSE_INVALID_CFG;
                    }
                    else
                    {
                        p_inst->is_init = true;
                        status = TSENSE_OK;
                    }
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
        }
        else
        {
            status = TSENSE_DBL_INIT;
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

    if(p_inst == NULL)
    {
        status = TSENSE_NULL_PTR;
    }

    if(status == TSENSE_OK)
    {
        if(p_inst->is_init)
        {
            if(!p_inst->p_ctx->gives_real_val)
            {
                adc_status = IO_ADC_Task(p_inst->p_adc);

                if(adc_status != ADC_OK)
                {
                    p_inst->status = TSENSE_ADC_FAULT;

                    p_inst->state = TSENSE_STATE_ERROR;
                }
            }

            if((p_inst->state >= TSENSE_STATE_MAX) || (p_inst->state <= TSENSE_STATE_UNDEF))
            {
                p_inst->status = TSENSE_UNDEF_STATE;

                p_inst->state = TSENSE_STATE_ERROR;
            }

            switch(p_inst->state)
            {
                case TSENSE_STATE_IDLE:
                {
                    /* intentionally left blank*/

                    break;
                }

                case TSENSE_STATE_START:
                {
                    time_status = UTIL_Time_Get_Tick(p_inst->p_time, &now_time);

                    if(time_status == TIME_OK)
                    {
                        p_inst->start_time = now_time;

                        if(p_inst->p_func->temp_start(&p_inst->sensor))
                        {
                            p_inst->state = TSENSE_STATE_WAIT;
                        }
                        else
                        {
                                p_inst->status = TSENSE_CONVERSION_FAIL;

                                p_inst->state = TSENSE_STATE_ERROR;
                        }
                    }
                    else
                    {
                        p_inst->status = TSENSE_TIME_FAULT;

                        p_inst->state = TSENSE_STATE_ERROR;   
                    }

                    break;
                }

                case TSENSE_STATE_WAIT:
                {
                    time_status = UTIL_Time_Get_Tick(p_inst->p_time, &now_time);

                    if(time_status == TIME_OK)
                    {
                        if((now_time - p_inst->start_time <= p_inst->p_ctx->timeout_ms) || (p_inst->p_ctx->timeout_ms == 0U))
                        {
                            if(p_inst->p_func->temp_state(&p_inst->sensor))
                            {
                                p_inst->start_time = now_time;

                                p_inst->state = TSENSE_STATE_GET;
                            }
                        }
                        else
                        {
                            p_inst->status = TSENSE_TIMEOUT;

                            p_inst->state = TSENSE_STATE_ERROR;
                        }
                    }
                    else
                    {
                        p_inst->status = TSENSE_TIME_FAULT;

                        p_inst->state = TSENSE_STATE_ERROR;
                    }

                    break;
                }

                case TSENSE_STATE_GET:
                {
                    p_inst->state = TSENSE_STATE_READY;

                    time_status = UTIL_Time_Get_Tick(p_inst->p_time, &now_time);

                    if(time_status == TIME_OK)
                    {
                        if((now_time - p_inst->start_time <= p_inst->p_ctx->timeout_ms) || (p_inst->p_ctx->timeout_ms == 0U))
                        {
                            if(p_inst->p_func->temp_get_result(&p_inst->sensor))
                            {
                                if(!p_inst->p_ctx->gives_real_val)
                                {
                                    for(uint8_t i = 0U; i < p_inst->p_ctx->ic_num; i++)
                                    {
                                        for(uint8_t j = 0U; j < p_inst->p_ctx->temp_num; j++)
                                        {
                                            p_inst->status = DEV_Tsense_Process_Raw(p_inst, p_inst->sensor.tsense_raw_vals[i].temps_C[j], 
                                                                                    &p_inst->sensor.tsense_process_vals[i].temps_C[j]);

                                            if(p_inst->status != TSENSE_OK)
                                            {
                                                p_inst->state = TSENSE_STATE_ERROR;

                                                break;
                                            }       
                                        }

                                        if(p_inst->status != TSENSE_OK)
                                        {
                                            break;
                                        }
                                    }
                                }
                                else
                                {
                                    for(uint8_t i = 0U; i < p_inst->p_ctx->ic_num; i++)
                                    {
                                        for(uint8_t j = 0U; j < p_inst->p_ctx->temp_num; j++)
                                        {
                                            p_inst->sensor.tsense_process_vals[i].temps_C[j] = 
                                            p_inst->sensor.tsense_raw_vals[i].temps_C[j];    
                                        }
                                    }
                                }
                            }
                            else
                            {
                                p_inst->status = TSENSE_CONVERSION_FAIL;

                                p_inst->state = TSENSE_STATE_ERROR;
                            }
                        }
                        else
                        {
                            p_inst->status = TSENSE_TIMEOUT;

                            p_inst->state = TSENSE_STATE_ERROR;
                        }
                    }
                    else
                    {
                        p_inst->status = TSENSE_TIME_FAULT;

                        p_inst->state = TSENSE_STATE_ERROR;
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
                    if(p_inst->status == TSENSE_OK)
                    {
                        p_inst->status = TSENSE_UNKNOWN_ERROR;
                    }

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
            p_inst->status = TSENSE_NOT_INIT;
        }
    }

    if(p_inst != NULL)
    {
        status = p_inst->status;
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
            if(p_inst->state == TSENSE_STATE_IDLE)
            {
                p_inst->state = TSENSE_STATE_START;
            }
            else if((p_inst->state >= TSENSE_STATE_MAX) || (p_inst->state <= TSENSE_STATE_UNDEF))
            {
                status = TSENSE_UNDEF_STATE;
            }
            else
            {
                status = TSENSE_BUSY;
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

tsense_status_t DEV_Tsense_Process_Raw(tsense_t *p_inst, float p_in, float *p_out)
{
    tsense_status_t status = TSENSE_OK;

    adc_status_t adc_status;

    float temp_C = 0.0f;

    if((p_inst != NULL) && (p_out != NULL))
    {
        if(p_inst->is_init)
        {
            float raw = p_in;

            float vref = 0.f;

            float resolution = 0.0f;

            float offset = 0.0f;

            temp_C = (float)raw;

            adc_status = IO_ADC_Get_Vref(p_inst->p_adc, &vref);

            if(adc_status != ADC_OK)
            {
                status = TSENSE_ADC_FAULT;
            }

            adc_status = IO_ADC_Get_Resolution(p_inst->p_adc, &resolution);

            if(adc_status != ADC_OK)
            {
                status = TSENSE_ADC_FAULT;
            }

            if(status == TSENSE_OK)
            {
                if((resolution > 0U) || (vref == 0))
                {
                    if(raw >= vref)
                    {
                        temp_C = -999.0f;

                        status = TSENSE_INVALID_CFG;
                    }
                    else
                    {


                        if((p_inst->p_ctx->temp_rn > 0.0f) 
                            && (p_inst->p_ctx->beta > 0.0f) 
                            && (p_inst->p_ctx->temp_nom_K > 0.0f)
                            && (p_inst->p_ctx->temp_rs > 0.0f))
                        {
                            float r_temp = (float)p_inst->p_ctx->temp_rs * 
                                ((float)raw / ((float)vref - (float)raw));
                            float temp_K = r_temp / (float)p_inst->p_ctx->temp_rn;
                            temp_K = log(temp_K);
                            temp_K /= (float)p_inst->p_ctx->beta;
                            temp_K += 1.0f / (float)p_inst->p_ctx->temp_nom_K;
                            temp_K = 1.0f / temp_K;
                            temp_C = temp_K - 273.15f; /*ugh this is a magic number but whatev*/
                        }
                        else
                        {
                            status = TSENSE_INVALID_CFG;
                        }
                    }
                }
                else
                {
                    status = TSENSE_INVALID_CFG;
                }
            }
        }
        else
        {
            status = TSENSE_NOT_INIT;
        }

        *p_out = temp_C;
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

    if((p_inst != NULL) && (p_out != NULL))
    {
        if(p_inst->is_init)
        {
            for(uint8_t i = 0U; i < p_inst->p_ctx->ic_num; i++)
            {
                (*p_out)[i] = p_inst->sensor.tsense_process_vals[i];
            }

            p_inst->is_ready = false;
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
            if((p_inst->state >= TSENSE_STATE_MAX) || (p_inst->state <= TSENSE_STATE_UNDEF))
            {
                status = TSENSE_UNDEF_STATE;
            }

            state = p_inst->state;
        }
        else
        {
            status = TSENSE_NOT_INIT;
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