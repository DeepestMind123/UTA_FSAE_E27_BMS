/**
 * @file dw_manager.c
 * @author MITurley
 * @date 2026-06-03
 * @brief demo watch manager <SF>
 */

#include <stdio.h>
#include <string.h>

#include "dw_manager.h"

void demo_watcher() {
    demo_watch_adc_all();
    demo_watch_can_all();
    demo_watch_i2c_all();
    demo_watch_pwm_all();
    demo_watch_spi_all();
    
    demo_watch_current_sensor_all();
    // demo_watch_fan_control_all();
    // demo_watch_ltc6813_all();
}

// Global variables used in demos
char watch[100];     //watch the 'watch' variable to debug
uint32_t get_para_num_1     = UINT32_MAX;
uint32_t get_para_num_2     = UINT32_MAX;
uint32_t set_para_num_1     = UINT32_MAX;
uint32_t set_para_num_2     = UINT32_MAX;
bool get_para_bool_1        = false;
uint8_t test_id             = 0;
static driver_watcher_interface_t *active_driver = NULL;

void init_driver_watcher(char *p_driver_name, uint8_t p_test_id, driver_watcher_interface_t *p_driver) {
    snprintf(driver_name, sizeof(driver_name),
        "%s",
        p_driver_name);
    test_id = p_test_id;
    active_driver = p_driver;
}
// Watch method wrapper
void watch_inst_conf(int p_func_id, int p_status_exp, bool p_not_null_inst, bool p_not_null_conf, bool p_not_null_time_inst) {
    watch_inst_conf_set(p_func_id, p_status_exp, UINT64_MAX, p_not_null_inst, p_not_null_conf, p_not_null_time_inst);
}
void watch_inst_conf_set(int p_func_id, int p_status_exp, int p_set_val, bool p_not_null_inst, bool p_not_null_conf, bool p_not_null_time_inst) {
    watch_inst_conf_set_2(p_func_id, p_status_exp, p_set_val, UINT64_MAX, p_not_null_inst, p_not_null_conf, p_not_null_time_inst);
}

// Magic watch method
void watch_inst_conf_set_2(int p_func_id, int p_status_exp, int p_set_val, int p_set_val_2, bool p_not_null_inst, bool p_not_null_conf, bool p_not_null_time_inst) {
    if (active_driver == NULL) return;

    set_para_num_1 = p_set_val;
    uint32_t _set_val = set_para_num_1;
    set_para_num_2 = p_set_val_2;
    uint32_t _set_val_2 = set_para_num_2;
    
    // Run driver state function
    int state_idx = active_driver->execute_func(active_driver->get_state_func_id, true, true, true);
    
    // Run driver function
    int status_idx = active_driver->execute_func(p_func_id, p_not_null_inst, p_not_null_conf, p_not_null_time_inst);
    
    const char *_func_str       = active_driver->func_strings[p_func_id];
    const char *_status_exp_str = active_driver->status_strings[p_status_exp];
    const char *_status_str     = active_driver->status_strings[status_idx];
    const char *_state_str      = active_driver->state_strings[state_idx];
    
    memset(watch, 0, sizeof(watch));
    int len = snprintf(watch, sizeof(watch),
        "|%s:%d|Func:%s|Exp-Status:%s|Status:%s|",
        driver_name, test_id, _func_str, _status_exp_str, _status_str);

    if (_state_str != NULL &&  sizeof(_state_str) > 0 && active_driver->get_state_func_id > 0 && len < sizeof(watch)) {
        len += snprintf(watch + len, sizeof(watch) - len, "State:%s|", _state_str);
    }
    
    if (!ISMAXINT(_set_val) && len < sizeof(watch)) {
        len += snprintf(watch + len, sizeof(watch) - len, "Set:%u|", _set_val);
    }
    
    if (!ISMAXINT(_set_val_2) && len < sizeof(watch)) {
        len += snprintf(watch + len, sizeof(watch) - len, "Set(2):%u|", _set_val_2);
    }

    if (!ISMAXINT(get_para_num_1) && len < sizeof(watch)) {
        len += snprintf(watch + len, sizeof(watch) - len, "Get:%u|", get_para_num_1);
    }
    
    if (!ISMAXINT(get_para_num_2) && len < sizeof(watch)) {
        len += snprintf(watch + len, sizeof(watch) - len, "Get(2):%u|", get_para_num_2);
    }
    asm("NOP"); //!BREAK! Use this as breakpoint
}