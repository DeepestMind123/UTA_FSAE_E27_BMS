/**
 * @file dw_manager.h
 * @author MITurley
 * @date 2026-06-03
 * @brief demo watcher manager <HF>
 */

#ifndef DEMO_W_H
#define DEMO_W_H

#include <stdint.h>
#include <stdbool.h>

#include "demo_test.h"

#define IS_UPPER_DIRTY_16(x) (((uint64_t)(x) & 0xFFFFFFFFFFFF0000ULL) == 0xFFFFFFFFFFFF0000ULL)
#define IS_UPPER_DIRTY_32(x) (((uint64_t)(x) & 0xFFFFFFFF00000000ULL) == 0xFFFFFFFF00000000ULL)
#define ISMAXINT(x) ( \
    (x) == UINT64_MAX || \
    (x) == UINT32_MAX || \
    (x) == UINT16_MAX || \
    (x) == UINT8_MAX  || \
    IS_UPPER_DIRTY_16(x) || \
    IS_UPPER_DIRTY_32(x)   \
)

extern uint8_t test_id;
extern uint8_t param_size;
extern int64_t set_para_num[10];
extern int64_t get_para_num[10];
extern bool not_null_bools[10];
extern char driver_name[15];

void setBoolArrayDefault(bool *p_arr, int p_size, bool p_state);
void setIntArrayDefault(int64_t *p_arr, int p_size, int64_t p_val);
// This structure defines a driver demo
typedef struct {
    const char **func_strings;   // Array of function names
    const char **state_strings;  // Array of state names
    const char **status_strings; // Array of status names
    const int get_state_func_id;
    
    // Generic wrapper for printing demo information
    int (*execute_func)(int p_func_id, bool* p_non_null_inst);
} driver_watcher_interface_t;

// Purely generic testing engine functions
void init_driver_watcher(char *p_driver_name, uint8_t p_test_id, driver_watcher_interface_t *p_driver);
void watch_inst_conf(int p_func_id, int p_status_exp, bool *p_not_null_inst);

void demo_watcher();

// IO
#ifdef DEMO_ADC
void demo_watch_adc_all();
void demo_watch_adc_funcs();
void demo_watch_adc_null();
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
#endif

#endif