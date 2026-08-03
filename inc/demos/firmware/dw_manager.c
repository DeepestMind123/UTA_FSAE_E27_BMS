/**
 * @file dw_manager.c
 * @author MITurley
 * @date 2026-06-03
 * @brief demo watch manager <SF>
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dw_manager.h"

void demo_watcher() {
    setIntArrayDefault(set_para_num, 1, UINT64_MAX);
    setIntArrayDefault(get_para_num, 1, UINT64_MAX);
    setBoolArrayDefault(not_null_bools, 1, false);

    // demo_watch_adc_all();
    // demo_watch_can_all();
    // demo_watch_i2c_all();
    // demo_watch_pwm_all();
    // demo_watch_spi_all();
    
    // demo_watch_isense_all();
    // demo_watch_fan_control_all();
    // demo_watch_ltc6813_all();

    demo_watch_utils_all();
}

void setBoolArrayDefault(bool *p_arr, int p_size, bool p_state) {
    if (p_arr == NULL) return;
    for (int i = 0; i < p_size; i++) {
        p_arr[i] = p_state;
    }
}

void setIntArrayDefault(int64_t *p_arr, int p_size, int64_t p_val) {
    if (p_arr == NULL) return;
    for (int i = 0; i < p_size; i++) {
        p_arr[i] = p_val;
    }
}
bool* createTrueBoolArrayCopy(size_t p_size) {
    if (p_size == 0) return NULL;

    // Allocate memory for the copy
    bool *new_arr = malloc(p_size * sizeof(bool));
    if (new_arr == NULL) {
        return NULL; // Memory allocation failed
    }

    // Set all boolean bytes to true (1)
    memset(new_arr, true, p_size * sizeof(bool));

    return new_arr;
}

// Global variables used in demos
char watch[200];     //watch the 'watch' variable to debug
char driver_name_watch[20];
char test_id_watch[20];
char func_str_watch[20];
char status_exp_watch[20];
char status_str_watch[20];
char state_str_watch[20];

char set_para_num_watch[50];
char get_para_num_watch[50];

uint8_t param_size = 10;
int64_t set_para_num[10];
int64_t get_para_num[10];
bool not_null_bools[10];

char driver_name[15];
uint8_t test_id = 0;
static driver_watcher_interface_t *active_driver = NULL;

void init_driver_watcher(char *p_driver_name, uint8_t p_test_id, driver_watcher_interface_t *p_driver) {
    snprintf(driver_name, sizeof(driver_name),
        "%s",
        p_driver_name);
    test_id = p_test_id;
    active_driver = p_driver;
}
// Magic watch method
void watch_inst_conf(int p_func_id, int p_status_exp, bool* p_not_null_inst) {
    if (active_driver == NULL || p_not_null_inst == NULL) return;

    int state_idx = -1;
    if (active_driver->get_state_func_id >= 0)
        state_idx = active_driver->execute_func(active_driver->get_state_func_id, p_not_null_inst);

    // Run driver function with temporary allocated array
    bool *bool_copy = createTrueBoolArrayCopy(param_size);
    int status_idx = active_driver->execute_func(p_func_id, bool_copy);
    free(bool_copy);

    const char *_func_str       = active_driver->func_strings[p_func_id];
    const char *_status_exp_str = active_driver->status_strings[p_status_exp];
    const char *_status_str     = active_driver->status_strings[status_idx];
    const char *_state_str      = NULL;
    if (state_idx >= 0) _state_str = active_driver->state_strings[state_idx];

    memset(watch, 0, sizeof(watch));
    snprintf(driver_name_watch, sizeof(driver_name_watch), "%s", driver_name);
    snprintf(test_id_watch, sizeof(test_id_watch), "%d", test_id);
    snprintf(func_str_watch, sizeof(func_str_watch), "%s", _func_str ? _func_str : "");
    snprintf(status_exp_watch, sizeof(status_exp_watch), "%s", _status_exp_str ? _status_exp_str : "");
    snprintf(status_str_watch, sizeof(status_str_watch), "%s", _status_str ? _status_str : "");

    if (active_driver->get_state_func_id > 0 && _state_str != NULL) {
        snprintf(state_str_watch, sizeof(state_str_watch), "%s", _state_str);
    } else {
        state_str_watch[0] = '\0';
    }

    size_t _set_para_size = sizeof(&set_para_num) / sizeof(&set_para_num[0]);
    size_t _get_para_size = sizeof(&get_para_num) / sizeof(&get_para_num[0]);
    memset(set_para_num_watch, 0, sizeof(set_para_num_watch));
    memset(get_para_num_watch, 0, sizeof(get_para_num_watch));

    int set_len = 0, get_len = 0;

    for (size_t i = 0; i < _set_para_size || i < _get_para_size; i++) {
        if (i < _set_para_size && !ISMAXINT(set_para_num[i])) {
            if (set_len < sizeof(set_para_num_watch)) {
                set_len += snprintf(set_para_num_watch + set_len, sizeof(set_para_num_watch) - set_len, "{Set(%zu):%lld}", i, set_para_num[i]);
            }
        }
        if (i < _get_para_size && !ISMAXINT(get_para_num[i])) {
            if (get_len < sizeof(get_para_num_watch)) {
                get_len += snprintf(get_para_num_watch + get_len, sizeof(get_para_num_watch) - get_len, "{Get(%zu):%lld}", i, get_para_num[i]);
            }
        }
    }

    snprintf(watch, sizeof(watch),
        "|%s:%s|Func:%s|Exp:%s|Stat:%s|State:%s|%s|%s|",
        driver_name_watch, test_id_watch, func_str_watch,
        status_exp_watch, status_str_watch, state_str_watch,
        set_para_num_watch, get_para_num_watch);

    asm("NOP"); //!BREAK! Use this as breakpoint
}