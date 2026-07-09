/**
 * @file bms_daq.c
 * @author notwe
 * @date 2026-07-08
 * @brief bms daq task source file
*/

#include "bms_daq.h"

typedef struct
{
    uint32_t isense_start_time;
    uint32_t vsense_start_time;
    uint32_t tsense_start_time;
    uint32_t now_time;
    bool is_init;
    daq_state_t state;
    daq_timeout_t real_timeout;
    daq_timeout_t init_timeout;
    daq_data_t data_buffer;
    daq_data_t *p_data_out;
    const util_time_t *p_time;
    const dev_current_sensor_t *p_isense_high;
    const dev_current_sensor_t *p_isense_low;
    dev_current_sensor_t *p_last_isense;
    current_sensor_state_t isense_state;
    voltage_sensor_state_t vsense_state;
} daq_t;

static daq_t s_daq;

daq_status_t BMS_DAQ_Init(const daq_cfg_t *p_cfg)
{
    daq_status_t status = DAQ_NOT_INIT;

    if(s_daq.is_init)
    {
        if(p_cfg != NULL)
        {
            s_daq.p_data_out = p_cfg->out_mem;
            s_daq.p_time = p_cfg->time_cfg;

            s_daq.init_timeout = p_cfg->timeout_cfg;
            s_daq.real_timeout = s_daq.init_timeout;

            s_daq.p_isense_high = p_cfg->isense_high_cfg;
            s_daq.p_isense_low = p_cfg->isense_low_cfg;
            s_daq.p_last_isense = s_daq.p_isense_low;

            s_daq.isense_start_time = 0U;
            s_daq.vsense_start_time = 0U;
            s_daq.tsense_start_time = 0U;
            s_daq.now_time = 0U;

            s_daq.data_buffer.i_data.ival_mA = 0;
            s_daq.data_buffer.i_data.i_timestamp = 0U;
            s_daq.data_buffer.i_data.i_valid = true;   

            for(uint8_t i = 0U; i < SMALL_ARR_32; i++)
            {
                for(uint8_t j = 0U; j < LARGE_ARR_64; i++)
                {
                    s_daq.data_buffer.v_data.modv[i].cell_val_mV[j] = 0;
                }
            }

            s_daq.data_buffer.v_data.v_timestamp = 0U;
            s_daq.data_buffer.v_data.v_valid = true;

            for(uint8_t i = 0U; i < SMALL_ARR_32; i++)
            {
                for(uint8_t j = 0U; j < SMALL_ARR_32; j++)
                {
                    s_daq.data_buffer.t_data.modt[i].tsense_val_dC[j] = 0;
                }
            }

            s_daq.data_buffer.t_data.t_timestamp = 0U;
            s_daq.data_buffer.t_data.t_valid = true;

            s_daq.state = DAQ_STATE_IDLE;
            
            s_daq.is_init = true;

            status = DAQ_OK;
        }
        else
        {
            status = DAQ_NULL_PTR;
        }
    }
    else
    {
        status = DAQ_DBL_INIT;
    }

    return status;
}

daq_status_t BMS_DAQ_Task(void)
{
    daq_status_t status = DAQ_OK;

    current_sensor_state_t isense_status;
    time_status_t time_status;

    if(s_daq.is_init)
    {

        status = BMS_DAQ_Isense_Switch_Task();

        // add tsense task
        // add vsense task

        if(s_daq.state >= DAQ_STATE_MAX)
        {
            s_daq.state = DAQ_STATE_ERROR;

            status = DAQ_UNDEF_STATE;
        }

        switch(s_daq.state)
        {
            case DAQ_STATE_IDLE:
            {
                break;
            }

            case DAQ_STATE_ISENSE:
            {
                status = BMS_DAQ_Isense_Switch_State();

                break;
            }

            case DAQ_STATE_VSENSE:
            {
                break;
            }

            case DAQ_STATE_TSENSE:
            {
                break;
            }

            case DAQ_STATE_REPORT:
            {
                break;
            }

            case DAQ_STATE_ERROR:
            {
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
        status = DAQ_NOT_INIT;
    }

    return status;
}

daq_status_t BMS_DAQ_Isense_Switch_Task(void)
{
    daq_status_t status = DAQ_OK;
    current_sensor_state_t isense_status;

    if(s_daq.is_init)
    {
        isense_status = DEV_Current_Sensor_Get_State(s_daq.p_last_isense, &s_daq.isense_state);

        if(isense_status == CURRENT_SENSOR_OK)
        {
            if(s_daq.isense_state == CURRENT_SENSOR_IDLE)
            {
                if(abs(s_daq.data_buffer.i_data.ival_mA) > ISENSE_HANDOFF_MAX_MA)
                {
                    s_daq.p_last_isense = s_daq.p_isense_high;

                    isense_status = DEV_Current_Sensor_Set_Timeout(s_daq.p_isense_low, 0U);
                }
                else if(abs(s_daq.data_buffer.i_data.ival_mA) <= ISENSE_HANDOFF_MIN_MA)
                {
                    s_daq.p_last_isense = s_daq.p_isense_low;

                    isense_status = DEV_Current_Sensor_Set_Timeout(s_daq.p_isense_high, 0U);
                }

                if(isense_status == CURRENT_SENSOR_OK)
                {
                    isense_status = DEV_Current_Sensor_Task(s_daq.p_last_isense);

                    if(isense_status != CURRENT_SENSOR_OK)
                    {
                        status = DAQ_ISENSE_FAULT;
                    }
                }
                else
                {
                    status = DAQ_ISENSE_FAULT;
                }
            }
            else
            {
                isense_status = DEV_Current_Sensor_Task(s_daq.p_last_isense);

                if(isense_status != CURRENT_SENSOR_OK)
                {
                    status = DAQ_ISENSE_FAULT;
                }
            }
        }
        else
        {
            status = DAQ_ISENSE_FAULT;
        }
    }
    else
    {
        status = DAQ_NOT_INIT;
    }

    return status;
}

daq_status_t BMS_DAQ_Isense_State(void)
{
    daq_status_t status = DAQ_OK;
    current_sensor_state_t isense_status;
    time_status_t time_status;

    if(s_daq.is_init)
    {
        bool isense_ready = false;

        isense_status = DEV_Current_Sensor_Get_State(s_daq.p_last_isense, &s_daq.isense_state);

        if(isense_status == CURRENT_SENSOR_OK)
        {
            if(s_daq.isense_state == CURRENT_SENSOR_IDLE)
            {
                isense_status = DEV_Current_Sensor_Get_Ready_Flag(s_daq.p_last_isense, &isense_ready);

                if(isense_status == CURRENT_SENSOR_OK)
                {
                    time_status = UTIL_Time_Get_Tick(s_daq.p_time, &s_daq.now_time);

                    if(time_status == TIME_STATUS_OK)
                    {
                        if(isense_ready)
                        {
                            /*
                            need to add data integrity checking mechanism
                            it is a single clock cycle read so its probably fine to brute force this
                            buuut voltage measurements are a giant array so that wont work
                            */

                            int32_t temp_val = 0;

                            isense_status = DEV_Current_Sensor_Get_Val(s_daq.p_last_isense, &temp_val);

                            if(isense_status == CURRENT_SENSOR_OK)
                            {
                                s_daq.data_buffer.i_data.ival_mA = temp_val;

                                s_daq.data_buffer.i_data.i_timestamp = s_daq.now_time;
                            }
                            else
                            {
                                status = DAQ_ISENSE_FAULT;
                            }
                        }
                        else if(s_daq.now_time - s_daq.isense_start_time >= s_daq.real_timeout.isense_ready_timeout)
                        {
                            isense_status = DEV_Current_Sensor_Start(s_daq.p_last_isense);

                            if(isense_status == CURRENT_SENSOR_OK)
                            {
                                s_daq.isense_start_time = s_daq.now_time;
                            }
                            else
                            {
                                status = DAQ_ISENSE_FAULT;
                            }
                        }
                    }
                    else
                    {
                        status = DAQ_TIME_FAULT;
                    }
                }
                else
                {
                    status = DAQ_ISENSE_FAULT;
                }
            }
        }
        else
        {
            status = DAQ_ISENSE_FAULT;
        }
    }
    else
    {
        status = DAQ_NOT_INIT;
    }

    return status;
}