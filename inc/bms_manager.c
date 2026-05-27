/**
 * @file bms_manager.c
 * @author notwe
 * @date 2026-05-03
 * @brief system manager source
 */

#include "bms_manager.h"

static bms_manager_t mgr;
static bms_manager_cfg_t cfg;

void BMS_Manager_Init(bms_manager_t *p_mgr, bms_manager_cfg_t *p_mgr_cfg)
{
    // alias pointers
    mgr = *p_mgr;
    cfg = *p_mgr_cfg;

    mgr.state = BMS_STATE_IDLE;

}

void BMS_Manager_Task(bms_manager_t *p_mgr)
{
    if(1);

    switch(mgr.state)
    {
        case BMS_STATE_IDLE:

        break;

        case BMS_STATE_CHARGE:

        break;

        case BMS_STATE_DISCHARGE:

        break;

        case BMS_STATE_BALANCE:

        break;

        case BMS_STATE_FAULT:

        break;
    }   
}
