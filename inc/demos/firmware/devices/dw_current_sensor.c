/**
 * @file demo_current_sensor.c
 * @author MITurley
 * @date 2026-06-03
 * @brief demo to test current_sensor <SF>
 */
 
#include <stddef.h>
#include <stdbool.h>
 
 #include "dev_current_sensor.h"
#include "dw_manager.h"

#ifdef DEMO_CURRENT_SENSOR
// Initialization
static dev_current_sensor_cfg_t config = {
    .current_gain_uV            = 3300000,
    .current_raw_cutoff         = 5000000,
    
};
static dev_current_sensor_t inst;
static util_time_t time_inst = {
    .is_init = false,
    .tick = 0U,
    .irq = NULL
};
static io_adc_t inst;
// Watch strings
const char *func_str[] = {
    "CURRENT_SENSOR INIT",
    "CURRENT_SENSOR TASK",
    "CURRENT_SENSOR START",
    "CURRENT_SENSOR PROCESS RAW",
    "CURRENT_SENSOR GET WAIT",
    "CURRENT_SENSOR GET GAIN",
    "CURRENT_SENSOR GET RAW",
    "CURRENT_SENSOR GET VAL",
    "CURRENT_SENSOR GET STATE",
    "CURRENT_SENSOR GET DATA DIFF",
    "CURRENT_SENSOR SET TIMEOUT",
    "CURRENT_SENSOR GET TIMEOUT"
    //!NOTE! Add func name here
    //"NEW FUNC NAME"
};
const char *status_str[] = {
    "OK",
    "NULL_POINTER",
    "NOT_INIT",
    "TIMEOUT",
    "ADC_ERROR",
    "TIME_ERROR",
    "UNDEFINED_STATE",
    "STATE_MISMATCH",
    "BUSY",
    "MAX"
//    "STATUS ALIGNMENT"
    //!NOTE! Add status here
    //"NEW STATUS"
};
const char *states_str[] = {
    "UNDEF",
    "IDLE",
    "START",
    "WAIT",
    "GET",
    "READY",
    "ERROR",
    "UNDEFINED"
    //!NOTE! Add state here
    //"NEW STATE"
};
// Get func output to compare non-null vs. null
int get_func(int p_func_id, bool p_not_inst_null, bool p_not_conf_null, bool p_not_null_time_inst) {
    get_para_num_1              = UINT32_MAX;
    get_para_num_2              = UINT32_MAX;
    get_para_bool_1             = false;
    dev_current_sensor_t *_inst    = p_not_inst_null ? &inst : NULL;
    dev_current_sensor_cfg_t *_config = p_not_conf_null ? &config : NULL;
    util_time_t *_time_inst    = p_not_null_time_inst ? &time_inst : NULL;
    
    switch(p_func_id) {
        case 0: return (uint64_t)DEV_Current_Sensor_Init(_inst, _config, _time_inst);                         break;
        case 1: return (uint64_t)DEV_Current_Sensor_Task(_inst);                                      break;
        case 2: return (uint64_t)DEV_Current_Sensor_Start(_inst);                                     break;
        case 3: return (uint64_t)DEV_Current_Sensor_Raw(_inst);                                               break;
        case 4: return (uint64_t)DEV_Current_Sensor_Get_Wait(_inst, &get_para_num_1);           break;
        case 5: return (uint64_t)DEV_Current_Sensor_Get_Gain(_inst, set_para_num_1);            break;
        case 6: return (uint64_t)DEV_Current_Sensor_Get_Raw(_inst, &get_para_num_1);                          break;
        case 7: return (uint64_t)DEV_Current_Sensor_Get_Val(_inst, &get_para_num_1);            break;
        case 8: return (uint64_t)DEV_Current_Sensor_Get_State(_inst, &get_para_num_1);          break;
        case 9: return (uint64_t)DEV_Current_Sensor_Get_Data_Diff(_inst, &get_para_num_1);      break;
        case 10: return (uint64_t)DEV_Current_Sensor_Set_Timeout(_inst, &get_para_num_1);     break;
        case 11: return (uint64_t)DEV_Current_Sensor_Get_Timeout(_inst, &get_para_num_1);       break;
        //!NOTE! Add new functions here
        //case #: return (uint64_t)IO_New_Func(&_inst, &get_para_num_1);      break;
    }
    set_para_num_1 = UINT32_MAX;
}
static peripheral_test_interface_t current_sensor_test_interface = {
    .func_strings   = func_str,
    .state_strings  = states_str,
    .status_strings = status_str,
    .execute_func   = get_func,
    .get_state_func_id = 8
};

// Tests all adc methods
void demo_watch_adc_all() {
    demo_current_sensor_fault();
}

// General CURRENT_SENSOR Test Method
uint32_t demo_current_sensor_fault() {
    start_peripheral_test(&current_sensor_test_interface);
    
    // Action Methods
    int _func_id = 0, _status_exp = 0; //!BREAK! Use this as breakpoint
    _func_id = 0;   // CURRENT_SENSOR INIT
    watch_inst_conf(_func_id, _status_exp, true, true, true);
    watch_inst_conf(_func_id, _status_exp, false, true, true);
    watch_inst_conf(_func_id, _status_exp, true, false, true);
    watch_inst_conf(_func_id, _status_exp, true, true, false);
    watch_inst_conf(_func_id, _status_exp, true, false, false);
    watch_inst_conf(_func_id, _status_exp, false, true, false);
    watch_inst_conf(_func_id, _status_exp, false, false, true);
    watch_inst_conf(_func_id, _status_exp, false, false, false);

    _func_id++;     // CURRENT_SENSOR TASK
    watch_inst_conf(_func_id, _status_exp, true, true, true);
    watch_inst_conf(_func_id, _status_exp, false, false, true);
    
    _func_id++;     // CURRENT_SENSOR START
    watch_inst_conf(_func_id, _status_exp, true, true, true);
    watch_inst_conf(_func_id, _status_exp, false, false, true);
    
    _func_id++;     // CURRENT_SENSOR GET VAL
    watch_inst_conf(_func_id, _status_exp, true, true, true);
    watch_inst_conf(_func_id, _status_exp, false, false, true);
    
    _func_id++;     // CURRENT_SENSOR GET RES
    watch_inst_conf(_func_id, _status_exp, true, true, true);
    watch_inst_conf(_func_id, _status_exp, false, false, true);
    
    _func_id++;     // CURRENT_SENSOR SET OFFSET
    watch_inst_conf(_func_id, _status_exp, true, true, true);
    watch_inst_conf(_func_id, _status_exp, false, false, true);
    
    _func_id++;     // CURRENT_SENSOR GET OFFSET
    watch_inst_conf_set(_func_id, _status_exp, 10, true, true, true);
    watch_inst_conf_set(_func_id, _status_exp, 20, false, false, true);
    
    _func_id++;     // CURRENT_SENSOR GET VREF
    watch_inst_conf(_func_id, _status_exp, true, true, true);
    watch_inst_conf(_func_id, _status_exp, false, false, true);
    
    _func_id++;     // CURRENT_SENSOR GET STATE
    watch_inst_conf(_func_id, _status_exp, true, true, true);
    watch_inst_conf(_func_id, _status_exp, false, false, true);
    
    _func_id++;     // CURRENT_SENSOR GET TIMEOUT
    watch_inst_conf(_func_id, _status_exp, true, true, true);
    watch_inst_conf(_func_id, _status_exp, false, false, true);
    
    //!NOTE! Add new function to track here
    //_test_id++;     // CURRENT_SENSOR NEW FUNC
    //track_inst_conf(_test_id, _status_exp, false, false);
    
    asm("NOP"); //!BREAK! Use this as breakpoint
    return 0;
}
#endif