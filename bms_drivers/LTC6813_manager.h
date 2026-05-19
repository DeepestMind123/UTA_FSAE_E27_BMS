#ifndef LTC6813_MANAGER_H
#define LTC6813_MANAGER_H

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>

#include "LTC6813.h"
#include "spi_hal.h"
#include "ADI.h"
#include "sys_time.h"

typedef enum
{
    LTC6813_STATE_IDLE = 0,
    LTC6813_STATE_START,
    LTC6813_STATE_WAIT,
    LTC6813_STATE_GET,
    LTC6813_STATE_READY
} LTC6813_state_t;

typedef enum
{
    LTC6813_CMD_NONE = 0,
    LTC6813_CMD_WRITE_CFG,
    LTC6813_CMD_POLL_CELLS,
    LTC6813_CMD_POLL_GPIOS,
    LTC6813_CMD_BALANCE

} LTC6813_cmd_t;

typedef struct
{
    uint8_t cs_pin;

    uint8_t ic_num; // number of daisy-chained devices]
    uint8_t cell_num; // number of cells connected to device


} LTC6813_manager_cfg_t;

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

    uint8_t command_packet[4];
    uint8_t len;

    uint8_t command_buffer; // stores next command
    uint16_t cv_buffer[18]; // stores the last polled cell voltages
    uint16_t aux_buffer[12]; // stores the last polled gpio values

    bool busy_flag; // true if command is currently being excecuted
    bool ready_flag; // true if buffer is full

    uint32_t delay_ms;
    uint32_t elapsed_ms;

    LTC6813_manager_cfg_t *cfg;

    LTC6813_state_t state;
    LTC6813_cmd_t cmd;

} LTC6813_manager_t;

void LTC6813_manager_init(LTC6813_manager_t *p_inst, LTC6813_manager_cfg_t *p_cfg);

void LTC6813_manager_task(LTC6813_manager_t *p_inst);

bool LTC6813_manager_cmd_request(LTC6813_manager_t *p_inst, LTC6813_cmd_t cmd);

void LTC6813_manager_poll_cells(LTC6813_manager_t *p_inst);

void LTC6813_manager_get_command_packet(LTC6813_manager_t *p_inst, uint16_t val1, uint16_t val2);


#endif