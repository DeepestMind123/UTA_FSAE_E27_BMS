/**
 * @file DEV_current.h
 * @author notwe
 * @date 2026-05-03
 * @brief current sensor device driver source
 */

#include "dev_current.h"
#include "io_adc.h"
#include "sys_fault.h"

void DEV_Current_Init(dev_current_t *p_inst, dev_current_cfg_t *p_cfg, io_adc_t *p_adc_inst)
{
    if((p_inst != NULL) && (p_cfg != NULL) && (p_adc_inst != NULL))
    {
        p_inst->cfg = p_cfg;
        p_inst->adc_inst = p_adc_inst;

        p_inst->elapsed = 0;
        p_inst->cal_acc = 0;
        p_inst->cal_count = 0;
        p_inst->raw_offset = 0;
    }
    else
    {
        BMS_Fault_Update(BMS_FAULT_NULL_POINTER, false);
    }
}

void DEV_Current_Task(dev_current_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL) && (p_inst->adc_inst != NULL))
    {
        if(UTIL_Time_Get_Tick() - p_inst->elapsed >= p_inst->cfg->wait_ms)
        {
            if(!IO_ADC_Check(p_inst->adc_inst))
            {
                IO_ADC_Start(p_inst->adc_inst);

                p_inst->ready_flag = false;
            }
        }
        else
        {
            // insert error handler (current manager timeout)
        }

        if(IO_ADC_Check(p_inst->adc_inst))
        {
            DEV_Current_Process_Raw(p_inst);

            p_inst->ready_flag = true;
        }

        IO_ADC_Task(p_inst->adc_inst);
    }
    else
    {
        // insert error handler 
    }
}

bool DEV_Current_Cal(dev_current_t *p_inst)
{

    uint32_t out = false;

    if((p_inst != NULL) && (p_inst->cfg != NULL) && (p_inst->adc_inst != NULL))
    {
        if(p_inst->cal_count < 32)
        {
            if(IO_ADC_Check(p_inst->adc_inst))
            {
                out = IO_ADC_Get_Val(p_inst->adc_inst);

                if(out <= p_inst->cfg->raw_cutoff)
                {
                    p_inst->cal_acc += out;

                    p_inst->cal_count++;
                }
            }
            else if(IO_ADC_Check(p_inst->adc_inst))
            {
                DEV_Current_Task(p_inst);
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

void DEV_Current_Process_Raw(dev_current_t *p_inst)
{
    int32_t current_mA = 0; // initialize at safe value;

    if((p_inst != NULL) && (p_inst->cfg != NULL) && (p_inst->adc_inst != NULL))
    {
        const uint32_t raw = DEV_Current_Get_Raw(p_inst);

        const uint16_t vref = IO_ADC_Get_Vref(p_inst->adc_inst);

        const uint16_t scale = IO_ADC_Get_Scale(p_inst->adc_inst);

        const int32_t offset = IO_ADC_Get_Offset(p_inst->adc_inst);

        const int32_t gain_uV = p_inst->cfg->sensor_gain_uV;

        if((scale > 0U) && (gain_uV != 0))
        {
            uint32_t voltage_mV = (raw * vref) / scale;

            int32_t delta_mV = (int32_t)voltage_mV - offset;

            current_mA = (delta_mV * MV_TO_UV) / gain_uV;
        }
    }

    p_inst->val_buffer = current_mA;
}

uint32_t DEV_Current_Get_Wait(dev_current_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        return p_inst->cfg->wait_ms;
    }
    return 0;
}

int32_t DEV_Current_Get_Gain(dev_current_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        return p_inst->cfg->sensor_gain_uV;
    }
    return 0;
}

uint32_t DEV_Current_Get_Raw(dev_current_t *p_inst)
{
    int64_t raw = 0; // large unsigned integer (long long) so as to not lose accuracy

    if((p_inst != NULL) && (p_inst->adc_inst != NULL))
    {
        raw = (int64_t)IO_ADC_Get_Val(p_inst->adc_inst);

        raw -= p_inst->raw_offset; 

        if(raw < 0)
        {
            raw = 0; // value clamping;
        }
    }
    else
    {
        // insert error handler
    }
    return (uint32_t)raw;
}

int32_t DEV_Current_Get_Val(dev_current_t *p_inst)
{
    int32_t val = 0U;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        val = p_inst->val_buffer;
    }
    else
    {
        // insert error handler (i really gotta get around to making this sucker)
    }

    return val;
}

bool DEV_Current_Get_Ready(dev_current_t *p_inst)
{
    bool is_ready = false;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        is_ready = p_inst->ready_flag;
    }
    else
    {
        // insert error handler
    }

    return is_ready;
}

