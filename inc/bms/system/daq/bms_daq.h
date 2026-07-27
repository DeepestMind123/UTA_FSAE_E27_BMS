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
#include <stdlib.h>

#include "util_time.h"
#include "util_const.h"
#include "util_irq.h"
#include "dev_isense.h"
#include "dev_vsense.h"
#include "dev_tsense.h"
#include "bms_config.h"
#include "bms_data.h"

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
    DAQ_IRQ_FAULT,
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

typedef enum
{
    // order these in order of priority
    DAQ_SENSOR_MIN = 0U,
    DAQ_ISENSE,
    DAQ_VSENSE,
    DAQ_TSENSE,
    DAQ_SENSOR_MAX
} daq_sensors_t;

typedef struct
{
    uint32_t timeout_ms;
    uint32_t delay_ms;
    uint32_t start_time;
    bool data_valid;
    daq_state_t dsd_state;
    daq_status_t timeout_fault;
} daq_sensor_ctx_t;

typedef struct
{
    daq_sensor_ctx_t sensor_ctx[DAQ_SENSOR_MAX];
} daq_system_ctx_t;

typedef struct
{
    uint32_t timeout_ms;
    uint32_t delay_ms;
} daq_sensor_ctx_in_t;

typedef struct
{
    daq_sensor_ctx_in_t sensor_ctx_in[DAQ_SENSOR_MAX];
} daq_system_ctx_in_t;

typedef struct
{
    daq_system_ctx_t sensor_init_ctx;
    daq_data_t *out_mem;
    const util_time_t *time_cfg;
    isense_t *isense_high_cfg;
    isense_t *isense_low_cfg;
    vsense_t *vsense_cfg;
    tsense_t *tsense_cfg;
    const util_irq_t *irq_cfg;
} daq_cfg_t;

daq_status_t BMS_DAQ_Init(const daq_cfg_t *p_cfg);

daq_status_t BMS_DAQ_Task(const daq_system_ctx_in_t *p_in);

daq_status_t BMS_DAQ_Map_Ctx(const daq_system_ctx_in_t *p_in);

daq_status_t BMS_DAQ_Idle_State(void);

daq_status_t BMS_DAQ_Isense_State(void);

daq_status_t BMS_DAQ_Isense_Switch_Task(void);

daq_status_t BMS_DAQ_Vsense_State(void);

daq_status_t BMS_DAQ_Map_Vdata(const vsense_val_t (*p_in)[SMALL_ARR_16], vsense_data_t *p_out);

daq_status_t BMS_DAQ_Tsense_State(void);

daq_status_t BMS_DAQ_Map_Tdata(const tsense_val_t (*p_in)[SMALL_ARR_16], tsense_data_t *p_out);

daq_status_t BMS_DAQ_Report_State(void);

daq_status_t BMS_DAQ_Get_Data(daq_data_t *p_out);

daq_status_t BMS_DAQ_Get_Ready_Flag(bool *p_out);

#endif