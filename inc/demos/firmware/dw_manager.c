/**
 * @file dw_manager.c
 * @author MITurley
 * @date 2026-06-03
 * @brief demo watch manager <SF>
 */
#include <stdio.h>

#include "dw_manager.h"

void demo_watcher() {
    demo_watcher_init(0);

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

void demo_watcher_init(size_t p_size) {
    setIntArrayDefault(set_para_int, 1, UINT64_MAX);
    setIntArrayDefault(get_para_int, 1, UINT64_MAX);
    setFloatArrayDefault(set_para_float, 1, FLT_MAX);
    setFloatArrayDefault(get_para_float, 1, FLT_MAX);
    not_null_bools = malloc(sizeof(bool) * p_size);
    setBoolArrayDefault(not_null_bools, p_size, false);
}

void setBoolArrayDefault(bool *p_arr, int p_size, bool p_state) {
    if (p_arr == NULL) return;
    for (int i = 0; i < p_size; i++) p_arr[i] = p_state;
}
void setFloatArrayDefault(float *p_arr, int p_size, float p_val) {
    if (p_arr == NULL) return;
    for (int i = 0; i < p_size; i++) p_arr[i] = p_val;
}
void setIntArrayDefault(int64_t *p_arr, int p_size, int64_t p_val) {
    if (p_arr == NULL) return;
    for (int i = 0; i < p_size; i++) p_arr[i] = p_val;
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
char func_str_watch[30];
char status_exp_watch[30];
char status_str_watch[30];
char state_str_watch[20];

char set_para_int_watch[50];
char get_para_int_watch[50];
char set_para_float_watch[50];
char get_para_float_watch[50];

int get_state = INT_MAX;
int64_t set_para_int[PARAMSSIZE];
int64_t get_para_int[PARAMSSIZE];
float set_para_float[PARAMSSIZE];
float get_para_float[PARAMSSIZE];
bool *not_null_bools = NULL;

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
void watch_inst_conf(int p_func_id, int p_status_exp) {
    if (active_driver == NULL) return;

    // Get State
    int state_idx = -1;
    int status_idx = -1;

    // Run driver function with temporary allocated array
    bool *bool_copy = createTrueBoolArrayCopy(PARAMSSIZE);
    status_idx = active_driver->execute_func(p_func_id);
    free(bool_copy);
    if(p_func_id == active_driver->get_state_func_id)
        get_para_int[0] = get_state;
    if(active_driver->get_state_func_id >= 0) {
        if (p_func_id != active_driver->get_state_func_id)
            status_idx = active_driver->execute_func(active_driver->get_state_func_id);
        state_idx = get_state;
    }

    const char *_func_str       = active_driver->func_strings[p_func_id];
    const char *_status_exp_str = active_driver->status_strings[p_status_exp];
    const char *_status_str     = active_driver->status_strings[status_idx];

    memset(watch, 0, sizeof(watch));
    snprintf(driver_name_watch, sizeof(driver_name_watch), "%s", driver_name);
    snprintf(test_id_watch, sizeof(test_id_watch), "%d", test_id);
    snprintf(func_str_watch, sizeof(func_str_watch), "%s", _func_str ? _func_str : "");
    snprintf(status_exp_watch, sizeof(status_exp_watch), "%s", _status_exp_str ? _status_exp_str : "");
    snprintf(status_str_watch, sizeof(status_str_watch), "%s", _status_str ? _status_str : "");

    if (state_idx >= 0 && active_driver->get_state_func_id > 0)
        snprintf(state_str_watch, sizeof(state_str_watch), "%s", active_driver->state_strings[state_idx]);
    else memset(state_str_watch, 0, sizeof(state_str_watch));

    size_t _set_para_int_size   = sizeof(&set_para_int) / sizeof(&set_para_int[0]);
    size_t _get_para_int_size   = sizeof(&get_para_int) / sizeof(&get_para_int[0]);
    size_t _set_para_float_size = sizeof(&set_para_float) / sizeof(&set_para_float[0]);
    size_t _get_para_float_size = sizeof(&get_para_float) / sizeof(&get_para_float[0]);
    memset(set_para_int_watch, 0, sizeof(set_para_int_watch));
    memset(get_para_int_watch, 0, sizeof(get_para_int_watch));
    memset(set_para_float_watch, 0, sizeof(set_para_float_watch));
    memset(get_para_float_watch, 0, sizeof(get_para_float_watch));

    int set_int_len = 0, get_int_len = 0, set_float_len = 0, get_float_len = 0;

    for (size_t i = 0; i < _set_para_int_size || i < _get_para_int_size; i++) {
        if (i < _set_para_int_size && !ISMAXINT(set_para_int[i]) && set_int_len < sizeof(set_para_int_watch))
            set_int_len += snprintf(set_para_int_watch + set_int_len, sizeof(set_para_int_watch) - set_int_len, "{Set_I(%zu):%lld}", i, set_para_int[i]);
        if (i < _get_para_int_size && !ISMAXINT(get_para_int[i]) && get_int_len < sizeof(get_para_int_watch))
            get_int_len += snprintf(get_para_int_watch + get_int_len, sizeof(get_para_int_watch) - get_int_len, "{Get_I(%zu):%lld}", i, get_para_int[i]);
        
        if (i < _set_para_float_size && !ISMAXFLOAT(set_para_float[i]) && set_float_len < sizeof(set_para_float_watch))
            set_float_len += snprintf(set_para_float_watch + set_float_len, sizeof(set_para_float_watch) - set_float_len, "{Set_F(%zu):%f}", i, set_para_float[i]);
        if (i < _get_para_float_size && !ISMAXFLOAT(get_para_float[i]) && get_float_len < sizeof(get_para_float_watch))
            get_float_len += snprintf(get_para_float_watch + get_float_len, sizeof(get_para_float_watch) - get_float_len, "{Get_F(%zu):%f}", i, get_para_float[i]);
    }

    snprintf(watch, sizeof(watch),
        "|%s:%s|Func:%s|Exp:%s|Stat:%s|State:%s|%s|%s|%s|%s|",
        driver_name_watch, test_id_watch, func_str_watch,
        status_exp_watch, status_str_watch, state_str_watch,
        set_para_int_watch, set_para_float_watch,
        get_para_int_watch, get_para_float_watch);

    asm("NOP"); //!BREAK! Use this as breakpoint
}