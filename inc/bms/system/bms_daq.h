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

typedef enum
{
    DAQ_OK = 0,
    DAQ_NOT_INIT,
    DAQ_FAULT,
    DAQ_UNDEF_STATE,
    DAQ_NULL_PTR,
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
    vsense_mod_val_t mod_val_mV[SMALL_ARR_32];
    uint32_t v_timestamp;
    bool v_valid;
} vsense_data_t;

typedef struct
{
    int16_t mod_val_dC[];
} tsense_mod_val_t;

typedef struct
{
    tsense_mod_val_t mod_val_dC[255];
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

} daq_cfg_t;

typedef struct
{
    uint32_t start_time;
    uint32_t now_time;
    bool is_init;
    daq_state_t state;
    daq_timeout_t *init_timeout;
    daq_data_t *data_buffer;
    daq_data_t *data_out;
    const util_time_t *time;
} daq_t;

daq_status_t BMS_DAQ_Init();


#endif