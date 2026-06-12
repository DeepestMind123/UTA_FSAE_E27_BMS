/**
 * @file io_pwm.h
 * @author notwe
 * @date 2026-05-03
 * @brief pwm io abstraction header
 */

#ifndef IO_PWM_H
#define IO_PWM_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    PWM_STATUS_OK = 0,
    PWM_STATUS_ERROR_NOT_INIT,
    PWM_STATUS_ERROR_NULL_POINTER
} pwm_status_t;

typedef const struct
{
    void(*Load_Duty_Cycle)(uint16_t duty_value);

} io_pwm_cfg_t;

typedef struct
{
    bool is_init;

    uint16_t last_duty_value;

    pwm_status_t status;

    io_pwm_cfg_t *cfg;

} io_pwm_t;

pwm_status_t IO_PWM_Init(io_pwm_t *p_inst, const io_pwm_cfg_t *p_cfg);

pwm_status_t IO_PWM_Set_Duty_Cycle(io_pwm_t *p_inst, uint16_t duty_value);

pwm_status_t IO_PWM_Get_Duty_Cycle(io_pwm_t *p_inst, uint16_t *p_out);

#endif