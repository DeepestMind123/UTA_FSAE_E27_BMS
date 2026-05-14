#ifndef LTC6813_H
#define LTC6813_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>

#include "ADI.h"

const enum uint16_t
{
    MD_SHIFT = 7,
    PUP_SHIFT = 6,
    ST_SHIFT = 5,
    DCP_SHIFT = 4,
    CH_SHIFT = 0,
    CHG_SHIFT = 0,
    CHST_SHIFT = 0,

    //variable commands to basic (0s where variables are)
    ADCV = 0x0260, // start cell voltage ADC conversion and poll status 
    ADOW = 0x0214, // start open wrie ADC conversion and poll status
    CVST = 0x0207, // start self test cell voltage conversion and poll status
    ADOL = 0x0201, // start overlap meas of cell 7 and cell 13 voltages
    ADAX = 0x0460, // start GPIOs adc conversion and poll status
    ADAXD = 0x0400, // start GPIOs adc conversion with digital redund and poll status
    AXOW = 0x0410, // start GPIOs open wrie ad conversion and poll status
    AXST = 0x0407, // start self test GPIOs converion and poll status
    ADSTAT = 0x0468, // start status group adc conversion and poll status
    ADSTATD = 0x0408, // start status group ADC converion and digital redundancy and poll status
    STATST = 0x040F, // start self test status group conversion and poll status
    ADCVAX = 0x046F, // start combined cell voltage and GPIO1,2 conversion and poll status
    ADCVSC = 0x0467, // "6 7" *hand gesture* start combined cell voltage and SC conversion and poll status

    //'s the rest of the 16 bit commands
    CLRCELL = 0x0711, // clear cell voltage reg group
    CLRAUX = 0x0712, // clear auxillary reg group
    CLRSTAT = 0x0713, // clear status reg
    PLADC = 0x0714, // poll adc conversion status
    DIAGN = 0x0715, // diagnose MUX and poll status
    WRCOMM = 0x0721, // write comm reg group
    RDCOMM = 0x0722, // read comm reg group
    STCOMM = 0x0723, // start I2C/SPI comm
};

const enum uint8_t
{
    WRCFGA = 0x01, // write config reg A
    WRCFGB = 0x24, // write config reg B
    RDCFGA = 0x02, // read config reg A
    RDCFGB = 0x26, // read config reg B
    RDCVA = 0x04, // read cell voltage reg A
    RDCVB = 0x06, // read cell voltage reg B
    RDCVC = 0x08, // read cell voltage reg C
    RDCVD = 0x0A, // read cell voltage reg D
    RDCVE = 0x09, // read cell voltage reg E
    RDCVF = 0x0B, // read cell voltage reg F
    RDAUXA = 0x0C, // read aux reg A
    RDAUXB = 0x0E, // read aux reg B
    RDAUXC = 0x0D, // read aux reg C
    RDAUXD = 0x0F, // read aux reg D
    RDSTATA = 0x10, // read status reg A
    RDSTATB = 0x12, // read status reg B
    WRSCTRL = 0x14, // write S control reg group
    WRPWM = 0x20, // write PWM control reg group
    WRPSB = 0x1C, // write PWM/S control reg group
    RDSCTRL = 0x16, // read S control reg group
    RDPWM = 0x22, // read PWM control reg group
    RDPSB = 0x1E, // read PWM/S control reg group B
    STSCTRL = 0x19, // start S control pulsing and poll status
    CLRSCTRL = 0x18, // clear S control reg group
    MUTE = 0x28, // mute discharge
    UNMUTE = 0x29, // unmute discharge
};

typedef enum
{
    READY = 0,
    START_VOLT,
    WAIT_VOLT,
    GET_VOLT,
    START_GPIO,
    WAIT_GPIO,
    GET_GPIO,
    START_TEST,
    WAIT_TEST,
    GET_TEST
} LTC6813_state_t;

typedef struct
{
    void(*LTC6813_cs_high)(void);

    void(*LTC6813_cs_low)(void);

    
} LTC6813_cfg_t;

typedef struct
{
    uint8_t MD;        // adc mode (0 - slow, 1 - fast, 2 - normal, 3 - filtered)
    bool DCP;          // discarge permitted
    uint8_t CH;        // cell selection for adc conversion
    bool PUP;          // pull-up(1)/pull-down(0) current for open wire conversion
    uint8_t ST;        // self test mode select
    uint8_t CHG;       // GPIO select for ADC conversion
    uint8_t CHST;       // status group select

    uint8_t CFGAR[6];  // cfg reg A
    uint8_t CFGBR[6];  // cfg reg B
    uint8_t CVAR[6];   // cell voltage reg A
    uint8_t CVBR[6];   // cell voltage reg B
    uint8_t CVCR[6];   // cell voltage reg C
    uint8_t CVDR[6];   // cell voltage reg D
    uint8_t CVER[6];   // cell voltage reg E
    uint8_t CVFR[6];   // cell voltage reg F
    uint8_t AVAR[6];   // aux reg A 
    uint8_t AVBR[6];   // aux reg B
    uint8_t AVCR[6];   // aux reg C
    uint8_t AVDR[6];   // aux reg D
    uint8_t STAR[6];   // status reg A
    uint8_t STBR[6];   // status reg B
    uint8_t COMM[6];   // comm reg
    uint8_t SCTRL[6];  // S control reg
    uint8_t PWMR[6];   // PWM control reg
    uint8_t PSR[6];    // PWM/S control reg (why does this exist?)

    LTC6813_state_t state;
    LTC6813_cfg_t *cfg;

} LTC6813_t;

void LTC6813_init(LTC6813_t *p_inst, LTC6813_cfg_t *p_cfg);

uint16_t LTC6813_task(LTC6813_t *p_inst);

uint16_t LTC6813_get_ADCV(LTC6813_t *p_inst);

uint16_t LTC6813_get_ADOW(LTC6813_t *p_inst);

uint16_t LTC6813_get_CVST(LTC6813_t *p_inst);

uint16_t LTC6813_get_ADOL(LTC6813_t *p_inst);

uint16_t LTC6813_get_ADAX(LTC6813_t *p_inst);

uint16_t LTC6813_get_ADAXD(LTC6813_t *p_inst);

uint16_t LTC6813_get_AXOW(LTC6813_t *p_inst);

uint16_t LTC6813_get_AXST(LTC6813_t *p_inst);

uint16_t LTC6813_get_ADSTAT(LTC6813_t *p_inst);

uint16_t LTC6813_get_ADSTATD(LTC6813_t *p_inst);

uint16_t LTC6813_get_STATST(LTC6813_t *p_inst);

uint16_t LTC6813_get_ADCVAX(LTC6813_t *p_inst);

uint16_t LTC6813_get_ADCVSC(LTC6813_t *p_inst);

#endif