/**
 * @file demo_watcher.h
 * @author MITurley
 * @date 2026-06-03
 * @brief demo watcher <HF>
 */

#ifndef DEMO_WATCHER_H
#define DEMO_WATCHER_H

#define DEMO_ADC
#define DEMO_CAN
//#define DEMO_I2C
//#define DEMO_PWM
//#define DEMO_SPI

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#define ISMAXINT(x) (x == UINT64_MAX || x == UINT32_MAX || x == UINT16_MAX || x == UINT8_MAX)

extern uint64_t get_val;
extern uint64_t set_val;

// This structure defines a demo peripheral
typedef struct {
    const char **func_strings;   // Array of function names
    const char **state_strings;  // Array of state names
    const char **status_strings; // Array of status names
    const int get_state_func_id;
    
    // Generic wrapper for printing demo information
    int (*execute_func)(int func_id, bool non_null_inst, bool non_null_conf);
} periph_watcher_t;

// Purely generic testing engine functions
void init_driver(periph_watcher_t *driver);
void watch_inst_conf_set(int p_func_id, int p_status_exp, int p_set_val, bool p_not_inst_null, bool p_not_conf_null);
void watch_inst_conf(int p_func_id, int p_status_exp, bool p_not_inst_null, bool p_not_conf_null);

uint32_t demo_watcher();

#ifdef DEMO_ADC
uint32_t demo_watch_adc_fault();
uint32_t demo_watch_adc_get(uint32_t p_cycles);
#endif

#ifdef DEMO_CAN
uint32_t demo_watch_can_fault();
#endif

#ifdef DEMO_I2C
uint32_t demo_watch_i2c_fault();
#endif

#ifdef DEMO_PWM
uint32_t demo_watch_pwm_fault();
#endif

#ifdef DEMO_SPI
uint32_t demo_watch_spi_fault();
#endif

#endif