/**
 * @file DEV_current.h
 * @author notwe
 * @date 2026-05-03
 * @brief current sensor device driver source
 */

#include "dev_current.h"

current_status_t DEV_Current_Init(dev_current_t *p_inst, dev_current_cfg_t *p_cfg, io_adc_t *p_adc_inst)
{
    current_status_t status = CURRENT_STATUS_OK;

    if((p_inst != NULL) && (p_cfg != NULL) && (p_adc_inst != NULL))
    {
        p_inst->cfg = p_cfg;
        p_inst->adc_inst = p_adc_inst;

        p_inst->cal_acc = 0U;
        p_inst->cal_count = 0U;
        p_inst->raw_offset = 0U;

        p_inst->start_time = 0U;

        p_inst->is_init = true;
    }
    else
    {
        status = CURRENT_STATUS_ERROR_NULL_POINTER;

        BMS_Fault_Update(BMS_FAULT_NULL_POINTER, false);
    }

    return status;
}

current_status_t DEV_Current_Task(dev_current_t *p_inst)
{
    current_status_t status = CURRENT_STATUS_OK;

    if((p_inst != NULL) && (p_inst->cfg != NULL) && (p_inst->adc_inst != NULL))
    {
        if(p_inst->is_init)
        {
            switch(p_inst->state)
            {
                case CURRENT_STATE_IDLE:

                uint32_t now_time = UTIL_Time_Get_Tick();

                if(now_time - p_inst->start_time <= p_inst->cfg->current_timeout)
                { 
                    if(now_time - p_inst->start_time >= p_inst->cfg->current_wait_ms)
                    {
                        p_inst->state = CURRENT_STATE_START;
                    }
                }
                else
                {
                    BMS_Fault_Update(BMS_FAULT_CURRENT_TIMEOUT, true);

                    status = CURRENT_STATUS_ERROR_TIMEOUT;
                }

                break;

                case CURRENT_STATE_START:

                adc_state_t adc_state = IO_ADC_Get_State(p_inst->adc_inst);

                if(adc_state == ADC_STATE_IDLE)
                {
                    if(IO_ADC_Start(p_inst->adc_inst) == ADC_STATUS_OK)
                    {
                        p_inst->state = CURRENT_STATE_WAIT;
                    }
                    else 
                    {
                        status = CURRENT_STATUS_ERROR_ADC_ERROR;
                    }
                }
                else 
                {
                    status = CURRENT_STATUS_ERROR_ADC_ERROR;

                    BMS_Fault_Update(BMS_FAULT_TIMING_LAPSE, true);
                }

                break;

                case CURRENT_STATE_WAIT:

                if(IO_ADC_Get_State(p_inst->adc_inst) != ADC_STATE_READY)
                {
                    if(IO_ADC_Task(p_inst->adc_inst) != ADC_STATUS_OK)
                    {
                        status = CURRENT_STATUS_ERROR_ADC_ERROR;
                    }
                }
                else 
                {
                    p_inst->state = CURRENT_STATE_GET;
                }

                break;

                case CURRENT_STATE_GET:

                if(IO_ADC_Get_State(p_inst->adc_inst) == ADC_STATE_READY)
                {
                    status = DEV_Current_Process_Raw(p_inst);

                    if(status == CURRENT_STATUS_OK)
                    {
                        p_inst->state = CURRENT_STATE_READY;
                    }
                }
                else 
                {
                    status = CURRENT_STATUS_ERROR_ADC_ERROR;

                    BMS_Fault_Update(BMS_FAULT_TIMING_LAPSE, true);
                }

                break;

                case CURRENT_STATE_READY:

                p_inst->start_time = UTIL_Time_Get_Tick();



                break;
            }
        }
        else 
        {
            status = CURRENT_STATUS_ERROR_NOT_INIT;

            BMS_Fault_Update(BMS_FAULT_NOT_INIT, true);
        }
    }
    else
    {
        status = CURRENT_STATUS_ERROR_NULL_POINTER;
        
        BMS_Fault_Update(BMS_FAULT_NULL_POINTER, true);
    }

    return status;
}

current_status_t DEV_Current_Process_Raw(dev_current_t *p_inst)
{
    current_status_t status = CURRENT_STATUS_OK;

    int32_t current_mA = 0; // initialize at safe value;

    if((p_inst != NULL) && (p_inst->cfg != NULL) && (p_inst->adc_inst != NULL))
    {
        const uint32_t raw = DEV_Current_Get_Raw(p_inst);

        const uint16_t vref = IO_ADC_Get_Vref(p_inst->adc_inst);

        const uint16_t scale = IO_ADC_Get_Scale(p_inst->adc_inst);

        const int32_t offset = IO_ADC_Get_Offset(p_inst->adc_inst);

        const int32_t gain_uV = DEV_Current_Get_Gain(p_inst);

        if((scale > 0U) && (gain_uV != 0))
        {
            uint32_t voltage_mV = (raw * vref) / scale;

            int32_t delta_mV = (int32_t)voltage_mV - offset;

            current_mA = (delta_mV * MV_TO_UV) / gain_uV;
        }
    }
    else 
    {
        status = CURRENT_STATUS_ERROR_NULL_POINTER;

        BMS_Fault_Update(BMS_FAULT_NULL_POINTER, true);
    }

    p_inst->last_val = current_mA;

    return status;
}

uint32_t DEV_Current_Get_Wait(dev_current_t *p_inst)
{
    uint32_t wait = 0U;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        if(p_inst->is_init)
        {
            wait = p_inst->cfg->current_wait_ms;
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

    return wait;
}

int32_t DEV_Current_Get_Gain(dev_current_t *p_inst)
{
    int32_t gain = 0;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        if(p_inst->is_init)
        {
            gain = p_inst->cfg->current_gain_uV;
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

    return gain;
}

uint32_t DEV_Current_Get_Raw(dev_current_t *p_inst)
{
    int64_t raw = 0; // large unsigned integer (long long) so as to not lose accuracy

    if((p_inst != NULL) && (p_inst->adc_inst != NULL))
    {
        if(p_inst->is_init)
        {
            raw = (int64_t)IO_ADC_Get_Val(p_inst->adc_inst);

            raw -= (int64_t)p_inst->raw_offset; 

            if(raw < 0)
            {
                raw = 0; // value clamping;
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

    return (uint32_t)raw;
}

int32_t DEV_Current_Get_Val(dev_current_t *p_inst)
{
    int32_t val = 0;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        if(p_inst->is_init)
        {
            val = p_inst->last_val;

            if(DEV_Current_Get_State(p_inst) == CURRENT_STATE_READY)
            {
                p_inst->state = CURRENT_STATE_IDLE;
            }
        }
    }
    else
    {
        BMS_Fault_Update(BMS_FAULT_NULL_POINTER, true);
    }

    return val;
}

current_state_t DEV_Current_Get_State(dev_current_t *p_inst)
{
    current_state_t state = CURRENT_STATE_ERROR;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        if(p_inst->is_init)
        {
            state = p_inst->state;
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

    return state;
}


