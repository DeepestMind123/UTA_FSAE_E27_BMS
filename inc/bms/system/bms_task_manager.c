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
    tm_state_t state;
    bool is_init;

    daq_timeout_t timeout;
    util_time_t *p_time;
} bms_tm_t;

static bms_tm_t s_tm;

tm_status_t BMS_Manager_Init(const tm_cfg_t *p_cfg)
{
    tm_status_t status = TM_NOT_INIT;

    if(!s_tm.is_init) // this is ok to check at init due to is_init being static member of struct
    {
        if(p_cfg != NULL)
        {
            s_tm.p_time = p_cfg->time_cfg;

            s_tm.state = TM_STATE_IDLE;

            // after all linkages are finished
            s_tm.is_init = true;
            status = TM_OK;
        }
        else
        {
            status = TM_NULL_PTR;
        }
    }
    else
    {
        status = TM_DBL_INIT;
    }

    return status;
}

tm_status_t BMS_Manager_Task(void)
{
    tm_status_t status = TM_OK;

    if(s_tm.is_init)
    {
        if(BMS_Is_Fault())
        {
            status = TM_UNDEF_STATE;

            
            s_tm.state = TM_STATE_ERROR;
        }

        if(s_tm.state >= TM_STATE_MAX)
        {

        }

        switch(s_tm.state)
        {
            case TM_STATE_IDLE:
            {
                break;
            }

            /*
            need to change sensor waits to idle time
            */

            case TM_STATE_CHARGE:
            {
                break;
            }

            /*
            current/volt/temp measurement periods are same for discharge
            */

            case TM_STATE_DISCHARGE:
            {
                break;
            }

            case TM_STATE_BALANCE:
            {
                break;
            }

            /*
            current measurements don't matter as much here since it enters this state periodically during charging
            its mostly working off of the data from cell voltage measurements
            so there should probably be a new wait period for this
            */

            case TM_STATE_ERROR:
            {
                break;
            }

            default:
            {
                s_tm.state = TM_STATE_ERROR;

                break;
            }

        }
    }
    else 
    {
        status = TM_NOT_INIT;
    }

    return status;
}

