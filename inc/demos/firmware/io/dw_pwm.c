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
// Get func output to compare non-null vs. null
static int get_func(int p_func_id, bool* p_not_inst_null) {
    setIntArrayDefault(get_para_num, 1, UINT8_MAX);
    setBoolArrayDefault(not_null_bools, 1, false);
    io_pwm_t *_inst             = p_not_inst_null[0] ? &inst : NULL;
    io_pwm_cfg_t *_config       = p_not_inst_null[1] ? &config : NULL;
    
    switch(p_func_id) {
        case 0: return (int64_t)IO_PWM_Init(_inst, _config);                           break;
        case 1: return (int64_t)IO_PWM_Set_Duty_Cycle(_inst, (uint16_t)set_para_num[0]);     break;
        case 2: return (int64_t)IO_PWM_Get_Duty_Cycle(_inst, (uint16_t*)&get_para_num[0]);         break;
        //!NOTE! Add new functions here
        //case #: return (int64_t)New_Func(&_inst, &get_para_num[0]);                              break;
    }
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
    demo_watch_pwm_funcs();
}
#define NULL_COUNT 2
// Tests the pwm fault system to ensure its accurate
void demo_watch_pwm_funcs() {
    init_driver_watcher("PWM", 1, &drive_watcher); //!BREAK! Use this as breakpoint
    
    // Action Methods
    // [0] PWM INIT
    watch_inst_conf(PWM_INIT, PWM_STATUS_OK,
        (bool[NULL_COUNT]) {true, true, true});
    watch_inst_conf(PWM_INIT, PWM_STATUS_ERROR_NULL_POINTER,
        (bool[NULL_COUNT]) {false, true, true});
    watch_inst_conf(PWM_INIT, PWM_STATUS_ERROR_NULL_POINTER,
        (bool[NULL_COUNT]) {true, false, true});
    watch_inst_conf(PWM_INIT, PWM_STATUS_ERROR_NULL_POINTER,
        (bool[NULL_COUNT]) {false, false, true});
    
    set_para_num[0] = 1000;
    // I/O Methods
    // [1] PWM SET DUTY CYCLE
    watch_inst_conf(PWM_SET_DUTY_CYCLE, PWM_STATUS_OK,
        (bool[NULL_COUNT]) {true, true, true});
    watch_inst_conf(PWM_SET_DUTY_CYCLE, PWM_STATUS_ERROR_NULL_POINTER,
        (bool[NULL_COUNT]) {false, false, true});

    // [2] PWM GET DUTY CYCLE
    watch_inst_conf(PWM_GET_DUTY_CYCLE, PWM_STATUS_OK,
        (bool[NULL_COUNT]) {true, true, true});
    watch_inst_conf(PWM_GET_DUTY_CYCLE, PWM_STATUS_ERROR_NULL_POINTER,
        (bool[NULL_COUNT]) {false, false, true});
    
    //!NOTE! Add new function to track here
    //// [#] NEW FUNC
    //track_inst_conf(FUNC_ID, STATUS_ID, NULL, NULL, NULL);
    
    asm("NOP"); //!BREAK! Use this as breakpoint
}
#endif