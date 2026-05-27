#ifndef LTC6813_H
#define LTC6813_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>

#define MD_MAX 4 // it makes me so mad i have to do this. compile time arg vs run-time and all that

const uint16_t US_TO_MS = 1000;

const uint16_t MD_SHIFT = 7;
const uint16_t PUP_SHIFT = 6;
const uint16_t ST_SHIFT = 5;
const uint16_t DCP_SHIFT = 4;
const uint16_t CH_SHIFT = 0;
const uint16_t CHG_SHIFT = 0;
const uint16_t CHST_SHIFT = 0;

// variable commands to basic (0s where variables are)
const uint16_t ADCV = 0x0260; // start cell voltage ADC conversion and poll status 
const uint16_t ADOW = 0x0214; // start open wrie ADC conversion and poll status
const uint16_t CVST = 0x0207; // start self test cell voltage conversion and poll status
const uint16_t ADOL = 0x0201; // start overlap meas of cell 7 and cell 13 voltages
const uint16_t ADAX = 0x0460; // start GPIOs adc conversion and poll status
const uint16_t ADAXD = 0x0400; // start GPIOs adc conversion with digital redund and poll status
const uint16_t AXOW = 0x0410; // start GPIOs open wrie ad conversion and poll status
const uint16_t AXST = 0x0407; // start self test GPIOs converion and poll status
const uint16_t ADSTAT = 0x0468;  // start status group adc conversion and poll status
const uint16_t ADSTATD = 0x0408; // start status group ADC converion and digital redundancy and poll status
const uint16_t STATST = 0x040F; // start self test status group conversion and poll status
const uint16_t ADCVAX = 0x046F; // start combined cell voltage and GPIO1,2 conversion and poll status
const uint16_t ADCVSC = 0x0467; // "6 7" *hand gesture* start combined cell voltage and SC conversion and poll status

// here's the rest of the 16 bit commands
const uint16_t CLRCELL = 0x0711; // clear cell voltage reg group
const uint16_t CLRAUX = 0x0712; // clear auxillary reg group
const uint16_t CLRSTAT = 0x0713; // clear status reg
const uint16_t PLADC = 0x0714; // poll adc conversion status
const uint16_t DIAGN = 0x0715; // diagnose MUX and poll status
const uint16_t WRCOMM = 0x0721; // write comm reg group
const uint16_t RDCOMM = 0x0722; // read comm reg group
const uint16_t STCOMM = 0x0723; // start I2C/SPI comm

// 8 bit commands (yes i know that *technically* all of them end up being 16 bits but i only have so much assumed FLASH memory)
// (so there)
const uint8_t WRCFGA = 0x01; // write config reg A
const uint8_t WRCFGB = 0x24; // write config reg B
const uint8_t RDCFGA = 0x02; // read config reg A
const uint8_t RDCFGB = 0x26; // read config reg B
const uint8_t RDCVA = 0x04; // read cell voltage reg A
const uint8_t RDCVB = 0x06; // read cell voltage reg B
const uint8_t RDCVC = 0x08; // read cell voltage reg C
const uint8_t RDCVD = 0x0A; // read cell voltage reg D
const uint8_t RDCVE = 0x09; // read cell voltage reg E
const uint8_t RDCVF = 0x0B; // read cell voltage reg F
const uint8_t RDAUXA = 0x0C; // read aux reg A
const uint8_t RDAUXB = 0x0E; // read aux reg B
const uint8_t RDAUXC = 0x0D; // read aux reg C
const uint8_t RDAUXD = 0x0F; // read aux reg D
const uint8_t RDSTATA = 0x10; // read status reg A
const uint8_t RDSTATB = 0x12; // read status reg B
const uint8_t WRSCTRL = 0x14; // write S control reg group
const uint8_t WRPWM = 0x20; // write PWM control reg group
const uint8_t WRPSB = 0x1C; // write PWM/S control reg group
const uint8_t RDSCTRL = 0x16; // read S control reg group
const uint8_t RDPWM = 0x22; // read PWM control reg group
const uint8_t RDPSB = 0x1E; // read PWM/S control reg group B
const uint8_t STSCTRL = 0x19; // start S control pulsing and poll status
const uint8_t CLRSCTRL = 0x18; // clear S control reg group
const uint8_t MUTE = 0x28; // mute discharge
const uint8_t UNMUTE = 0x29; // unmute discharge

static const uint16_t cell_meas_time_us[MD_MAX] =
{
    2152U, // 422Hz (md = 0)
    203U, // 27kHz (md = 1)
    407U, // 7kHz  (md = 2)
    33570U // 26Hz  (md = 3)
};

uint32_t LTC6813_get_delay_ms(uint8_t md, uint8_t cell_num);

uint16_t LTC6813_get_ADCV(uint8_t MD, bool DCP, uint8_t CH);

uint16_t LTC6813_get_ADOW(uint8_t MD, bool PUP, bool DCP, uint8_t CH);

uint16_t LTC6813_get_CVST(uint8_t MD, uint8_t ST);

uint16_t LTC6813_get_ADOL(uint8_t MD, bool DCP);

uint16_t LTC6813_get_ADAX(uint8_t MD, uint8_t CHG);

uint16_t LTC6813_get_ADAXD(uint8_t MD, uint8_t CHG);

uint16_t LTC6813_get_AXOW(uint8_t MD, bool PUP, uint8_t CHG);

uint16_t LTC6813_get_AXST(uint8_t MD, uint8_t ST);

uint16_t LTC6813_get_ADSTAT(uint8_t MD, uint8_t CHST);

uint16_t LTC6813_get_ADSTATD(uint8_t MD, uint8_t CHST);

uint16_t LTC6813_get_STATST(uint8_t MD, uint8_t ST);

uint16_t LTC6813_get_ADCVAX(uint8_t MD, bool DCP);

uint16_t LTC6813_get_ADCVSC(uint8_t MD, bool DCP);



#endif