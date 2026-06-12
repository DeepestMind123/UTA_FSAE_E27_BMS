/**
 * @file can_io.h
 * @author notwe
 * @date 2026-05-03
 * @brief can io abstraction header file
 */

#ifndef IO_CAN_H
#define IO_CAN_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    CAN_STATUS_OK = 0,
    CAN_STATUS_ERROR_NOT_INIT,
    CAN_STATUS_ERROR_TRANSMIT_FAIL,
    CAN_STATUS_ERROR_RECEIVE_FAIL,
    CAN_STATUS_ERROR_NULL_POINTER,
    CAN_STATUS_MAX
} can_status_t;

typedef struct
{
    uint32_t id;    // 11-bit or 29-bit ID
    uint8_t dlc;    // data length (0-8 bytes)
    uint8_t data[8]; // p_msg data
    bool is_ext; // flag for extended id
} can_msg_t; 

typedef struct
{
    bool(*init)(uint32_t baudrate);
    bool(*transmit)(const can_msg_t *p_msg);
    bool(*receive)(can_msg_t *p_msg);
} io_can_cfg_t;

typedef struct
{
    bool is_init;

    io_can_cfg_t *p_cfg;
} io_can_t;

can_status_t IO_CAN_Init(io_can_t *p_inst, const io_can_cfg_t *p_cfg, uint32_t baudrate);

can_status_t IO_CAN_Transmit(io_can_t *p_inst, const can_msg_t *p_msg);

can_status_t IO_CAN_Receive(io_can_t *p_inst, can_msg_t *p_msg);

#endif