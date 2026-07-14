/**
 * @file bms_daq.h
 * @author notwe
 * @date 2026-07-08
 * @brief bms daq task header file
*/

#ifndef BMS_DAQ_H
#define BMS_DAQ_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>

#include "util_time.h"
#include "util_const.h"
#include "dev_current_sensor.h"
#include "dev_voltage_sensor.h"
#include "dev_temp_sensor.h"
#include "bms_config.h"

#define HANDOFF_MAX_MA_ISENSE 7500
#define HANDOFF_MIN_MA_ISENSE 7000
#define TASK_DELAY_ISENSE 2U
#define TASK_DELAY_VSENSE 25U
#define TASK_DELAY_TSENSE 100U

typedef enum
{
    DAQ_OK = 0,
    DAQ_NOT_INIT,
    DAQ_DBL_INIT,
    DAQ_FAULT,
    DAQ_UNDEF_STATE,
    DAQ_NULL_PTR,
    DAQ_ISENSE_FAULT,
    DAQ_ISENSE_TIMEOUT,
    DAQ_VSENSE_FAULT,
    DAQ_VSENSE_TIMEOUT,
    DAQ_TSENSE_FAULT,
    DAQ_TSENSE_TIMEOUT,
    DAQ_TIME_FAULT,
    DAQ_STATUS_MAX
} daq_status_t;

typedef enum
{
    DAQ_STATE_UNDEF = 0,
    DAQ_STATE_IDLE,
    DAQ_STATE_ISENSE,
    DAQ_STATE_VSENSE,
    DAQ_STATE_TSENSE,
    DAQ_STATE_REPORT,
    DAQ_STATE_ERROR,
    DAQ_STATE_MAX
} daq_state_t;

typedef struct
{
    int32_t ival_mA;
    uint32_t i_timestamp;
    bool i_valid;
} isense_data_t;

typedef struct
{
    int16_t cell_val_mV[LARGE_ARR_64];
} vsense_mod_val_t;

typedef struct
{
    vsense_mod_val_t modv[SMALL_ARR_32];
    uint32_t v_timestamp;
    bool v_valid;
} vsense_data_t;

typedef struct
{
    float tsense_val_C[SMALL_ARR_32];
} tsense_mod_val_t;

typedef struct
{
    tsense_mod_val_t modt[SMALL_ARR_32];
    uint32_t t_timestamp;
    bool t_valid;
} tsense_data_t;

typedef struct 
{
    isense_data_t i_data;
    vsense_data_t v_data;
    tsense_data_t t_data;
} daq_data_t;

typedef struct
{
    uint32_t isense_timeout;
    uint32_t isense_ready_timeout;
    uint32_t vsense_timeout;
    uint32_t vsense_ready_timeout;
    uint32_t tsense_timeout;
    uint32_t tsense_ready_timeout;
} daq_timeout_t;

typedef struct
{
    daq_timeout_t new_timeout;
} daq_ctx_t;

typedef struct
{
    daq_timeout_t timeout_cfg;
    daq_data_t *out_mem;
    const util_time_t *time_cfg;
    const isense_t *isense_high_cfg;
    const isense_t *isense_low_cfg;
} daq_cfg_t;

daq_status_t BMS_DAQ_Init(const daq_cfg_t *p_cfg);

daq_status_t BMS_DAQ_Task(void);

daq_status_t BMS_DAQ_Idle_State(void);

daq_status_t BMS_DAQ_Isense_State(void);

daq_status_t BMS_DAQ_Isense_Switch_Task(void);

daq_status_t BMS_DAQ_Vsense_State(void);

daq_status_t BMS_DAQ_Map_Vdata(const vsense_val_t (*p_in)[SMALL_ARR_32], vsense_data_t *p_out);

daq_status_t BMS_DAQ_Tsense_State(void);

daq_status_t BMS_DAQ_Map_Tdata(const tsense_val_t (*p_in)[SMALL_ARR_32], tsense_data_t *p_out);

daq_status_t BMS_DAQ_Report_State(void);

#endif