/**
 * @file bms_manager.c
 * @author notwe
 * @date 2026-05-03
 * @brief system manager source
 */

#include "bms_task_manager.h"

static bms_state_t state;
static bms_discharge_state_t discharge_state;
static dev_current_sensor_t *current_sensor_high;
static dev_current_sensor_t *current_sensor_low;
static dev_current_sensor_t *last_current_sensor;
static current_sensor_status_t current_sensor_status;
static bms_config_t *bms_cfg; 
static bms_pack_state_t pack_state = {
    .last_current_mA = 0,
    .last_time_ms = 0U
};

static bool is_init;

bms_status_t BMS_Manager_Init(void)
{
    bms_status_t status = BMS_STATUS_ERROR_NOT_INIT;

    /*
        put configs for bms here
    */
    if((current_sensor_high != NULL) && (current_sensor_low != NULL) && (last_current_sensor != NULL) && (bms_cfg != NULL))
    {
        last_current_sensor = current_sensor_low;

        is_init = true;

        status = BMS_STATUS_OK;
    }
    else
    {
        status = BMS_STATUS_ERROR_NULL_POINTER;
    }

    return status;
}

bms_status_t BMS_Manager_Task(void)
{
    bms_status_t status = BMS_STATUS_OK;

    if(is_init)
    {
        if(BMS_Is_Fault())
        {
            status = BMS_STATUS_ERROR_FAULT;

            state = BMS_STATE_FAULT;

            // call error handler
        }

        if(state >= BMS_STATE_MAX)
        {
            state = BMS_STATE_UNDEFINED;
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
                status = BMS_Manager_Discharge_Task();

                if(status != BMS_STATUS_OK)
                {
                    state = BMS_STATE_FAULT;
                }

                break;
            }

            case BMS_STATE_BALANCE:
            {
                break;
            }

            /*
            current measurements don't matter as much here since it enters this state periodically during charging
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

bms_status_t BMS_Manager_Discharge_Task(void)
{
    bms_status_t status = BMS_STATUS_OK;

    if(is_init)
    {
        if(discharge_state >= BMS_DISCHARGE_STATE_MAX)
        {
            discharge_state = BMS_DISCHARGE_STATE_UNDEFINED;
        }

        switch(discharge_state)
        {
            case BMS_DISCHARGE_STATE_IDLE:
            {
                status = BMS_Manager_Current_Sensor_Task();

                if(status != BMS_STATUS_OK)
                {
                    discharge_state = BMS_DISCHARGE_STATE_ERROR;
                }

                // need to add bms ic task as well

                break;
            }

            case BMS_DISCHARGE_STATE_REPORT:
            {
                break;
            }

            case BMS_DISCHARGE_STATE_UNDEFINED:
            {
                status = BMS_STATUS_ERROR_UNDEFINED_STATE;

                discharge_state = BMS_DISCHARGE_STATE_ERROR;

                break;
            }

            case BMS_DISCHARGE_STATE_ERROR:
            {
                break;
            }
        }
    }
    else
    {
        status = BMS_STATUS_ERROR_NOT_INIT;
    }

    return status;
}

bms_status_t BMS_Manager_Current_Sensor_Task(void)
{
    bms_status_t status = BMS_STATUS_OK;

    if(pack_state.last_current_mA > SENSOR_HANDOFF_MAX)
    {
        last_current_sensor = current_sensor_high;

        current_sensor_status = DEV_Current_Sensor_Set_Timeout(&current_sensor_low, 0U);
    }
    else if(pack_state.last_current_mA <= SENSOR_HANDOFF_MIN)
    {
        last_current_sensor = current_sensor_low;

        current_sensor_status = DEV_Current_Sensor_Set_Timeout(&current_sensor_high, 0U);
    }

    current_sensor_status = DEV_Current_Sensor_Task(&last_current_sensor);

    if(current_sensor_status != CURRENT_SENSOR_OK)
    {
        status = BMS_STATUS_ERROR_CURRENT_SENSOR_FAILURE;
    }

    return status;
}