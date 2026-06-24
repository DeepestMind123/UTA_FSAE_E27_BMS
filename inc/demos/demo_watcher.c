/**
 * @file demo_watcher.c
 * @author MITurley
 * @date 2026-06-03
 * @brief demo watcher <SF>
 */

#include <stdio.h>
#include <string.h>

 #include "demo_watcher.h"

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
uint64_t get_val        = UINT64_MAX;
uint64_t get_val_2      = UINT64_MAX;
uint64_t set_val        = UINT64_MAX;
uint64_t set_val_2      = UINT64_MAX;
uint8_t test_id         = 0;
static driver_watcher_interface_t *active_driver = NULL;

void init_driver_watcher(char *p_driver_name, uint8_t p_test_id, driver_watcher_interface_t *p_driver) {
    snprintf(driver_name, sizeof(driver_name),
        "%s",
        p_driver_name);
    test_id = p_test_id;
    active_driver = p_driver;
}
void watch_inst_conf_set(int p_func_id, int p_status_exp, int p_set_val, bool p_not_inst_null, bool p_not_conf_null) {
    watch_inst_conf_set_2(p_func_id, p_status_exp, p_set_val, UINT64_MAX, p_not_inst_null, p_not_conf_null);
}

// Magic watch method
void watch_inst_conf_set_2(int p_func_id, int p_status_exp, int p_set_val, int p_set_val_2, bool p_not_inst_null, bool p_not_conf_null) {
    if (active_driver == NULL) return;

    set_val = p_set_val;
    int64_t _set_val = set_val;
    set_val_2 = p_set_val_2;
    int64_t _set_val_2 = set_val_2;
    
    // Run driver state function
    int state_idx = active_driver->execute_func(active_driver->get_state_func_id, true, true);
    
    // Run driver function
    int status_idx = active_driver->execute_func(p_func_id, p_not_inst_null, p_not_conf_null);
    
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
        len += snprintf(watch + len, sizeof(watch) - len, "Set:%lld|", _set_val);
    }
    
    if (!ISMAXINT(_set_val_2) && len < sizeof(watch)) {
        len += snprintf(watch + len, sizeof(watch) - len, "Set(2):%lld|", _set_val_2);
    }

    if (!ISMAXINT(get_val) && len < sizeof(watch)) {
        len += snprintf(watch + len, sizeof(watch) - len, "Get:%u|", get_val);
    }
    
    if (!ISMAXINT(get_val_2) && len < sizeof(watch)) {
        len += snprintf(watch + len, sizeof(watch) - len, "Get(2):%u|", get_val_2);
    }
    asm("NOP"); //!BREAK! Use this as breakpoint
}
// Watch method wrapper
void watch_inst_conf(int p_func_id, int p_status_exp, bool p_not_inst_null, bool p_not_conf_null) {
    watch_inst_conf_set(p_func_id, p_status_exp, UINT64_MAX, p_not_inst_null, p_not_conf_null);
}