

#ifndef BMS_FAN_CTRL_H
#define BMS_FAN_CTRL_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "io_pwm.h"
#include "sys_fault.h"
// needs access to a temperature device
// like, yeah that is going to be the ltc6813 but it isnt done (yikes)

typedef enum
{
    FAN_STATUS_IDLE = 0,
    FAN_STATUS_ON,
    FAN_STATUS_ERROR
} fan_status_t;

typedef const struct
{
    int16_t setpoint;

} bms_fan_cfg_t;

typedef struct
{
    int16_t now_module_temp;
    int16_t last_module_temp;
    uint16_t now_duty_value;
    uint16_t last_duty_value;

    bool is_init;

    io_pwm_t *pwm_inst;

    fan_status_t status;

    bms_fan_cfg_t *cfg;

} bms_fan_t;

fan_status_t BMS_Fan_Init(bms_fan_t *p_inst, bms_fan_cfg_t *p_cfg);

fan_status_t BMS_Fan_Task(bms_fan_t *p_inst);


#endif