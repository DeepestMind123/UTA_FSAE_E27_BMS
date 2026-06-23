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
    ADC_STATUS_OK = 0,
    ADC_STATUS_ERROR_NULL_POINTER,
    ADC_STATUS_ERROR_NOT_INIT,
    ADC_STATUS_ERROR_TIMEOUT,
    ADC_STATUS_ERROR_ADC_BUSY,
    ADC_STATUS_ERROR_UNDEFINED_STATE
} adc_status_t;

typedef enum
{
    ADC_STATE_IDLE = 0,   // adc not taking data
    ADC_STATE_START,      // start adc if necessary
    ADC_STATE_WAIT,       // wait for adc conversion
    ADC_STATE_GET,        // get measurement and stop adc if necessary; process current
    ADC_STATE_READY,
    ADC_STATE_ERROR,
    ADC_STATE_UNDEFINED
} adc_state_t;

typedef struct
{
    uint32_t adc_timeout;                       // max time allowed before adc timeout;

    uint16_t adc_resolution;
    int16_t adc_offset;
    uint16_t adc_vref_mV;                       // reference voltage
    uint8_t channel_id;                         // if using multiple sensors
    
    void(*ADC_channel_select)(uint8_t channel); // function for setting current sensor pin
    void(*ADC_start)(void);                     // function for starting adc
    uint8_t(*ADC_done)(void);                   // function for adc state; returns 1 for done, 0 for not done
    uint32_t(*ADC_get_result)(void);            // function for getting adc result
    void(*ADC_stop)(void);                      // function for stopping adc

} io_adc_cfg_t;

typedef struct
{
    uint32_t last_raw;                      // store last value so other modules can read
    int16_t set_offset;                     // basically just the calibrated value of offset
    uint32_t start_time;

    bool is_init;

    adc_state_t state;                      // stores instanced state
    
    const io_adc_cfg_t *cfg;

} io_adc_t;

adc_status_t IO_ADC_Init(io_adc_t *p_inst, io_adc_cfg_t *p_cfg);

adc_status_t IO_ADC_Task(io_adc_t *p_inst);

adc_status_t IO_ADC_Start(io_adc_t *p_inst);

adc_status_t IO_ADC_Get_Val(io_adc_t *p_inst, uint16_t *p_out);

adc_status_t IO_ADC_Get_Resolution(io_adc_t *p_inst, uint16_t *p_out);

adc_status_t IO_ADC_Get_Offset(io_adc_t *p_inst, int16_t *p_out);

adc_status_t IO_ADC_Set_Offset(io_adc_t *p_inst, int16_t new_val);

adc_status_t IO_ADC_Get_Vref(io_adc_t *p_inst, uint16_t *p_out);

adc_status_t IO_ADC_Get_State(io_adc_t *p_inst, adc_state_t *p_out);

adc_status_t IO_ADC_Get_Timeout(io_adc_t *p_inst, uint32_t *p_out);

#endif