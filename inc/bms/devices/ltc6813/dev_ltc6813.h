/**
 * @file bms_config.h
 * @author notwe
 * @date 2026-06-08
 * @brief idc driver header
 */

#ifndef DEV_LTC6813_H
#define DEV_LTC6813_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    LTC6813_STATE_IDLE = 0,
    LTC6813_STATE_START,
    LTC6813_STATE_WAIT,
    LTC6813_STATE_GET,
    LTC6813_STATE_READY
} dev_ltc6813_state_t;

typedef enum
{
    LTC6813_CMD_NONE = 0,
    LTC6813_CMD_WRITE_CFG,
    LTC6813_CMD_POLL_CELLS,
    LTC6813_CMD_POLL_GPIOS,
    LTC6813_CMD_BALANCE

} dev_ltc6813_cmd_t;

typedef struct
{
    uint8_t cs_pin;

    uint8_t ic_num; // number of daisy-chained devices]
    uint8_t cell_num; // number of cells connected to device


} dev_ltc6813_cfg_t;

typedef struct
{
    uint8_t md;        // adc mode (0 - slow, 1 - fast, 2 - normal, 3 - filtered)
    bool dcp;          // discarge permitted
    uint8_t ch;        // cell selection for adc conversion
    bool pup;          // pull-up(1)/pull-down(0) current for open wire conversion
    uint8_t st;        // self test mode select
    uint8_t chg;       // GPIO select for ADC conversion
    uint8_t chst;       // status group select (idek what these are atm)

    uint8_t cfgar[6]; // cfg reg A
    uint8_t cfgbr[6];  // cfg reg B
    uint8_t cvar[6];   // cell voltage reg A
    uint8_t cvbr[6];   // cell voltage reg B
    uint8_t cvcr[6];   // cell voltage reg C
    uint8_t cvdr[6];   // cell voltage reg D
    uint8_t cver[6];   // cell voltage reg E
    uint8_t cvfr[6];   // cell voltage reg F
    uint8_t avar[6];   // aux reg A 
    uint8_t avbr[6];   // aux reg B
    uint8_t avcr[6];   // aux reg C
    uint8_t avdr[6];   // aux reg D
    uint8_t star[6];   // status reg A
    uint8_t stbr[6];   // status reg B
    uint8_t comm[6];   // comm reg
    uint8_t sctrl[6];  // S control reg
    uint8_t pwmr[6];   // PWM control reg
    uint8_t psr[6];    // PWM/S control reg (why does this exist?) (there's already a PWM and an S pin control register)

    uint8_t cmd_packet[4];
    uint8_t len;

    uint8_t cmd_buffer; // stores next command
    uint16_t cv_buffer[18]; // stores the last polled cell voltages
    uint16_t aux_buffer[12]; // stores the last polled gpio values

    bool busy_flag; // true if command is currently being excecuted
    bool ready_flag; // true if buffer is full

    uint32_t delay_ms;
    uint32_t elapsed_ms;

    dev_ltc6813_cfg_t *cfg;

    dev_ltc6813_state_t state;
    dev_ltc6813_cmd_t cmd;

} dev_ltc6813_t;

void DEV_LTC6813_Init(dev_ltc6813_t *p_inst, const dev_ltc6813_cfg_t *p_cfg);

void DEV_LTC6813_Task(dev_ltc6813_t *p_inst);

bool DEV_LTC6813_CMD_Request(dev_ltc6813_t *p_inst, dev_ltc6813_cmd_t cmd);

void DEV_LTC6813_Poll_Cells(dev_ltc6813_t *p_inst);

void DEV_LTC6813_Get_CMD_Packet(dev_ltc6813_t *p_inst, uint16_t val1, uint16_t val2);


#endif