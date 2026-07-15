/**
 * @file bms_data.h
 * @author notwe
 * @date 2026-07-08
 * @brief bms data storage definitions
*/

#ifndef BMS_DATA_H
#define BMS_DATA_H

#include <stdint.h>
#include <stdbool.h>

#include "util_const.h"

typedef struct
{
    int32_t ival_mA;
    uint32_t i_timestamp;
    bool i_valid;
} isense_data_t;

typedef struct
{
    int16_t cell_val_mV[LARGE_ARR_64];
} vsense_mod_val_t;

typedef struct
{
    vsense_mod_val_t modv[SMALL_ARR_32];
    uint32_t v_timestamp;
    bool v_valid;
} vsense_data_t;

typedef struct
{
    float tsense_val_C[SMALL_ARR_32];
} tsense_mod_val_t;

typedef struct
{
    tsense_mod_val_t modt[SMALL_ARR_32];
    uint32_t t_timestamp;
    bool t_valid;
} tsense_data_t;

typedef struct 
{
    isense_data_t i_data;
    vsense_data_t v_data;
    tsense_data_t t_data;
} daq_data_t;

#endif