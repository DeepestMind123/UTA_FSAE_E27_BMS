/**
 * @file util_pid_ctrl.h
 * @author notwe
 * @date 2026-06-12
 * @brief generic pid ctrl header
 */

#ifndef UTIL_PID_CTRL_H
#define UTIL_PID_CTRL_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "util_time.h"

const float K_P = 10.0f;
const float K_I = 1.0f;
const float K_D = 1.0f;
const float INTEGRAL_MAX = 100.0f;
const float INTEGRAL_MIN = 0.0f;
const float PWM_MAX = 100.0f;
const float PWM_MIN = 0.0f;

const uint16_t PWM_MAX_INT = 65535;

typedef enum
{
    PID_STATUS_OK = 0,
    PID_STATUS_ERROR_NOT_INIT,
    PID_STATUS_ERROR_NULL_POINTER,
    PID_STATUS_ERROR_TIME_ERROR,
    PID_STATUS_MAX
} pid_status_t;

typedef const struct
{
    float setpoint;

} pid_ctrl_cfg_t;

typedef struct
{
    float now_val;
    uint16_t duty_value;
    uint32_t now_time_ms;
    uint32_t last_time_ms;

    float error;

    bool is_init;

    pid_status_t status;

    const util_time_t *time_inst;

    const pid_ctrl_cfg_t *cfg;

} pid_ctrl_t;

pid_status_t UTIL_PID_Ctrl_Init(pid_ctrl_t *p_inst, const pid_ctrl_cfg_t *p_cfg, const util_time_t *p_time_inst);

pid_status_t UTIL_PID_Ctrl_Get_Val(pid_ctrl_t *p_inst, float *p_out);

#endif