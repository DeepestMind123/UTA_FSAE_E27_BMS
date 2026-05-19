#include "sys_fault.h"

void sys_fault_update(fault_id_t id, bool is_error)
{
    if(id >= FAULT_MAX)
    {
        return; // ignore errors outside bounds
    }

    if(is_error)
    {
        if(fault_table[id].counter == 0U)
        {
            fault_table[id].start_time = get_tick();
        }

        if(fault_table[id].counter < fault_table[id].threshold)
        {
            fault_table[id].counter++; // increment counter

            fault_table[id].status = FAULT_STATUS_PENDING; // fault pending successful debounce
        }
        else 
        {
            fault_table[id].status = FAULT_STATUS_ACTIVE;
        }
    }
    else if((!is_error) || ((get_tick() - fault_table[id].start_time) >= fault_table[id].max_time))
    {
        fault_table[id].counter = 0U;

        fault_table[id].status = FAULT_STATUS_CLEAR;
    }
}

fault_status_t sys_fault_get_status(fault_id_t id)
{
    fault_status_t status = FAULT_STATUS_ACTIVE; // fail-safe if fault is undefined

    if(id < FAULT_MAX)
    {
        status = fault_table[id].status; 
    }

    return status;
}