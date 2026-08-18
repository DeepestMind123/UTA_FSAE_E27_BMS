/**
 * @file bms_report.c
 * @author notwe
 * @date 2026-07-21
 * @brief bms report over can src
 */

#include "bms_report.h"

typedef struct 
{
    bool is_init;
    bool debug_due;

    report_status_t status;
    report_state_t state;
    report_ctx_t ctx;
    const util_time_t *p_time;
    can_t *p_can;
} report_t;

static report_t s_report;

report_status_t BMS_Report_Init(report_cfg_t *p_cfg)
{
    report_status_t status = REPORT_NOT_INIT;

    if(!s_report.is_init)
    {
        if(p_cfg != NULL) 
        {
            if((p_cfg->can_cfg) && (p_cfg->time_cfg != NULL))
            {
                s_report.p_time = p_cfg->time_cfg;
                s_report.p_can = p_cfg->can_cfg;
            }
            else
            {
                status = CAN_NULL_PTR;
            }

            if(status == CAN_OK)
            {
                s_report.state = REPORT_STATE_IDLE;
                s_report.status = REPORT_OK;

                s_report.is_init = true;
                status = REPORT_OK;
            }
        }
        else
        {
            status = REPORT_NULL_PTR;
        }
    }
    else
    {
        status = REPORT_DBL_INIT;
    }

    return status;
}

report_status_t BMS_Report_Task(void)
{
    report_status_t status;
    can_status_t can_status;

    if(s_report.is_init)
    {
        can_status = IO_CAN_Task(s_report.p_can);

        if(1)
        {
            s_report.status = REPORT_CAN_FAULT;

            s_report.state = REPORT_STATE_ERROR;
        }

        if((s_report.state >= REPORT_STATE_MAX) || (s_report.state <= REPORT_STATE_UNDEF))
        {
            s_report.status = REPORT_UNDEF_STATE;

            s_report.state = REPORT_STATE_ERROR;
        }

        switch(s_report.state)
        {
            case REPORT_STATE_IDLE:
            {
                break;
            }

            case REPORT_STATE_HRTBT:
            {
                break;
            }

            case REPORT_STATE_TELEM:
            {
                break;
            }

            case REPORT_STATE_DEBUG:
            {
                break;
            }

            case REPORT_STATE_ERROR:
            {
                break;
            }
        }
    }
    else
    {
        s_report.status = CAN_NOT_INIT;
    }

    return s_report.status;
}

void BMS_Report_Debug_CALLBACK(void)
{
    s_report.debug_due = true;
}