/**
 * @file dev_vsense.h
 * @author notwe
 * @date 2026-07-03
 * @brief generic voltage sensor device driver wrapper header
 * @todo rework timeout faults
*/

#ifndef DEV_VSENSE_H
#define DEV_VSENSE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "util_time.h"
#include "util_const.h"
#include "io_adc.h"

typedef enum
{
    VSENSE_OK = 0U,
    VSENSE_NULL_PTR,
    VSENSE_NULL_FUNC,
    VSENSE_NOT_INIT,
    VSENSE_DBL_INIT,
    VSENSE_TIMEOUT,
    VSENSE_BUSY,
    VSENSE_TIME_FAULT,
    VSENSE_ADC_FAULT,
    VSENSE_UNDEF_STATE,
    VSENSE_MISMATCH_STATE,
    VSENSE_INVALID_CFG,
    VSENSE_CONVERSION_FAIL,
    VSENSE_UNKNOWN_ERROR,
    VSENSE_STATUS_MAX
} vsense_status_t;

typedef enum
{
    VSENSE_STATE_UNDEF = 0U,
    VSENSE_STATE_IDLE,
    VSENSE_STATE_START,
    VSENSE_STATE_WAIT,
    VSENSE_STATE_GET,
    VSENSE_STATE_READY,
    VSENSE_STATE_ERROR,
    VSENSE_STATE_MAX
} vsense_state_t;

typedef struct 
{
    uint16_t cells_mV[LARGE_ARR_32];
} vsense_val_t;

typedef struct
{
    uint32_t timeout_ms;
    uint8_t cell_num;
    uint8_t ic_num;
    bool gives_real_val;
} vsense_ctx_t;

typedef struct
{
    vsense_val_t vsense_raw_vals[SMALL_ARR_16];
    vsense_val_t vsense_process_vals[SMALL_ARR_16];
} vsense_system_t;

typedef struct
{
    bool(*voltage_start_open_wire)(vsense_system_t *p_system);
    bool(*voltage_start_closed_wire)(vsense_system_t *p_system);
    bool(*voltage_state)(vsense_system_t *p_system);
    bool(*vsense_get_result)(vsense_system_t *p_system);
} vsense_func_t;

typedef struct
{
    const adc_t *adc_cfg;
    const util_time_t *time_cfg;
    const vsense_func_t *func_cfg;
    const vsense_ctx_t *init_ctx;
} vsense_cfg_t;

typedef struct
{
    uint32_t start_time;

    bool is_init;
    bool is_ready;
    bool val_diff;
    bool is_balance;

    vsense_state_t state;
    vsense_status_t status;

    const vsense_ctx_t *p_ctx;
    vsense_system_t sensor;
    const vsense_func_t *p_func;
    const adc_t *p_adc;
    const util_time_t *p_time;
} vsense_t;

vsense_status_t DEV_Vsense_Init(vsense_t *p_inst,
                                const vsense_cfg_t *p_cfg);

vsense_status_t DEV_Vsense_Task(vsense_t *p_inst);

vsense_status_t DEV_Vsense_Start(vsense_t *p_inst);

vsense_status_t DEV_Vsense_Process_Raw(vsense_t *p_inst, uint16_t val, uint16_t *p_out);

vsense_status_t DEV_Vsense_Get_Val(vsense_t *p_inst, vsense_val_t (*p_out)[SMALL_ARR_16]);

vsense_status_t DEV_Vsense_Get_State(vsense_t *p_inst, vsense_state_t *p_out);

vsense_status_t DEV_Vsense_Get_Ready_Flag(vsense_t *p_inst, bool *p_out);

vsense_status_t DEV_Vsense_Set_Balance(vsense_t *p_inst, bool val);

#endif