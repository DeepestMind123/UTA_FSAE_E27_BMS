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
#define ISMAXINT(x) (x == UINT64_MAX || x == UINT32_MAX || x == UINT16_MAX || x == UINT8_MAX)

extern int64_t *get_para_num;
extern int64_t *set_para_num;
extern bool *not_null_bools;
extern uint8_t test_id;
char driver_name[15];

bool* getBoolArrayDefault(int p_size, bool p_state);
int64_t* getIntArrayDefault(int p_size, int64_t p_val);
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
void watch_inst_conf(int p_func_id, int p_status_exp, int64_t *p_set_val, int64_t *p_get_val, bool *p_not_null_inst);

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

// Devies
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

#endif