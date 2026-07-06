/**
 * @file dev_voltage_sensor.h
 * @author notwe
 * @date 2026-07-03
 * @brief generic voltage sensor device driver wrapper header
 * @todo 
*/

#ifndef DEV_VOLTAGE_SENSOR_H
#define DEV_VOLTAGE_SENSOR_H

#include <stdint.h>
#include <stdbool.h>

#include "util_time.h"
#include "io_adc.h"

#define CELL_NUM_MAX 255
#define IC_NUM_MAX 255

typedef enum
{
    VOLTAGE_SENSOR_OK = 0,
    VOLTAGE_SENSOR_NULL_POINTER,
    VOLTAGE_SENSOR_NOT_INIT,
    VOLTAGE_SENSOR_TIMEOUT,
    VOLTAGE_SENSOR_TIME_ERROR,
    VOLTAGE_SENSOR_ENDEF_STATE,
    VOLTAGE_SENSOR_STATE_MISMATCH,
    VOLTAGE_SENSOR_BUSY,
    VOLTAGE_SENSOR_STATUS_MAX
} voltage_sensor_status_t;

typedef enum
{
    VOLTAGE_SENSOR_STATE_UNDEF = 0,
    VOLTAGE_SENSOR_STATE_IDLE,
    VOLTAGE_SENSOR_STATE_START,
    VOLTAGE_SENSOR_STATE_WAIT,
    VOLTAGE_SENSOR_STATE_GET,
    VOLTAGE_SENSOR_STATE_READY,
    VOLTAGE_SENSOR_STATE_ERROR,
    VOLTAGE_SENSOR_STATE_MAX
} voltage_sensor_state_t;

typedef struct 
{
    int16_t cells_mV[CELL_NUM_MAX];
} voltage_sensor_val_t;

typedef struct
{
    bool gives_real_val;
    bool has_balance;
    uint8_t cell_num;
    uint8_t total_ic;
    uint32_t init_timeout;
} voltage_sensor_cfg_t;

typedef struct
{
    const voltage_sensor_cfg_t *cfg;
    voltage_sensor_val_t sensor_vals[IC_NUM_MAX];
} voltage_sensor_system_t;

typedef struct
{
    bool(*voltage_start_open_wire)(voltage_sensor_system_t *p_system);
    bool(*voltage_start_closed_wire)(voltage_sensor_system_t *p_system);
    bool(*voltage_sensor_get_result)(const voltage_sensor_system_t *p_system);
} voltage_sensor_func_t;


typedef struct
{
    uint32_t start_time;
    uint32_t current_timeout;

    bool is_init;
    bool is_ready;
    bool val_diff;

    voltage_sensor_system_t sensor;
    voltage_sensor_state_t state;
    const io_adc_t *adc_inst;
    const util_time_t *time_inst;
} voltage_sensor_t;

#endif