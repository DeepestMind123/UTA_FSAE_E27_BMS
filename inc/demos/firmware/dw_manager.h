/**
 * @file dw_manager.h
 * @author MITurley
 * @date 2026-06-03
 * @brief demo watcher manager <HF>
 */

#ifndef DEMO_W_H
#define DEMO_W_H

#include <stdint.h>
#include <float.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "demo_test.h"

#define IS_UPPER_DIRTY_16(x) (((uint64_t)(x) & 0xFFFFFFFFFFFF0000ULL) == 0xFFFFFFFFFFFF0000ULL)
#define IS_UPPER_DIRTY_32(x) (((uint64_t)(x) & 0xFFFFFFFF00000000ULL) == 0xFFFFFFFF00000000ULL)
#define ISMAXNUM(x) (   \
    ISMAXINT(x)     ||  \
    ISMAXFLOAT(x)       \
)
#define ISMAXINT(x) (           \
    (x) == UINT64_MAX       ||  \
    (x) == UINT32_MAX       ||  \
    (x) == UINT16_MAX       ||  \
    (x) == UINT8_MAX        ||  \
    IS_UPPER_DIRTY_16(x)    ||  \
    IS_UPPER_DIRTY_32(x)        \
)
#define ISMAXFLOAT(x) ( \
    (x) == FLT_MAX      \
)
#define PARAMSSIZE 10

extern uint8_t test_id;
extern int get_state;
extern int64_t set_para_int[PARAMSSIZE];
extern int64_t get_para_int[PARAMSSIZE];
extern float set_para_float[PARAMSSIZE];
extern float get_para_float[PARAMSSIZE];

#define SET_NULL_FLAGS(...) \
    memcpy(not_null_bools, (bool[]){ __VA_ARGS__ }, sizeof((bool[]){ __VA_ARGS__ }))
extern bool *not_null_bools;

void demo_watcher_init(size_t p_size);
void setBoolArrayDefault(bool *p_arr, int p_size, bool p_state);
void setIntArrayDefault(int64_t *p_arr, int p_size, int64_t p_val);
void setFloatArrayDefault(float *p_arr, int p_size, float p_val);
// This structure defines a driver demo
typedef struct {
    const char **func_strings;   // Array of function names
    const char **state_strings;  // Array of state names
    const char **status_strings; // Array of status names
    const int get_state_func_id;
    
    // Generic wrapper for printing demo information
    int (*execute_func)(int p_func_id);
} driver_watcher_interface_t;

// Purely generic testing engine functions
void init_driver_watcher(char *p_driver_name, uint8_t p_test_id, driver_watcher_interface_t *p_driver);
void watch_inst_conf(int p_func_id, int p_status_exp);

void demo_watcher();

// IO
#ifdef DEMO_ADC
void demo_watch_adc_all();
void demo_watch_adc_funcs();
void demo_watch_adc_init();
void demo_watch_adc_task();
#endif
#ifdef DEMO_CAN
void demo_watch_can_all();
void demo_watch_can_funcs();
#endif
#ifdef DEMO_I2C
void demo_watch_i2c_all();
void demo_watch_i2c_funcs();
#endif
#ifdef DEMO_PWM
void demo_watch_pwm_all();
void demo_watch_pwm_funcs();
#endif
#ifdef DEMO_SPI
void demo_watch_spi_all();
void demo_watch_spi_funcs();
#endif

// Devices
#ifdef DEMO_ISENSE
void demo_watch_isense_all();
void demo_watch_isense_fault();
#endif
#ifdef DEMO_FAN_CONTROL
void demo_watch_fan_control_all();
void demo_watch_fan_control_funcs();
#endif
#ifdef DEMO_LTC6813
void demo_watch_ltc6813_all();
void demo_watch_ltc6813_funcs();
#endif
#ifdef DEMO_UTILS
void demo_watch_utils_all();
void demo_watch_utils_funcs();
void demo_watch_utils_irq_funcs();
void demo_watch_utils_time_funcs();
void demo_watch_utils_pid_ctrl_funcs();

void demo_watch_utils_init();
#endif

#endif