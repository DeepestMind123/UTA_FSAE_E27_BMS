/**
 * @file io_i2c.h
 * @author notwe
 * @date 2026-06-05
 * @brief i2c io abstraction header file
 */

#ifndef IO_I2C_H
#define IO_I2C_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    I2C_STATUS_OK = 0,
    I2C_STATUS_ERROR_NOT_INIT,
    I2C_STATUS_ERROR_NULL_POINTER,
    I2C_STATUS_ERROR_RECEIVE_FAIL,
    I2C_STATUS_ERROR_TRANSMIT_FAIL,
    I2C_STATUS_MAX
} i2c_status_t;

typedef struct
{
    uint8_t addr; // 7-bit address byte (can be 8-bits for some devices)
    uint8_t comm; // command bit (specific to device)
    uint8_t data; // data byte
} i2c_msg_t;

typedef struct
{
    bool(*init)(uint8_t addr);
    bool(*set_clock)(uint32_t speed);
    bool(*transmit)(const i2c_msg_t *p_msg);
    bool(*receive)(i2c_msg_t *p_msg);
} io_i2c_cfg_t;

typedef struct
{
    bool is_init;

    io_i2c_cfg_t *p_cfg;
} io_i2c_t;

i2c_status_t IO_I2C_Init(io_i2c_t *p_inst, const io_i2c_cfg_t *p_cfg, uint8_t addr);

i2c_status_t IO_I2C_Set_Clock(io_i2c_t *p_inst, uint32_t speed);

i2c_status_t IO_I2C_Transmit(io_i2c_t *p_inst, const i2c_msg_t *p_msg);

i2c_status_t IO_I2C_Receive(io_i2c_t *p_inst, i2c_msg_t *p_msg);

#endif