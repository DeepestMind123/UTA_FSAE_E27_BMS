/**
 * @file bms_manager.c
 * @author notwe
 * @date 2026-05-03
 * @brief system manager source
 */

#include <stddef.h>

#include "bms_manager.h"
#include "bms_fault.h"

bms_state_t state;
bool is_init;

bms_status_t BMS_Manager_Init(void)
{
    bms_status_t status = BMS_STATUS_OK;

    /*
        put configs for bms here
    */

    is_init = true;

    return status;
}

bms_status_t BMS_Manager_Task(void)
{
    bms_status_t status = BMS_STATUS_OK;

    if(is_init)
    {
        if(BMS_Is_Fault())
        {
            state = BMS_STATE_FAULT;

            status = BMS_STATUS_ERROR_FAULT;
        }

        switch(state)
        {
            case BMS_STATE_IDLE:
            {
                break;
            }

            /*
            need to change sensor waits to idle time
            */

            case BMS_STATE_CHARGE:
            {
                break;
            }

            /*
            current/volt/temp measurement periods are same for discharge
            */

            case BMS_STATE_DISCHARGE:
            {
                break;
            }

            case BMS_STATE_BALANCE:
            {
                break;
            }

            /*
            current measurements don't matter as much here
            its mostly working off of the data from cell voltage measurements
            so there should probably be a new wait period for this
            */

            case BMS_STATE_UNDEFINED:
            {
                status = BMS_STATUS_ERROR_UNDEFINED_STATE;

                state = BMS_STATE_FAULT;

                break;
            }

            case BMS_STATE_FAULT:
            {
                break;
            }

            /*
            measurements still need to happen while battery is latched
            so period will be the same as charge/discharge
            */
        }
    }
    else 
    {
        status = BMS_STATUS_ERROR_NOT_INIT;
    }

    return status;
}
