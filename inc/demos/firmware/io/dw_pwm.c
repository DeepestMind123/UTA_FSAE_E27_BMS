/**
 * @file demo_pwm.c
 * @author MITurley
 * @date 2026-06-03
 * @brief demo to test pwm <SF>
 */
 
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
 
 #include "io_pwm.h"
#include "dw_manager.h"

#ifdef DEMO_PWM
// Initialization
void Load_Duty_Cycle(uint16_t duty_value) {};
static io_pwm_cfg_t config = {
    .Load_Duty_Cycle       = Load_Duty_Cycle
};
static io_pwm_t inst;

// Watch strings
static const char *func_str[] = {
    "PWM INIT",
    "PWM SET DUTY CYCLE",
    "PWM GET DUTY CYCLE"
    //!NOTE! Add func name here
    //"NEW FUNC NAME"
};
static const char *status_str[] = {
    "OK",
    "NULL POINTER",
    "NOT INIT",
    //!NOTE! Add status here
    //"NEW STATUS"
};
// Helpful For ID Matching
typedef enum {
    PWM_INIT = 0,
    PWM_SET_DUTY_CYCLE,
    PWM_GET_DUTY_CYCLE,
    //!NOTE! Add func id here
    //"NEW FUNC ID"
} func_id;
// Get func output to compare non-null vs. null
static int get_func(int p_func_id, bool p_not_inst_null, bool p_not_conf_null, bool p_not_null_time_inst) {
    get_para_num_1              = UINT32_MAX;
    get_para_num_2              = UINT32_MAX;
    get_para_bool_1             = false;
    io_pwm_t *_inst             = p_not_inst_null ? &inst : NULL;
    io_pwm_cfg_t *_config       = p_not_conf_null ? &config : NULL;
    
    switch(p_func_id) {
        case 0: return (uint32_t)IO_PWM_Init(_inst, _config);                           break;
        case 1: return (uint32_t)IO_PWM_Set_Duty_Cycle(_inst, set_para_num_1);     break;
        case 2: return (uint32_t)IO_PWM_Get_Duty_Cycle(_inst, &get_para_num_1);         break;
        //!NOTE! Add new functions here
        //case #: return (uint32_t)IO_New_Func(&_inst, &get_para_num_1);                              break;
    }
    set_para_num_1 = UINT32_MAX;
    set_para_num_2 = UINT32_MAX;
}
static driver_watcher_interface_t drive_watcher = {
    .func_strings       = func_str,
    .state_strings      = NULL,
    .status_strings     = status_str,
    .execute_func       = get_func,
    .get_state_func_id  = -1
};

// Tests all pwm methods
void demo_watch_pwm_all() {
    demo_watch_pwm_funcs();
}
// Tests the pwm fault system to ensure its accurate
void demo_watch_pwm_funcs() {
    init_driver_watcher("PWM", 1, &drive_watcher); //!BREAK! Use this as breakpoint
    
    // Action Methods
    // [0] PWM INIT
    watch_inst_conf(PWM_INIT, PWM_STATUS_OK, true, true, true);
    watch_inst_conf(PWM_INIT, PWM_STATUS_ERROR_NULL_POINTER, true, false, true);
    watch_inst_conf(PWM_INIT, PWM_STATUS_ERROR_NULL_POINTER, false, true, true);
    watch_inst_conf(PWM_INIT, PWM_STATUS_ERROR_NULL_POINTER, false, false, true);
    
    // I/O Methods
    // [1] PWM SET DUTY CYCLE
    watch_inst_conf_set(PWM_SET_DUTY_CYCLE, PWM_STATUS_OK, 10, true, true, true);
    watch_inst_conf_set(PWM_SET_DUTY_CYCLE, PWM_STATUS_ERROR_NULL_POINTER, 20, false, false, true);

    // [2] PWM GET DUTY CYCLE
    watch_inst_conf(PWM_GET_DUTY_CYCLE, PWM_STATUS_OK, true, true, true);
    watch_inst_conf(PWM_GET_DUTY_CYCLE, PWM_STATUS_ERROR_NULL_POINTER, false, false, true);
    
    //!NOTE! Add new function to track here
    //// [#] ADC NEW FUNC
    //track_inst_conf(ADC_FUNC_ID, ADC_STATUS_ID, false, false);
    
    asm("NOP"); //!BREAK! Use this as breakpoint
}
#endif