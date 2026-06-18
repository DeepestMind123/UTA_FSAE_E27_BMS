/**
 * @file dev_fan.h
 * @author notwe
 * @date 2026-06-12
 * @brief fan driver header
 */

#ifndef BMS_FAN_CTRL_H
#define BMS_FAN_CTRL_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "util_time.h"
#include "io_pwm.h"
// needs access to a temperature device
// like, yeah that is going to be the ltc6813 but it isnt done (yikes)
// also need to make a schedule controller that defines time limits;

const float K_P = 10.0f;
const float K_I = 1.0f;
const float INTEGRAL_MAX = 100.0f;
const float INTEGRAL_MIN = 0.0f;
const float PWM_MAX = 100.0f;
const float PWM_MIN = 0.0f;

const uint16_t PWM_MAX_INT = 65535;

typedef enum
{
    FAN_STATUS_OK = 0,
    FAN_STATUS_ERROR_NOT_INIT,
    FAN_STATUS_ERROR_NULL_POINTER
} fan_status_t;

typedef const struct
{
    float setpoint;

} dev_fan_cfg_t;

typedef struct
{
    float now_temp_C;
    uint16_t duty_value;
    uint32_t now_time_ms;
    uint32_t last_time_ms;

    float error;

    bool is_init;

    io_pwm_t *pwm_inst;

    fan_status_t status;

    dev_fan_cfg_t *cfg;

} dev_fan_t;

fan_status_t DEV_Fan_Init(dev_fan_t *p_inst, const dev_fan_cfg_t *p_cfg, const io_pwm_t *p_pwm_inst);

fan_status_t DEV_Fan_Task(dev_fan_t *p_inst);


#endif