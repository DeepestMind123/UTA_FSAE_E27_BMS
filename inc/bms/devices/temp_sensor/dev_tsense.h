/**
 * @file dev_temp_sensor.h
 * @author notwe
 * @date 2026-07-03
 * @brief generic temp sensor device driver wrapper header
 * @todo rework timeout faults
*/

#ifndef DEV_TSENSE_H
#define DEV_TSENSE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>

#include "util_time.h"
#include "util_const.h"
#include "io_adc.h"

typedef enum
{
    TSENSE_OK = 0,
    TSENSE_NULL_PTR,
    TSENSE_NULL_FUNC,
    TSENSE_NOT_INIT,
    TSENSE_TIMEOUT,
    TSENSE_TIME_FAULT,
    TSENSE_ADC_FAULT,
    TSENSE_UNDEF_STATE,
    TSENSE_MISMATCH_STATE,
    TSENSE_BUSY,
    TSENSE_CONVERSION_FAIL,
    TSENSE_STATUS_MAX
} tsense_status_t;

typedef enum
{
    TSENSE_STATE_UNDEF = 0,
    TSENSE_STATE_IDLE,
    TSENSE_STATE_START,
    TSENSE_STATE_WAIT,
    TSENSE_STATE_GET,
    TSENSE_STATE_READY,
    TSENSE_STATE_ERROR,
    TSENSE_STATE_MAX
} tsense_state_t;

typedef struct 
{
    float temps_C[LARGE_ARR_32];
} tsense_val_t;

typedef struct 
{
    uint32_t init_timeout;
    uint8_t temp_num;
    uint8_t ic_num;
    bool gives_real_val;
    float beta;
    float temp_rs;
    float temp_rn;
    float temp_nom_K;
}tsense_cfg_t;

typedef struct
{
    const tsense_cfg_t *cfg;
    tsense_val_t tsense_raw_vals[SMALL_ARR_16];
    tsense_val_t tsense_process_vals[SMALL_ARR_16];
} tsense_system_t;

typedef struct
{
    bool(*temp_start)(tsense_system_t *p_sys);
    bool(*temp_state)(tsense_system_t *p_sys);
    bool(*temp_stop)(tsense_system_t *p_sys);
    bool(*temp_get_result)(tsense_system_t *p_sys);
} tsense_func_t;

typedef struct
{
    uint32_t start_time;
    uint32_t tsense_timeout_ms;

    bool is_init;
    bool is_ready;
    bool val_diff;
    
    tsense_system_t sensor;
    const tsense_func_t *func;
    tsense_state_t state;
    const io_adc_t *adc;
    const util_time_t *time;
} tsense_t;

tsense_status_t DEV_Tsense_Init(tsense_t *p_inst, 
                                const tsense_cfg_t *p_cfg,
                                const tsense_func_t *p_func,
                                const io_adc_t *p_adc,
                                const util_time_t *p_time);

tsense_status_t DEV_Tsense_Task(tsense_t *p_inst);

tsense_status_t DEV_Tsense_Start(tsense_t *p_inst);

tsense_status_t DEV_Tsense_Process_Raw(tsense_t *p_inst, uint16_t p_in, float *p_out);

tsense_status_t DEV_Tsense_Get_Val(tsense_t *p_inst, tsense_val_t (*p_out)[SMALL_ARR_16]);

tsense_status_t DEV_Tsense_Get_State(tsense_t *p_inst, tsense_state_t *p_out);

tsense_status_t DEV_Tsense_Get_Ready_Flag(tsense_t *p_inst, bool *p_out);

#endif