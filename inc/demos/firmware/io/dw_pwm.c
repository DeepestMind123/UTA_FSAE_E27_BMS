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
    "INIT",
    "SET DUTY CYCLE",
    "GET DUTY CYCLE"
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
#define NULL_COUNT 2
// Get func output to compare non-null vs. null
static int get_func(int p_func_id) {
    setIntArrayDefault(get_para_num, 1, UINT8_MAX);
    io_pwm_t *_inst             = not_null_bools[0] ? &inst : NULL;
    io_pwm_cfg_t *_config       = not_null_bools[1] ? &config : NULL;
    
    switch(p_func_id) {
        case 0: return (int64_t)IO_PWM_Init(_inst, _config);
        case 1: return (int64_t)IO_PWM_Set_Duty_Cycle(_inst, (uint16_t)set_para_num[0]);
        case 2: return (int64_t)IO_PWM_Get_Duty_Cycle(_inst, (uint16_t*)&get_para_num[0]);
        //!NOTE! Add new functions here
        //case #: return (int64_t)New_Func(&_inst, &get_para_num[0]);
    }
    setBoolArrayDefault(not_null_bools, NULL_COUNT, false);
    setIntArrayDefault(set_para_num, 1, UINT8_MAX);
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
    not_null_bools = malloc(sizeof(bool) * NULL_COUNT);
    setBoolArrayDefault(not_null_bools, NULL_COUNT, false);
    demo_watch_pwm_funcs();
}
// Tests the pwm fault system to ensure its accurate
void demo_watch_pwm_funcs() {
    init_driver_watcher("PWM", 1, &drive_watcher); //!BREAK! Use this as breakpoint
    
    // Action Methods
    // [0] PWM INIT
    SET_NULL_FLAGS(true, true);
    watch_inst_conf(PWM_INIT, PWM_STATUS_OK);
    SET_NULL_FLAGS(false, true);
    watch_inst_conf(PWM_INIT, PWM_STATUS_ERROR_NULL_POINTER);
    SET_NULL_FLAGS(true, false);
    watch_inst_conf(PWM_INIT, PWM_STATUS_ERROR_NULL_POINTER);
    SET_NULL_FLAGS(false, false);
    watch_inst_conf(PWM_INIT, PWM_STATUS_ERROR_NULL_POINTER);
    
    set_para_num[0] = 1000;
    // I/O Methods
    // [1] PWM SET DUTY CYCLE
    SET_NULL_FLAGS(true, true);
    watch_inst_conf(PWM_SET_DUTY_CYCLE, PWM_STATUS_OK);
    SET_NULL_FLAGS(true, true);
    watch_inst_conf(PWM_SET_DUTY_CYCLE, PWM_STATUS_ERROR_NULL_POINTER);

    // [2] PWM GET DUTY CYCLE
    SET_NULL_FLAGS(true, true);
    watch_inst_conf(PWM_GET_DUTY_CYCLE, PWM_STATUS_OK);
    SET_NULL_FLAGS(true, true);
    watch_inst_conf(PWM_GET_DUTY_CYCLE, PWM_STATUS_ERROR_NULL_POINTER);
    
    //!NOTE! Add new function to track here
    //// [#] NEW FUNC
    //track_inst_conf(FUNC_ID, STATUS_ID, NULL, NULL, NULL);
    
    asm("NOP"); //!BREAK! Use this as breakpoint
}
#endif