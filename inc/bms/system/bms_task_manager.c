/**
 * @file bms_manager.c
 * @author notwe
 * @date 2026-05-03
 * @brief system manager source
 */

#include "bms_task_manager.h"
#include "bms_fault.h"

typedef struct
{
    bms_tm_state_t state;
    bool is_init;

    daq_ctx_t daq_ctx;
    util_time_t *p_time;

    daq_data_t pack_data;
    eval_con_t pack_con;
} bms_tm_t;

static bms_tm_t s_tm;

bms_tm_status_t BMS_TM_Init(const bms_tm_cfg_t *p_cfg)
{
    bms_tm_status_t status = BMS_TM_NOT_INIT;

    if(!s_tm.is_init) // this is ok to check at init due to is_init being static member of struct
    {
        if(p_cfg != NULL)
        {
            s_tm.p_time = p_cfg->time_cfg;
            s_tm.daq_ctx.new_timeout = p_cfg->timeout_cfg;

            s_tm.state = BMS_TM_STATE_IDLE;

            // after all linkages are finished
            s_tm.is_init = true;
            status = BMS_TM_OK;
        }
        else
        {
            status = BMS_TM_NULL_PTR;
        }
    }
    else
    {
        status = BMS_TM_DBL_INIT;
    }

    return status;
}

bms_tm_status_t BMS_TM_Task(void)
{
    bms_tm_status_t status = BMS_TM_OK;
    daq_status_t daq_status;
    eval_status_t eval_status;

    bool is_daq_ready = false;

    if(s_tm.is_init)
    {
        daq_status = BMS_DAQ_Task(&s_tm.daq_ctx);

        if(daq_status != DAQ_OK)
        {
            status = BMS_TM_DAQ_FAULT;
        }

        if(status == BMS_TM_OK)
        {
            daq_status = BMS_DAQ_Get_Ready_Flag(&is_daq_ready);
            
            if((daq_status == EVAL_OK) && (is_daq_ready))
            {
                eval_status = BMS_Eval_Task(&s_tm.pack_data, &s_tm.pack_con);

                if(eval_status != EVAL_OK)
                {
                    status = BMS_TM_EVAL_FAULT;
                }
            }
        }

        if(s_tm.state >= BMS_TM_STATE_MAX)
        {
            status = BMS_TM_UNDEF_STATE;

            s_tm.state = BMS_TM_STATE_ERROR;
        }

        switch(s_tm.state)
        {
            /* HV OFF*/
            case BMS_TM_STATE_IDLE:
            {
                break;
            }

            /* HV ON, before closing AIR*/
            case BMS_TM_STATE_PRECHG:
            {
                break;
            }

            case BMS_TM_STATE_CHG:
            {
                break;
            }

            case BMS_TM_STATE_DCHG:
            {
                break;
            }

            /* Opens AIRs and reports state when possible*/
            case BMS_TM_STATE_FAULT:
            {
                break;
            }

            /* This will likely kick into fault state in most cases but provides space for error handling*/
            case BMS_TM_STATE_ERROR:
            {
                break;
            }

            default:
            {
                s_tm.state = BMS_TM_STATE_ERROR;

                break;
            }

        }
    }
    else 
    {
        status = BMS_TM_NOT_INIT;
    }

    return status;
}

