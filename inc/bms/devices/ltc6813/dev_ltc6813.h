#ifndef DEV_LTC6813_H
#define DEV_LTC6813_H

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>

#include "LTC6813.h"
#include "ADI.h"
#include "util_time.h"
#include "io_spi.h"

typedef enum
{
    LTC6813_STATE_IDLE = 0,
    LTC6813_STATE_UNDEFINED,
    LTC6813_STATE_ERROR
} dev_ltc6813_state_t;

typedef enum
{
    LTC6813_CMD_NONE = 0,
    LTC6813_CMD_WRCFGA,
    LTC6813_CMD_WRCFGB,
    LTC6813_CMD_RDCFGA,
    LTC6813_CMD_RDCFGB,
    LTC6813_CMD_ADCV,
    LTC6813_CMD_ADAX,
    LTC6813_CMD_ADSTAT,
    LTC6813_CMD_ADCVAX,
    LTC6813_CMD_RDCV,
    LTC6813_CMD_RDAUX,
    LTC6813_CMD_RDSTAT,
    LTC6813_CMD_POLLADC,
    LTC6813_CMD_CLRCELL,
    LTC6813_CMD_CLRAUX,
    LTC6813_CMD_CLRSTAT,
    LTC6813_CMD_DIAGN,
    LTC6813_CMD_CVST,
    LTC6813_CMD_AXST,
    LTC6813_CMD_STATST,
    LTC6813_CMD_ADOL,
    LTC6813_CMD_ADAXD,
    LTC6813_CMD_ADSTATD,
    LTC6813_CMD_AXOW,
    LTC6813_CMD_WRPWM,
    LTC6813_CMD_RDPWM,
    LTC6813_CMD_WRSCTRL,
    LTC6813_CMD_RDSCTRL,
    LTC6813_CMD_STASCTRL,
    LTC6813_CMD_CLRSCTRL,
    LTC6813_CMD_WRCOMM,
    LTC6813_CMD_RDCOMM,
    LTC6813_CMD_STCOMM,
    LTC6813_CMD_MAX
} dev_ltc6813_cmd_t;

typedef enum
{
  LTC6813_STATUS_OK = 0,
  LTC6813_STATUS_ERROR_NOT_INIT,
  LTC6813_STATUS_ERROR_NULL_POINTER,
  LTC6813_STATUS_MAX
} dev_ltc6813_status_t;

typedef struct
{
  uint16_t c_codes[18]; // Cell Voltage Codes
  uint8_t pec_match[6]; // If a PEC error was detected during most recent read cmd    
} cv_t;

typedef struct
{
  uint16_t a_codes[9]; // Aux Voltage Codes
  uint8_t pec_match[4]; // If a PEC error was detected during most recent read cmd
} ax_t;

typedef struct
{
  uint16_t stat_codes[4]; // Status codes.
  uint8_t flags[3]; // Byte array that contains the uv/ov flag data
  uint8_t mux_fail[1]; // Mux self test status flag
  uint8_t thsd[1]; // Thermal shutdown status
  uint8_t pec_match[2]; //!<  a PEC error was detected during most recent read cmd
} st_t;

typedef struct
{
  uint8_t tx_data[6];  // Stores data to be transmitted 
  uint8_t rx_data[8];  // Stores received data 
  uint8_t rx_pec_match; // If a PEC error was detected during most recent read cmd
} ic_register_t;

typedef struct
{
  uint16_t pec_count; // Overall PEC error count
  uint16_t cfgr_pec;  // Configuration register data PEC error count
  uint16_t cell_pec[6]; // Cell voltage register data PEC error count
  uint16_t aux_pec[4];  // Aux register data PEC error count
  uint16_t stat_pec[2]; // Status register data PEC error count
} pec_counter_t;

typedef struct
{
  uint8_t cell_channels; // Number of Cell channels
  uint8_t stat_channels; // Number of Stat channels
  uint8_t aux_channels;  // Number of Aux channels
  uint8_t num_cv_reg;    // Number of Cell voltage register
  uint8_t num_gpio_reg;  // Number of Aux register
  uint8_t num_stat_reg;  // Number of  Status register
} register_cfg_t;

typedef struct
{
    ic_register_t configa;
    ic_register_t configb;
    cv_t cells;
    ax_t stat;
    ic_register_t com;
    ic_register_t pwma;
    ic_register_t pwmb;
    ic_register_t sctrla;
    ic_register_t sctrlb;
    uint8_t sid[6];
    bool isospi_reverse;
    pec_counter_t crc_count;
    uint16_t system_open_wire;
} cell_asic_t;

typedef struct
{
    bool is_init;

    uint8_t md;        // adc mode (0 - slow, 1 - fast, 2 - normal, 3 - filtered)
    bool dcp;          // discarge permitted
    uint8_t ch;        // cell selection for adc conversion
    bool pup;          // pull-up(1)/pull-down(0) current for open wire conversion
    uint8_t st;        // self test mode select
    uint8_t chg;       // GPIO select for ADC conversion
    uint8_t chst;       // status group select (idek what these are atm)

    uint8_t cmd_packet[4];
    uint8_t len;

    uint8_t cmd_buffer; // stores next command
    uint16_t cv_buffer[18]; // stores the last polled cell voltages
    uint16_t aux_buffer[12]; // stores the last polled gpio values

    bool busy_flag; // true if command is currently being excecuted
    bool ready_flag; // true if buffer is full

    uint32_t delay_ms;
    uint32_t elapsed_ms;

    dev_ltc6813_state_t state;
    dev_ltc6813_cmd_t cmd;
    
    cell_asic_t *p_cell_asic;

} dev_ltc6813_t;

dev_ltc6813_status_t DEV_LTC6813_Init(dev_ltc6813_t *p_inst, cell_asic_t *p_cell_asic);

dev_ltc6813_status_t DEV_LTC6813_Task(dev_ltc6813_t *p_inst);

dev_ltc6813_status_t DEV_LTC6813_CMD_Request(dev_ltc6813_t *p_inst, dev_ltc6813_cmd_t cmd, bool *p_out);

dev_ltc6813_status_t DEV_LTC6813_Get_CMD_Packet(dev_ltc6813_t *p_inst, uint16_t val1, uint16_t val2);


#endif