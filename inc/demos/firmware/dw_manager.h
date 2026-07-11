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

extern uint32_t get_para_num_1;
extern uint32_t get_para_num_2;
extern uint32_t set_para_num_1;
extern uint32_t set_para_num_2;
extern bool get_para_bool_1;
extern uint8_t test_id;
char driver_name[15];

// This structure defines a driver demo
typedef struct {
    const char **func_strings;   // Array of function names
    const char **state_strings;  // Array of state names
    const char **status_strings; // Array of status names
    const int get_state_func_id;
    
    // Generic wrapper for printing demo information
    int (*execute_func)(int p_func_id, bool p_non_null_inst, bool p_non_null_conf, bool p_non_null_time_inst);
} driver_watcher_interface_t;

// Purely generic testing engine functions
void init_driver_watcher(char *p_driver_name, uint8_t p_test_id, driver_watcher_interface_t *p_driver);
void watch_inst_conf(int p_func_id, int p_status_exp, bool p_not_null_inst, bool p_not_null_conf, bool p_not_null_time_inst);
void watch_inst_conf_set(int p_func_id, int p_status_exp, int p_set_val, bool p_not_null_inst, bool p_not_null_conf, bool p_not_null_time_inst);
void watch_inst_conf_set_2(int p_func_id, int p_status_exp, int p_set_val, int p_set_val_2, bool p_not_null_inst, bool p_not_null_conf, bool p_not_null_time_inst);

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
#ifdef DEMO_CURRENT_SENSOR
void demo_watch_current_sensor_all();
void demo_watch_current_sensor_funcs();
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