
#include "bms_fan_ctrl.h"
#include "sys_fault.h"

fan_status_t BMS_Fan_Init(bms_fan_t *p_inst, bms_fan_cfg_t *p_cfg)
{
    fan_status_t status = FAN_STATUS_ERROR;

    if((p_inst != NULL) && (p_cfg != NULL))
    {
        p_inst->now_module_temp = 0U;
        p_inst->last_module_temp = 0U;
        p_inst->now_duty_value = 0U;
        p_inst->last_duty_value = 0U;

        status = FAN_STATUS_IDLE;
    }
    else
    {
        BMS_Fault_Update(BMS_FAULT_NULL_POINTER, true);
    }

    return status;
}

fan_status_t BMS_Fan_Task(bms_fan_t *p_inst)
{
    fan_status_t status = FAN_STATUS_ERROR;

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        p_inst->now_module_temp = 0U; // this needs to be changed to get temp from a getter function, but anyways

        int16_t temp_delta = p_inst->last_module_temp - p_inst->now_module_temp;

        



    }
    else 
    {
        BMS_Fault_Update(BMS_FAULT_NULL_POINTER, true);
    }
}