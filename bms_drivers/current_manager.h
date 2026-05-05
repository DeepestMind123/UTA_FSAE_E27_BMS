/**
 * @file main.c
 * @author notwe
 * @date 2026-05-03
 * @brief current sensor manager header
 */

#ifndef CURRENT_MANAGER_H
#define CURRENT_MANAGER_H

#include "stdint.h"
#include <stdint.h>

typedef enum
{
    CURRENT_IDLE = 0,   // sensor not making a measurement
    CURRENT_START,      // start adc if necessary
    CURRENT_WAIT,       // wait for adc conversion
    CURRENT_GET         // get measurement and stop adc if necessary; process current
} current_state_t;

typedef struct
{
    current_state_t state;  // naming state variable

    uint32_t raw;
    
    int32_t current_mA;


} current_manager_t;

typedef struct
{
    void(*set_pin)(void);               // function for setting current sensor pin
    void(*ADC_start)(void);             // function for starting adc
    uint8_t(*ADC_done)(void);           // function for adc state; returns 1 for done, 0 for not done
    uint32_t(*ADC_get_result)(void);    // function for getting adc result
    void(*ADC_stop)(void);              // function for stopping adc

} current_manager_cfg_t;

void current_manager_init(current_manager_t *p_cur, current_manager_cfg_t *p_cur_cfg);

void current_manager_task(current_manager_t *p_cur);    // state switch function

int32_t process_current(current_manager_t *p_cur, uint32_t raw);      // process raw current value to get current in mA

#endif