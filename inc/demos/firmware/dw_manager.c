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
    demo_watch_adc_all();
    demo_watch_can_all();
    demo_watch_i2c_all();
    demo_watch_pwm_all();
    demo_watch_spi_all();
    
    demo_watch_isense_all();
    // demo_watch_fan_control_all();
    // demo_watch_ltc6813_all();
}

bool* getBoolArrayDefault(int p_size, bool p_state) {
    bool *all_true = malloc(p_size * sizeof(bool));
    for (int i = 0; i < p_size; i++) {
        all_true[i] = p_state;
    }
    return all_true;
}
int64_t* getIntArrayDefault(int p_size, int64_t p_val) {
    int64_t *all_true = malloc(p_size * sizeof(int64_t));
    for (int i = 0; i < p_size; i++) {
        all_true[i] = p_val;
    }
    return all_true;
}

// Global variables used in demos
char watch[100];     //watch the 'watch' variable to debug
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
    const size_t _not_null_size = sizeof(*p_not_null_inst) / sizeof(p_not_null_inst[0]);
    int state_idx = active_driver->execute_func(active_driver->get_state_func_id, getBoolArrayDefault(_not_null_size, true));
    
    // Run driver function
    int status_idx = active_driver->execute_func(p_func_id, getBoolArrayDefault(_not_null_size, true));
    
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
    
    size_t _set_para_size = 0;
    size_t _get_para_size = 0;
    if(set_para_num != NULL) _set_para_size = sizeof(set_para_num) / sizeof(set_para_num[0]);
    if(get_para_num != NULL) _get_para_size = sizeof(get_para_num) / sizeof(get_para_num[0]);
    for(size_t i = 0; i < _set_para_size || i < _get_para_size; i++) {
        if(set_para_num != NULL && i < _set_para_size && get_para_num != NULL && i < _get_para_size) {
            if (!ISMAXINT(set_para_num[i]) && !ISMAXINT(get_para_num[i]) && len < sizeof(watch)) {
                len += snprintf(watch + len, sizeof(watch) - len, "Set(%zu):%lld|Get(%zu):%lld|", i, set_para_num[i], i, get_para_num[i]);
            }
        }
        else if(set_para_num != NULL && i < _set_para_size) {
            if (!ISMAXINT(set_para_num[i]) && len < sizeof(watch)) {
                len += snprintf(watch + len, sizeof(watch) - len, "Set(%zu):%lld|", i, set_para_num[i]);
            }
        }
        else if(get_para_num != NULL && i < _get_para_size) {
            if (!ISMAXINT(get_para_num[i]) && len < sizeof(watch)) {
                len += snprintf(watch + len, sizeof(watch) - len, "Get(%zu):%lld|", i, get_para_num[i]);
            }
        }
    }
    asm("NOP"); //!BREAK! Use this as breakpoint
}