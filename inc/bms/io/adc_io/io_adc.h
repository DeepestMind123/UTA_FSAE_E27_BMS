/**
 * @file io_adc.h
 * @author notwe
 * @date 2026-05-03
 * @brief abstract adc i/o header
 */

#ifndef IO_ADC_H
#define IO_ADC_H

#include "util_time.h"

typedef enum
{
    ADC_OK = 0U,
    ADC_NULL_PTR,
    ADC_NULL_FUNC,
    ADC_NOT_INIT,
    ADC_DBL_INIT,
    ADC_TIMEOUT,
    ADC_BUSY,
    ADC_TIME_FAULT,
    ADC_UNDEF_STATE,
    ADC_INVALID_CFG,
    ADC_STATUS_MAX
} adc_status_t;

typedef enum
{
    ADC_STATE_UNDEF = 0U,
    ADC_STATE_IDLE,             // adc not taking data
    ADC_STATE_START,            // start adc if necessary
    ADC_STATE_WAIT,             // wait for adc conversion
    ADC_STATE_GET,              // get measurement and stop adc if necessary; process current
    ADC_STATE_READY,
    ADC_STATE_ERROR,
    ADC_STATE_MAX
} adc_state_t;

typedef struct 
{
    void(*ADC_channel_select)(uint8_t channel); // function for setting current sensor pin
    void(*ADC_start)(void);                     // function for starting adc
    bool(*ADC_done)(void);                   // function for adc state; returns 1 for done, 0 for not done
    uint16_t(*ADC_get_result)(void);            // function for getting adc result
    void(*ADC_stop)(void);                      // function for stopping adc
    // need to add channel switching
} adc_func_t;

typedef struct
{
    uint32_t adc_timeout;                       // max time allowed in a given state before adc timeout;
    uint32_t adc_wait_ms;
    uint16_t adc_resolution;
    uint16_t adc_vref_mV;                       // reference voltage
    int16_t adc_offset;
    uint8_t channel_id;                         // if using multiple sensors
} adc_ctx_t;

typedef struct
{  
    adc_func_t adc_func_cfg;
    adc_ctx_t ctx_cfg;
    util_time_t *time_cfg;
} adc_cfg_t;

typedef struct
{
    uint32_t start_time;
    uint16_t last_raw;                      // store last value so other modules can read
    int16_t set_offset;                     // basically just the calibrated value of offset
    bool is_init;
    bool is_ready;
    adc_state_t state;                      // stores instanced state

    adc_ctx_t adc_ctx;
    adc_func_t adc_func;
    util_time_t *p_time_inst;
} adc_t;

adc_status_t IO_ADC_Init(adc_t *p_inst, const adc_cfg_t *p_cfg);

adc_status_t IO_ADC_Task(adc_t *p_inst);

adc_status_t IO_ADC_Start(adc_t *p_inst);

adc_status_t IO_ADC_Get_Val(adc_t *p_inst, uint16_t *p_out);

adc_status_t IO_ADC_Get_Resolution(adc_t *p_inst, uint16_t *p_out);

adc_status_t IO_ADC_Get_Offset(adc_t *p_inst, int16_t *p_out);

adc_status_t IO_ADC_Set_Offset(adc_t *p_inst, int16_t new_val);

adc_status_t IO_ADC_Get_Vref(adc_t *p_inst, uint16_t *p_out);

adc_status_t IO_ADC_Get_State(adc_t *p_inst, adc_state_t *p_out);

adc_status_t IO_ADC_Get_Timeout(adc_t *p_inst, uint32_t *p_out);

adc_status_t IO_ADC_Get_Ready_Flag(adc_t *p_isnt, bool *p_out);

#endif