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
    EVAL_IDATA_INVALID,
    EVAL_VDATA_INVALID,
    EVAL_TDATA_INVALID,
    EVAL_STATUS_MAX
} eval_status_t;

eval_status_t BMS_Eval_Task(const daq_data_t *p_data_in, eval_con_t *p_out);

eval_status_t BMS_Eval_Icon(const daq_data_t *p_data_in, eval_con_t *p_out);

eval_status_t BMS_Eval_Vcon(const daq_data_t *p_data_in, eval_con_t *p_out);

eval_status_t BMS_Eval_Tcon(const daq_data_t *p_data_in, eval_con_t *p_out);

#endif