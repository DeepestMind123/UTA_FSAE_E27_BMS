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
#include <stddef.h>

#include "util_time.h"
#include "io_adc.h"

#define CELL_NUM_MAX 255
#define IC_NUM_MAX 255

typedef enum
{
    VOLTAGE_SENSOR_OK = 0,
    VOLTAGE_SENSOR_NULL_POINTER,
    VOLTAGE_SENSOR_NULL_FUNC,
    VOLTAGE_SENSOR_NOT_INIT,
    VOLTAGE_SENSOR_TIMEOUT,
    VOLTAGE_SENSOR_TIME_ERROR,
    VOLTAGE_SENSOR_ADC_ERROR,
    VOLTAGE_SENSOR_UNDEF_STATE,
    VOLTAGE_SENSOR_MISMATCH_STATE,
    VOLTAGE_SENSOR_BUSY,
    VOLTAGE_SENSOR_CONVERSION_FAIL,
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
    uint32_t init_timeout;
    uint8_t cell_num;
    uint8_t ic_num;
    bool gives_real_val;
} voltage_sensor_cfg_t;

typedef struct
{
    const voltage_sensor_cfg_t *cfg;
    voltage_sensor_val_t sensor_raw_vals[IC_NUM_MAX];
    voltage_sensor_val_t sensor_process_vals[IC_NUM_MAX];
} voltage_sensor_system_t;

typedef struct
{
    bool(*voltage_start_open_wire)(voltage_sensor_system_t *p_system);
    bool(*voltage_start_closed_wire)(voltage_sensor_system_t *p_system);
    bool(*voltage_state)(voltage_sensor_system_t *p_system);
    bool(*voltage_sensor_get_result)(const voltage_sensor_system_t *p_system);
} voltage_sensor_func_t;


typedef struct
{
    uint32_t start_time;
    uint32_t current_timeout;

    bool is_init;
    bool is_ready;
    bool val_diff;
    bool is_balance;

    voltage_sensor_system_t sensor;
    const voltage_sensor_func_t *func;
    voltage_sensor_state_t state;
    const io_adc_t *adc;
    const util_time_t *time;
} voltage_sensor_t;

voltage_sensor_status_t DEV_Voltage_Sensor_Init(voltage_sensor_t *p_inst,
                                                const voltage_sensor_cfg_t *p_cfg,
                                                const voltage_sensor_func_t *p_func,
                                                const io_adc_t *p_adc,
                                                const util_time_t *p_time);

voltage_sensor_status_t DEV_Voltage_Sensor_Task(voltage_sensor_t *p_inst);

voltage_sensor_status_t DEV_Voltage_Sensor_Start(voltage_sensor_t *p_inst);

voltage_sensor_status_t DEV_Voltage_Sensor_Process_Raw(voltage_sensor_t *p_inst, int32_t val, uint16_t *p_out);

voltage_sensor_status_t DEV_Voltage_Sensor_Set_Balance(voltage_sensor_t *p_inst, bool val);

voltage_sensor_status_t DEV_Voltage_Sensor_Get_State(voltage_sensor_t *p_inst, voltage_sensor_state_t *p_out);

#endif