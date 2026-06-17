/**
 * @file demo_watcher.c
 * @author MITurley
 * @date 2026-06-03
 * @brief demo watcher <SF>
 */

#include <stdio.h>

 #include "demo_watcher.h"

uint32_t demo_watcher() {
//    demo_watch_adc_fault();
    demo_watch_can_fault();
//    demo_watch_i2c_fault();
//    demo_watch_pwm_fault();
//    demo_watch_spi_fault();
}

// Global variables used in demos
char watch[128];     //watch the 'watch' variable to debug
uint64_t get_val = UINT64_MAX;
uint64_t set_val = UINT64_MAX;
static periph_watcher_t *active_driver = NULL;

void init_driver(periph_watcher_t *driver) {
    active_driver = driver;
}
// Magic watch method
void watch_inst_conf_set(int p_func_id, int p_status_exp, int p_set_val, bool p_not_inst_null, bool p_not_conf_null) {
    if (active_driver == NULL) return;

    set_val = p_set_val;
    int64_t _set_val = set_val;
    
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
        "|Func:%s|Exp-Status:%s|Status:%s|",
        _func_str, _status_exp_str, _status_str);

    if (_state_str != NULL &&  sizeof(_state_str) > 0 && active_driver->get_state_func_id > 0 && len < sizeof(watch)) {
        len += snprintf(watch + len, sizeof(watch) - len, "State:%s|", _state_str);
    }
    
    if (!ISMAXINT(_set_val) && len < sizeof(watch)) {
        len += snprintf(watch + len, sizeof(watch) - len, "Set:%lld|", _set_val);
    }

    if (!ISMAXINT(get_val) && len < sizeof(watch)) {
        len += snprintf(watch + len, sizeof(watch) - len, "Get:%u|", get_val);
    }
    asm("NOP"); //!BREAK! Use this as breakpoint
}
// Watch method wrapper
void watch_inst_conf(int p_func_id, int p_status_exp, bool p_not_inst_null, bool p_not_conf_null) {
    watch_inst_conf_set(p_func_id, p_status_exp, UINT64_MAX, p_not_inst_null, p_not_conf_null);
}