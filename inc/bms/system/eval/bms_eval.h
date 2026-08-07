/**
 * @file bms_eval.h
 * @author notwe
 * @date 2026-07-08
 * @brief bms eval module header file
*/

#ifndef BMS_EVAL_H
#define BMS_EVAL_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdlib.h>

#include "bms_data.h"
#include "bms_config.h"

typedef enum
{
    EVAL_OK = 0U,
    EVAL_NULL_PTR,
    EVAL_NOT_INIT,
    EVAL_DBL_INIT,
    EVAL_IDATA_INVALID,
    EVAL_VDATA_INVALID,
    EVAL_TDATA_INVALID,
    EVAL_UNDEF_STATE,
    EVAL_STATUS_MAX
} eval_status_t;

typedef enum
{
    EVAL_STATE_UNDEF = 0U,
    EVAL_STATE_IDLE,
    EVAL_STATE_ICON,
    EVAL_STATE_VCON,
    EVAL_STATE_TCON,
    EVAL_STATE_ERROR,
    EVAL_STATE_MAX
} eval_state_t;

eval_status_t BMS_Eval_Init(void);

eval_status_t BMS_Eval_Task(const daq_data_t *p_data_in, eval_con_t *p_out);

eval_status_t BMS_Eval_Icon(const daq_data_t *p_data_in, eval_con_t *p_out);

eval_status_t BMS_Eval_Vcon(const daq_data_t *p_data_in, eval_con_t *p_out);

eval_status_t BMS_Eval_Tcon(const daq_data_t *p_data_in, eval_con_t *p_out);

eval_status_t BMS_Eval_Find_Ready(const daq_data_t *p_data_in);

eval_status_t BMS_Eval_Get_State(eval_state_t *p_out);

#endif