#ifndef LTC6813_H
#define LTC6813_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

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
    uint8_t WRCFGA;    // write config reg A
    uint8_t WRCFGB;    // write config reg B
    uint8_t RDCFGA;    // read config reg A
    uint8_t RDCFGB;    // read config reg B
    uint8_t RDCVA;     // read cell voltage reg A
    uint8_t RDCVB;     // read cell voltage reg B
    uint8_t RDCVC;     // read cell voltage reg C
    uint8_t RDCVD;     // read cell voltage reg D
    uint8_t RDCVE;     // read cell voltage reg E
    uint8_t RDCVF;     // read cell voltage reg F
    uint8_t RDAUXA;    // read aux reg A
    uint8_t RDAUXB;    // read aux reg B
    uint8_t RDAUXC;    // read aux reg C
    uint8_t RDAUXD;    // read aux reg D
    uint8_t RDSTATA;   // read status reg A
    uint8_t RDSTATB;   // read status reg B
    uint8_t WRSCTRL;   // write S control reg group
    uint8_t WRPWM;     // write PWM control reg group
    uint8_t WRPSB;     // write PWM/S control reg group
    uint8_t RDSCTRL;   // read S control reg group
    uint8_t RDPWM;     // read PWM control reg group
    uint8_t RDPSB;     // read PWM/S control reg group B
    uint8_t STSCTRL;   // start S control pulsing and poll status
    uint8_t CLRSCTRL;  // clear S control reg group
    uint16_t ADCV;     // start cell voltage ADC conversion and poll status
    uint16_t ADOW;     // start open wrie ADC conversion and poll status
    uint16_t CVST;     // start self test cell voltage conversion and poll status
    uint16_t ADOL;     // start overlap meas of cell 7 and cell 13 voltages
    uint16_t ADAX;     // start GPIOs adc conversion and poll status
    uint16_t ADAXD;    // start GPIOs adc conversion with digital redund and poll status
    uint16_t AXOW;     // start GPIOs open wrie ad conversion and poll status
    uint16_t AXST;     // start self test GPIOs converion and poll status
    uint16_t AXSTAT;   // start status group adc conversion and poll status
    uint16_t ADSTATD;  // start status group ADC converion and digital redundancy and poll status
    uint16_t STATST;   // start self test status group conversion and poll status
    uint16_t ADCVAX;   // start combined cell voltage and GPIO1,2 conversion and poll status
    uint16_t ADCVSC;   // start combined cell voltage and SC conversion and poll status
    uint16_t CLRCELL;  // clear cell voltage reg group
    uint16_t CLRAUX;   // clear auxillary reg group
    uint16_t CLRSTAT;  // clear status reg
    uint16_t PLADC;    // poll adc conversion status
    uint16_t DIAGN;    // diagnose MUX and poll status
    uint16_t WRCOMM;   // write comm reg group
    uint16_t RDCOMM;   // read comm reg group
    uint16_t STCOMM;   // start I2C/SPI comm
    uint8_t MUTE;      // mute discharge
    uint8_t UNMUTE;    // unmute discharge
} LTC6813_cfg_t;

typedef struct
{
    uint8_t MD;        // adc mode (0 - slow, 1 - fast, 2 - normal, 3 - filtered)
    bool DCP;          // discarge permitted
    uint8_t CH;        // cell selection for adc conversion
    bool PUP;          // pull-up (1)/pull-down(0) current for open wire conversion
    uint8_t ST;        // self test mode select
    uint8_t CHG;       // GPIO select for ADC conversion
    uint8_t CHS;       // status group select

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
    const LTC6813_cfg_t *cfg;

} LTC6813_t;

#endif