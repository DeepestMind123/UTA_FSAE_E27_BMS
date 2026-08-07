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

typedef enum
{
    EVAL_ICON_OK = 0U,       // current discharge/charge ok
    EVAL_ICON_CHG_OC,            // over-current detected
    EVAL_ICON_DCHG_OC,
    EVAL_ICON_MAX
} eval_icon_t;

typedef enum
{
    EVAL_VCON_OK = 0U,       // cell voltage ok
    EVAL_VCON_OV,           // cell over-voltage
    EVAL_VCON_UV,           // cell under-voltage
    EVAL_VCON_MAX
} eval_vcon_t;

typedef enum
{
    EVAL_TCON_OK = 0U,       // temp within desired range (40 C during operation)
    EVAL_TCON_NOT_OPTIMAL,  // temp outside desired range but within safe limits
    EVAL_TCON_OT,            // temp outside safe limits (>60 C)
    EVAL_TCON_UT,
    EVAL_TCON_MAX
} eval_tcon_t;

typedef enum
{
    BATT_STATE_IDLE = 0,
    BATT_STATE_CHG,
    BATT_STATE_DCHG,
    BATT_STATE_MAX
} eval_batt_state_t;

typedef struct
{
    int32_t ival_mA;
    uint32_t i_timestamp;
    bool i_valid;
} isense_data_t;

typedef struct
{
    int16_t cell_val_mV[LARGE_ARR_32];
} vsense_mod_val_t;

typedef struct
{
    vsense_mod_val_t vmod[SMALL_ARR_16];
    uint32_t v_timestamp;
    bool v_valid;
} vsense_data_t;

typedef struct
{
    float tsense_val_C[SMALL_ARR_16];
} tsense_mod_val_t;

typedef struct
{
    tsense_mod_val_t tmod[SMALL_ARR_16];
    uint32_t t_timestamp;
    bool t_valid;
} tsense_data_t;

typedef struct 
{
    isense_data_t i_data;
    vsense_data_t v_data;
    tsense_data_t t_data;
} daq_data_t;

typedef struct
{
    bool ipack_fault;
    eval_icon_t icon;
} eval_ipack_t;

typedef struct
{
    eval_vcon_t cell_con[LARGE_ARR_32];
} eval_vmod_t;

typedef struct
{
    bool vpack_fault;
    eval_vmod_t vmod[SMALL_ARR_16];
} eval_vpack_t;

typedef struct
{
    eval_tcon_t temp_con[SMALL_ARR_16];
} eval_tmod_t;

typedef struct
{
    bool tpack_fault;
    eval_tmod_t tmod[SMALL_ARR_16];
} eval_tpack_t;

typedef struct
{
    eval_ipack_t icon;
    eval_vpack_t vcon;
    eval_tpack_t tcon;
    eval_batt_state_t batt_state;
} eval_con_t;

#endif