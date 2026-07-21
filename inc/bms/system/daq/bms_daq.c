/**
 * @file bms_daq.c
 * @author notwe
 * @date 2026-07-08
 * @brief bms daq task source file
*/

#include "bms_daq.h"

typedef struct
{
    isense_t *p_isense_high;
    isense_t *p_isense_low;
    isense_t *p_last_isense;
    uint32_t isense_start_time;
    isense_state_t isense_state;
} daq_isense_ctx_t;

typedef struct
{
    vsense_t *p_vsense;
    uint32_t vsense_start_time;
    vsense_state_t vsense_state;
} daq_vsense_ctx_t;

typedef struct 
{
    tsense_t *p_tsense;
    uint32_t tsense_start_time;
    tsense_state_t tsense_state;
} daq_tsense_ctx_t;

typedef struct
{
    bool is_init;
    bool is_ready;
    daq_state_t state;
    uint32_t now_time;
    const util_time_t *p_time;
    const util_irq_t *p_irq;
    
    daq_timeout_t real_timeout;
    daq_timeout_t init_timeout;

    daq_data_t data_buffer;
    daq_data_t *p_data_out;

    daq_isense_ctx_t isense;
    daq_vsense_ctx_t vsense;
    daq_tsense_ctx_t tsense;
} daq_t;

static daq_t s_daq;

daq_status_t BMS_DAQ_Init(const daq_cfg_t *p_cfg)
{
    daq_status_t status = DAQ_NOT_INIT;

    if(!s_daq.is_init)
    {
        if(p_cfg != NULL)
        {
            s_daq.p_data_out = p_cfg->out_mem;
            s_daq.p_time = p_cfg->time_cfg;
            s_daq.p_irq = p_cfg->irq_cfg;

            s_daq.init_timeout = p_cfg->timeout_cfg;
            s_daq.real_timeout = s_daq.init_timeout;

            s_daq.isense.p_isense_high = p_cfg->isense_high_cfg;
            s_daq.isense.p_isense_low = p_cfg->isense_low_cfg;
            s_daq.isense.p_last_isense = s_daq.isense.p_isense_low;
            s_daq.vsense.p_vsense = p_cfg->vsense_cfg;
            s_daq.tsense.p_tsense = p_cfg->tsense_cfg;

            s_daq.isense.isense_start_time = 0U;
            s_daq.vsense.vsense_start_time = 0U;
            s_daq.tsense.tsense_start_time = 0U;
            s_daq.now_time = 0U;

            s_daq.data_buffer.i_data.ival_mA = 0;
            s_daq.data_buffer.i_data.i_timestamp = 0U;
            s_daq.data_buffer.i_data.i_valid = true;   

            for(uint8_t i = 0U; i < SMALL_ARR_16; i++)
            {
                for(uint8_t j = 0U; j < LARGE_ARR_32; j++)
                {
                    s_daq.data_buffer.v_data.vmod[i].cell_val_mV[j] = 0;
                }
            }

            s_daq.data_buffer.v_data.v_timestamp = 0U;
            s_daq.data_buffer.v_data.v_valid = true;

            for(uint8_t i = 0U; i < SMALL_ARR_16; i++)
            {
                for(uint8_t j = 0U; j < SMALL_ARR_16; j++)
                {
                    s_daq.data_buffer.t_data.tmod[i].tsense_val_C[j] = 0;
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

daq_status_t BMS_DAQ_Task(const daq_ctx_t *p_in)
{
    daq_status_t status = DAQ_OK;
    
    vsense_status_t vsense_status;
    tsense_status_t tsense_status;

    if(s_daq.is_init)
    {
        if(p_in != NULL)
        {
            s_daq.real_timeout = p_in->new_timeout;

            status = BMS_DAQ_Isense_Switch_Task();

            vsense_status = DEV_Vsense_Task(s_daq.vsense.p_vsense);

            tsense_status = DEV_Tsense_Task(s_daq.tsense.p_tsense);

            if(status != DAQ_OK)
            {
                /* intentionally left blank*/
            } 
            else if(vsense_status != VSENSE_OK)
            {
                status = DAQ_VSENSE_FAULT;
            }
            else if(tsense_status != TSENSE_OK)
            {
                status = DAQ_TSENSE_FAULT;
            }
            else
            {
                if(s_daq.state >= DAQ_STATE_MAX)
                {
                    s_daq.state = DAQ_STATE_ERROR;

                    status = DAQ_UNDEF_STATE;
                }

                switch(s_daq.state)
                {
                    case DAQ_STATE_IDLE:
                    {
                        status = BMS_DAQ_Idle_State();

                        if(status != DAQ_OK)
                        {
                            s_daq.state = DAQ_STATE_ERROR;
                        }

                        break;
                    }

                    case DAQ_STATE_ISENSE:
                    {
                        status = BMS_DAQ_Isense_State();

                        if(status != DAQ_OK)
                        {
                            s_daq.state = DAQ_STATE_ERROR;
                        }
                        else
                        {
                            s_daq.state = DAQ_STATE_IDLE;
                        }

                        break;
                    }

                    case DAQ_STATE_VSENSE:
                    {
                        status = BMS_DAQ_Vsense_State();

                        if(status != DAQ_OK)
                        {
                            s_daq.state = DAQ_STATE_ERROR;
                        }
                        else
                        {
                            s_daq.state = DAQ_STATE_IDLE;
                        }

                        break;
                    }

                    case DAQ_STATE_TSENSE:
                    {
                        status = BMS_DAQ_Tsense_State();

                        if(status != DAQ_OK)
                        {
                            s_daq.state = DAQ_STATE_ERROR;
                        }
                        else
                        {
                            s_daq.state = DAQ_STATE_IDLE;
                        }

                        break;
                    }

                    case DAQ_STATE_REPORT:
                    {
                        status = BMS_DAQ_Report_State();

                        if(status != DAQ_OK)
                        {
                            s_daq.state = DAQ_STATE_ERROR;
                        }
                        else
                        {
                            s_daq.state = DAQ_STATE_IDLE;
                        }

                        break;
                    }

                    case DAQ_STATE_ERROR:
                    {
                        break;
                    }

                    default:
                    {
                        s_daq.state = DAQ_STATE_ERROR;

                        break;
                    }
                }  
            }
        }
        else
        {
            status = DAQ_NULL_PTR;
        }
    }
    else
    {
        status = DAQ_NOT_INIT;
    }

    return status;
}

daq_status_t BMS_DAQ_Idle_State(void)
{
    daq_status_t status = DAQ_OK;

    if(!s_daq.is_init)
    {
        status = DAQ_NOT_INIT;
    }
    
    if(status == DAQ_OK)
    {
        if(UTIL_Time_Get_Tick(s_daq.p_time, &s_daq.now_time) == TIME_STATUS_OK)
        {
            if((s_daq.now_time - s_daq.data_buffer.i_data.i_timestamp >= TASK_DELAY_ISENSE) &&
                (s_daq.now_time - s_daq.data_buffer.i_data.i_timestamp < s_daq.real_timeout.isense_ready_timeout))
            {
                s_daq.state = DAQ_STATE_ISENSE;
            }
            else if((s_daq.now_time - s_daq.data_buffer.v_data.v_timestamp >= TASK_DELAY_VSENSE) &&
                    (s_daq.now_time - s_daq.data_buffer.v_data.v_timestamp < s_daq.real_timeout.vsense_ready_timeout))
            {
                s_daq.state = DAQ_STATE_VSENSE;
            }
            else if((s_daq.now_time - s_daq.data_buffer.t_data.t_timestamp >= TASK_DELAY_TSENSE) &&
                    (s_daq.now_time - s_daq.data_buffer.t_data.t_timestamp < s_daq.real_timeout.tsense_ready_timeout))
            {
                s_daq.state = DAQ_STATE_TSENSE;
            }
            else if((s_daq.data_buffer.i_data.i_valid) || (s_daq.data_buffer.v_data.v_valid) || (s_daq.data_buffer.t_data.t_valid))
            {
                s_daq.state = DAQ_STATE_REPORT;
            }
            else if(s_daq.now_time - s_daq.data_buffer.i_data.i_timestamp >= s_daq.real_timeout.isense_timeout)
            {
                status = DAQ_ISENSE_TIMEOUT;

                s_daq.state = DAQ_STATE_ERROR;
            }
            else if(s_daq.now_time - s_daq.data_buffer.v_data.v_timestamp >= s_daq.real_timeout.vsense_timeout)
            {
                status = DAQ_VSENSE_TIMEOUT;

                s_daq.state = DAQ_STATE_ERROR;
            }
            else if(s_daq.now_time - s_daq.data_buffer.t_data.t_timestamp >= s_daq.real_timeout.tsense_timeout)
            {
                status = DAQ_TSENSE_TIMEOUT;

                s_daq.state = DAQ_STATE_ERROR;
            }
        }
        else
        {
            status = DAQ_TIME_FAULT;
        }
    }

    return status;
}

daq_status_t BMS_DAQ_Isense_Switch_Task(void)
{
    daq_status_t status = DAQ_OK;
    isense_state_t isense_status;

    if(!s_daq.is_init)
    {
        isense_status = DEV_Isense_Get_State(s_daq.isense.p_last_isense, &s_daq.isense.isense_state);

        if(isense_status == ISENSE_OK)
        {
            if(s_daq.isense.isense_state == ISENSE_STATE_IDLE)
            {
                if(abs(s_daq.data_buffer.i_data.ival_mA) > HANDOFF_MAX_MA_ISENSE)
                {
                    s_daq.isense.p_last_isense = s_daq.isense.p_isense_high;

                    isense_status = DEV_Isense_Set_Timeout(s_daq.isense.p_isense_low, 0U);
                }
                else if(abs(s_daq.data_buffer.i_data.ival_mA) <= HANDOFF_MIN_MA_ISENSE)
                {
                    s_daq.isense.p_last_isense = s_daq.isense.p_isense_low;

                    isense_status = DEV_Isense_Set_Timeout(s_daq.isense.p_isense_high, 0U);
                }

                if(isense_status == ISENSE_OK)
                {
                    isense_status = DEV_Isense_Task(s_daq.isense.p_last_isense);

                    if(isense_status != ISENSE_OK)
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
                isense_status = DEV_Isense_Task(s_daq.isense.p_last_isense);

                if(isense_status != ISENSE_OK)
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
    isense_state_t isense_status;
    time_status_t time_status;
    bool isense_ready = false;

    if(!s_daq.is_init)
    {
        status = DAQ_NOT_INIT;
    }

    if(status == DAQ_OK)
    {
        isense_status = DEV_Isense_Get_State(s_daq.isense.p_last_isense, &s_daq.isense.isense_state);

        if(isense_status != ISENSE_OK)
        {
            status = DAQ_ISENSE_FAULT;
        }
    }

    if((status == DAQ_OK) && (s_daq.isense.isense_state == ISENSE_STATE_IDLE))
    {
        isense_status = DEV_Isense_Get_Ready_Flag(s_daq.isense.p_last_isense, &isense_ready);

        if(isense_status != ISENSE_OK)
        {
            status = DAQ_ISENSE_FAULT;
        }
        else if(UTIL_Time_Get_Tick(s_daq.p_time, &s_daq.now_time) != TIME_STATUS_OK)
        {
            status = DAQ_TIME_FAULT;
        }
        else if(isense_ready)
        {
            int16_t temp_val = 0;

            isense_status = DEV_Isense_Get_Val(s_daq.isense.p_last_isense, &temp_val);

            if(isense_status == ISENSE_OK)
            {
                s_daq.data_buffer.i_data.ival_mA = temp_val;

                s_daq.data_buffer.i_data.i_timestamp = s_daq.now_time;

                s_daq.data_buffer.i_data.i_valid = true;
            }
            else
            {
                status = DAQ_ISENSE_FAULT;
            }
        }
        else if((s_daq.now_time - s_daq.isense.isense_start_time) >= s_daq.real_timeout.isense_ready_timeout)
        {
            isense_status = DEV_Isense_Start(s_daq.isense.p_last_isense);

            if(isense_status == ISENSE_OK)
            {
                s_daq.isense.isense_start_time = s_daq.now_time;
            }
            else
            {
                status = DAQ_ISENSE_FAULT;
            }
        }
        else
        {
            /* wait for sensor to be ready*/
        }
    }

    return status;
}

daq_status_t BMS_DAQ_Vsense_State(void)
{
    daq_status_t status = DAQ_OK;
    vsense_status_t vsense_status;
    time_status_t time_status;
    bool vsense_ready = false;

    if(!s_daq.is_init)
    {
        status = DAQ_NOT_INIT;
    }

    if(status == DAQ_OK)
    {
        vsense_status = DEV_Vsense_Get_State(s_daq.vsense.p_vsense, &s_daq.vsense.vsense_state);

        if(vsense_status != VSENSE_OK)
        {
            status = DAQ_VSENSE_FAULT;
        }
    }

    if((status == DAQ_OK) && (s_daq.vsense.vsense_state == VSENSE_STATE_IDLE))
    {
        vsense_status = DEV_Vsense_Get_Ready_Flag(s_daq.vsense.p_vsense, &vsense_ready);

        if(vsense_status != VSENSE_OK)
        {
            status = DAQ_VSENSE_FAULT;
        }
        else if(UTIL_Time_Get_Tick(s_daq.p_time, &s_daq.now_time) != TIME_STATUS_OK)
        {
            status = DAQ_TIME_FAULT;
        }
        else if(vsense_ready)
        {
            bool temp_valid = false;

            /*
            need to add data integrity checking mechanism
            voltage measurements are a giant array
            that being said, for 5 modules with 28 cells each its takes like 60 us
            to index through each and update the values so i don't even think its that big a deal
            turning off interrupts for that
            */

            /*this is a runtime defined non-variable sized array
            thus it can be sized to actual number of ICs for efficiency*/

            vsense_val_t temp_val[SMALL_ARR_16] = {0};
            temp_valid = true;
            
            vsense_status = DEV_Vsense_Get_Val(s_daq.vsense.p_vsense, &temp_val);

            //now need to cast to the array layout that bms_daq uses

            if(vsense_status != VSENSE_OK)
            {
                status = DAQ_VSENSE_FAULT;
            }
            else if(temp_valid)
            {
                status = BMS_DAQ_Map_Vdata(&temp_val, &s_daq.data_buffer.v_data);

                if(status == DAQ_OK)
                {
                    s_daq.data_buffer.v_data.v_timestamp = s_daq.now_time;

                    s_daq.data_buffer.v_data.v_valid = true;
                }
            }              
        }
        else if(s_daq.now_time - s_daq.vsense.vsense_start_time >= s_daq.real_timeout.vsense_ready_timeout)
        {
            vsense_status = DEV_Vsense_Start(s_daq.vsense.p_vsense);

            if(vsense_status == VSENSE_OK)
            {
                s_daq.vsense.vsense_start_time = s_daq.now_time;
            }
            else
            {
                status = DAQ_VSENSE_FAULT;
            }
        }
        else
        {
            /* wait for ready timeout*/
        }
    }

    return status;
}

daq_status_t BMS_DAQ_Map_Vdata(const vsense_val_t (*p_in)[SMALL_ARR_16], vsense_data_t *p_out)
{
    daq_status_t status = DAQ_OK;

    if((p_in != NULL) && (p_out != NULL))
    {
        if(s_daq.is_init)
        {
            for(uint8_t ic_idx = 0U; ic_idx < TOTAL_BMS_IC_NUM; ic_idx++)
            {
                uint8_t mod_idx = ic_idx / MOD_BMS_IC_NUM;
                uint8_t cell_offset = (ic_idx % MOD_BMS_IC_NUM) * CELLS_PER_BMS_IC;

                for (uint8_t cell_idx = 0U; cell_idx < CELLS_PER_BMS_IC; cell_idx++)
                {
                    p_out->vmod[mod_idx].cell_val_mV[cell_offset + cell_idx] =
                        (*p_in)[ic_idx].cells_mV[cell_idx];
                }
            }

            // data validation performed in task function so is unecessary here 
        }
        else
        {
            status = DAQ_NOT_INIT;
        }
    }
    else
    {
        status = DAQ_NULL_PTR;
    }

    return status;
}

daq_status_t BMS_DAQ_Tsense_State(void)
{
    daq_status_t status = DAQ_OK;
    tsense_status_t tsense_status;
    time_status_t time_status;
    bool tsense_ready = false;

    if(!s_daq.is_init)
    {
        status = DAQ_NOT_INIT;
    }

    if(status == DAQ_OK)
    {
        tsense_status = DEV_Tsense_Get_State(s_daq.tsense.p_tsense, &s_daq.tsense.tsense_state);

        if(tsense_status != TSENSE_OK)
        {
            status = DAQ_TSENSE_FAULT;
        }
    }

    if((status == DAQ_OK) && (s_daq.tsense.tsense_state == TSENSE_STATE_IDLE))
    {
        tsense_status = DEV_Tsense_Get_Ready_Flag(s_daq.tsense.p_tsense, &tsense_ready);

        if(tsense_status != TSENSE_OK)
        {
            status = DAQ_TSENSE_FAULT;
        }
        else if(UTIL_Time_Get_Tick(s_daq.p_time, &s_daq.now_time) != TIME_STATUS_OK)
        {
            status = DAQ_TIME_FAULT;
        }
        else if(tsense_ready)
        {
            bool temp_valid = false;

            // need data valid check

            /*this is a runtime defined non-variable sized array
            thus it can be sized to actual number of ICs for efficiency*/

            tsense_val_t temp_val[SMALL_ARR_16] = {0};
            temp_valid = true;
            
            tsense_status = DEV_Tsense_Get_Val(s_daq.tsense.p_tsense, &temp_val);

            //now need to cast to the array layout that bms_daq uses

            if(tsense_status != TSENSE_OK)
            {
                status = DAQ_TSENSE_FAULT;
            }
            else if(temp_valid)
            {
                status = BMS_DAQ_Map_Tdata(&temp_val, &s_daq.data_buffer.t_data);

                if(status == DAQ_OK)
                {
                    s_daq.data_buffer.t_data.t_timestamp = s_daq.now_time;

                    s_daq.data_buffer.t_data.t_valid = true;
                }
            }              
        }
        else if(s_daq.now_time - s_daq.tsense.tsense_start_time >= s_daq.real_timeout.tsense_ready_timeout)
        {
            tsense_status = DEV_Tsense_Start(s_daq.tsense.p_tsense);

            if(tsense_status == TSENSE_OK)
            {
                s_daq.tsense.tsense_start_time = s_daq.now_time;
            }
            else
            {
                status = DAQ_TSENSE_FAULT;
            }
        }
        else
        {
            /* wait for ready timeout*/
        }
    }

    return status;

}

daq_status_t BMS_DAQ_Map_Tdata(const tsense_val_t (*p_in)[SMALL_ARR_16], tsense_data_t *p_out)
{
    daq_status_t status = DAQ_OK;

    if((p_in != NULL) && (p_out != NULL))
    {
        if(s_daq.is_init)
        {
            for(uint8_t ic_idx = 0U; ic_idx < TOTAL_BMS_IC_NUM; ic_idx++)
            {
                uint8_t mod_idx = ic_idx / MOD_BMS_IC_NUM;
                uint8_t cell_offset = (ic_idx % MOD_BMS_IC_NUM) * CELLS_PER_BMS_IC;

                for (uint8_t cell_idx = 0U; cell_idx < CELLS_PER_BMS_IC; cell_idx++)
                {
                    p_out->tmod[mod_idx].tsense_val_C[cell_offset + cell_idx] =
                        (*p_in)[ic_idx].temps_C[cell_idx];
                }
            }

            // data validation performed in task function so is unecessary here 
        }
        else
        {
            status = DAQ_NOT_INIT;
        }
    }
    else
    {
        status = DAQ_NULL_PTR;
    }

    return status;
}

daq_status_t BMS_DAQ_Report_State(void)
{
    daq_status_t status = DAQ_OK;

    if(!s_daq.is_init)
    {
        status = DAQ_NOT_INIT;
    }

    if(s_daq.p_data_out == NULL)
    {
        status = DAQ_NULL_PTR;
    }

    if(status == DAQ_OK)
    {
        if(s_daq.data_buffer.i_data.i_valid)
        {
            //need to add enter critical read/write
            s_daq.p_data_out->i_data = s_daq.data_buffer.i_data;
            //need to add exit critical read/write

            s_daq.data_buffer.i_data.i_valid = false;
        }
        
        if(s_daq.data_buffer.v_data.v_valid)
        {
            for(uint8_t i = 0U; i < MOD_NUM; i++)
            {
                //need to add enter critical read/write
                s_daq.p_data_out->v_data.vmod[i] = s_daq.data_buffer.v_data.vmod[i];
                //need to add exit critical read/write
            }

            s_daq.p_data_out->v_data.v_timestamp = s_daq.data_buffer.v_data.v_timestamp;
            s_daq.p_data_out->v_data.v_valid = s_daq.data_buffer.v_data.v_valid;

            s_daq.data_buffer.v_data.v_valid = false;
        }

        if(s_daq.data_buffer.t_data.t_valid)
        {
            for(uint8_t i = 0U; i < MOD_NUM; i++)
            {
                //need to add enter critical read/write
                s_daq.p_data_out->t_data.tmod[i] = s_daq.data_buffer.t_data.tmod[i];
                //need to add exit critical read/write
            }

            s_daq.p_data_out->t_data.t_timestamp = s_daq.data_buffer.t_data.t_timestamp;
            s_daq.p_data_out->t_data.t_valid = s_daq.data_buffer.t_data.t_valid;

            s_daq.data_buffer.t_data.t_valid = false;
        }

        if((s_daq.data_buffer.i_data.i_valid) && 
            (s_daq.data_buffer.v_data.v_valid) && 
            (s_daq.data_buffer.t_data.t_valid))
        {
            s_daq.is_ready = true;
        }
        else
        {
            s_daq.is_ready = false;
        }
    }

    return status;
}

daq_status_t BMS_DAQ_Get_Data(const daq_data_t *p_out)
{
    daq_status_t status = DAQ_OK;

    daq_data_t data = {0};

    irq_status_t irq_status;

    uint32_t interrupt_state;

    if(s_daq.is_init)
    {
        if(p_out != NULL)
        {
            irq_status = UTIL_IRQ_Enter_Critical(s_daq.p_irq, &interrupt_state);

            if(irq_status == IRQ_STATUS_OK)
            {
                data = *s_daq.p_data_out;
            }
            else
            {
                status = DAQ_IRQ_FAULT;
            }

            irq_status = UTIL_IRQ_Exit_Critical(s_daq.p_irq, interrupt_state);

            if(irq_status != IRQ_STATUS_OK)
            {
                status = DAQ_IRQ_FAULT;
            }

            p_out = &data;
        }
        else
        {
            status = DAQ_NULL_PTR;
        }
    }
    else
    {
        status = DAQ_NOT_INIT;
    }

    return status;
}

daq_status_t BMS_DAQ_Get_Ready_Flag(bool *p_out)
{
    daq_status_t status = DAQ_OK;

    bool flag = false;

    if(s_daq.is_init)
    {
        if(p_out != NULL)
        {
            flag = s_daq.is_ready;

            *p_out = flag;
        }
        else
        {
            status = DAQ_NULL_PTR;
        }
    }
    else
    {
        status = DAQ_NOT_INIT;
    }

    return status;
}