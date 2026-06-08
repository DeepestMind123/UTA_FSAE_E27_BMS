#include "bms_fault.h"

void BMS_Fault_Update(bms_fault_id_t id, bool is_error)
{
    if(id >= BMS_FAULT_MAX)
    {
        return; // ignore errors outside bounds
    }

    if(is_error)
    {
        if(bms_fault_table[id].counter == 0U)
        {
            bms_fault_table[id].start_time = UTIL_Time_Get_Tick();
        }

        if(bms_fault_table[id].counter < bms_fault_table[id].threshold)
        {
            bms_fault_table[id].counter++; // increment counter

            bms_fault_table[id].status = BMS_FAULT_STATUS_PENDING; // fault pending successful debounce
        }
        else 
        {
            bms_fault_table[id].status = BMS_FAULT_STATUS_ACTIVE;
        }
    }
    else if((!is_error) || ((UTIL_Time_Get_Tick() - bms_fault_table[id].start_time) >= bms_fault_table[id].max_time))
    {
        bms_fault_table[id].counter = 0U;

        bms_fault_table[id].status = BMS_FAULT_STATUS_CLEAR;
    }
}

bms_fault_status_t BMS_Fault_Get_Status(bms_fault_id_t id)
{
    bms_fault_status_t status = BMS_FAULT_STATUS_ACTIVE; // fail-safe if fault is undefined

    if(id < BMS_FAULT_MAX)
    {
        status = bms_fault_table[id].status; 
    }

    return status;
}

bool BMS_Is_Fault(void)
{
    bool is_fault = false;

    for(uint8_t i = 0; i < BMS_FAULT_MAX; i++)
    {
        if(bms_fault_table[i].status == BMS_FAULT_STATUS_ACTIVE)
        {
            is_fault = true;
        }
    }

    return is_fault;
}