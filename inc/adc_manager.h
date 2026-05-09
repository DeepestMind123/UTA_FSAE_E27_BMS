/**
 * @file adc_manager.c
 * @author notwe
 * @date 2026-05-03
 * @brief adc manager header
 */

#ifndef ADC_MANAGER_H
#define ADC_MANAGER_H

#include <stdint.h>
#include <stddef.h>

typedef enum
{
    ADC_READY = 0,   // adc not taking data
    ADC_START,      // start adc if necessary
    ADC_WAIT,       // wait for adc conversion
    ADC_GET        // get measurement and stop adc if necessary; process current
} adc_state_t;

typedef struct
{
    uint16_t adc_scale;
    uint16_t adc_offset;
    uint16_t adc_vref_mV;                       // reference voltage
    uint8_t channel_id;                         // if using multiple sensors
    
    void(*ADC_channel_select)(uint8_t channel); // function for setting current sensor pin
    void(*ADC_start)(void);                     // function for starting adc
    uint8_t(*ADC_done)(void);                   // function for adc state; returns 1 for done, 0 for not done
    uint32_t(*ADC_get_result)(void);            // function for getting adc result
    void(*ADC_stop)(void);                      // function for stopping adc

} adc_manager_cfg_t;

typedef struct
{
    adc_state_t state;
    uint32_t last_raw;                      // store last value so other modules can read
    uint8_t data_ready_flag;                // data ready flag
    int16_t set_offset;

    const adc_manager_cfg_t *cfg;

} adc_manager_t;

void adc_manager_init(adc_manager_t *p_inst, adc_manager_cfg_t *p_cfg);

void adc_manager_task(adc_manager_t *p_inst);

uint8_t adc_manager_check(adc_manager_t *p_inst);

uint8_t adc_manager_get_flag(adc_manager_t *p_inst);

uint32_t adc_manager_get_val(adc_manager_t *p_inst);

uint16_t adc_manager_get_scale(adc_manager_t *p_inst);

uint16_t adc_manager_get_offset(adc_manager_t *p_inst);

void adc_manager_set_offset(adc_manager_t *p_inst, int16_t new_val);

uint16_t adc_manager_get_vref(adc_manager_t *p_inst);

#endif