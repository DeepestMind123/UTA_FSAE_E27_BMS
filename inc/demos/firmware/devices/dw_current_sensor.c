/**
 * @file demo_current_sensor.c
 * @author MITurley
 * @date 2026-06-03
 * @brief demo to test current_sensor <SF>
 */
 
#include <stddef.h>
#include <stdbool.h>
 
#include "dev_current_sensor.h"
#include "io_adc.h"
#include "dw_manager.h"

#ifdef DEMO_ISENSE
// Initialization
static isense_cfg_t config = {
    .isense_gain_uV            = 3300000,
    .isense_raw_cutoff         = 5000000,
    
};
static isense_t inst;
static util_time_t time_inst = {
    .is_init = false,
    .tick = 0U,
    .irq = NULL
};
static io_adc_t adc_inst;

// Watch strings
const char *func_str[] = {
    "INIT",
    "TASK",
    "START",
    "PROCESS RAW",
    "GET WAIT",
    "GET GAIN",
    "GET VAL",
    "GET STATE",
    "GET DATA DIFF",
    "GET READY FLAG",
    "SET TIMEOUT",
    "GET TIMEOUT"
    //!NOTE! Add func name here
    //"NEW FUNC NAME"
};
const char *status_str[] = {
    "OK",
    "NULL_PTR",
    "NOT_INIT",
    "TIMEOUT",
    "ADC_FAULT",
    "TIME_FAULT",
    "UNDEF_STATE",
    "MISMATCH_STATE",
    "BUSY",
    "STATUS_MAX"
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
    "MAX"
    //!NOTE! Add state here
    //"NEW STATE"
};
// Helpful For ID Matching
typedef enum {
    ISENSE_INIT = 0,
    ISENSE_TASK,
    ISENSE_START,
    ISENSE_PROCESS_RAW,
    ISENSE_GET_WAIT,
    ISENSE_GET_GAIN,
    ISENSE_GET_VAL,
    ISENSE_GET_STATE,
    ISENSE_GET_DATA_DIFF,
    ISENSE_GET_READY_FLAG,
    ISENSE_SET_TIMEOUT,
    ISENSE_GET_TIMEOUT,
    //!NOTE! Add func id here
    //"NEW FUNC ID"
} func_id;
// Get func output to compare non-null vs. null
int get_func(int p_func_id, bool* p_not_inst_null) {
    get_para_num = getIntArrayDefault(1, UINT64_MAX);
    get_para_bool = getBoolArrayDefault(1, false);
    isense_t *_inst             = p_not_inst_null[0] ? &inst : NULL;
    isense_cfg_t *_config       = p_not_inst_null[1] ? &config : NULL;
    io_adc_t *_adc_inst         = p_not_inst_null[2] ? &adc_inst : NULL;
    util_time_t *_time_inst     = p_not_inst_null[3] ? &time_inst : NULL;
    
    switch(p_func_id) {
        case 0: return (int64_t)DEV_Isense_Init(_inst, _config,
             _adc_inst, _time_inst);                                       break;
        case 1: return (int64_t)DEV_Isense_Task(_inst);                                    break;
        case 2: return (int64_t)DEV_Isense_Start(_inst);                                   break;
        case 3: return (int64_t)DEV_Isense_Process_Raw(_inst);                              break;
        case 4: return (int64_t)DEV_Isense_Get_Wait(_inst, &get_para_num[0]);           break;
        case 5: return (int64_t)DEV_Isense_Get_Gain(_inst, &get_para_num[0]);           break;
        case 6: return (int64_t)DEV_Isense_Get_Val(_inst, &get_para_num[0]);            break;
        case 7: return (int64_t)DEV_Isense_Get_State(_inst, &get_para_num[0]);          break;
        case 8: return (int64_t)DEV_Isense_Get_Data_Diff(_inst, &get_para_num[0]);      break;
        case 9: return (int64_t)DEV_Isense_Get_Ready_Flag(_inst, &get_para_num[0]);    break;
        case 10: return (int64_t)DEV_Isense_Set_Timeout(_inst, set_para_num[0]);        break;
        case 11: return (int64_t)DEV_Isense_Get_Timeout(_inst, &get_para_num[0]);       break;
        //!NOTE! Add new functions here
        //case #: return (int64_t)New_Func(&_inst, &get_para_num[0]);      break;
    }
    set_para_num = getIntArrayDefault(1, UINT64_MAX);
}
static driver_watcher_interface_t isense_test_interface = {
    .func_strings   = func_str,
    .state_strings  = states_str,
    .status_strings = status_str,
    .execute_func   = get_func,
    .get_state_func_id = 8
};

// Tests all adc methods
void demo_watch_isense_all() {
    demo_watch_isense_fault();
}

#define NULL_COUNT 4
// General ISense test method that goes through every function
void demo_watch_isense_fault() {
    init_driver_watcher("ISense", 1, &isense_test_interface);
    int64_t *_set_para_num = getIntArrayDefault(1, UINT64_MAX);
    int64_t *_get_para_num = getIntArrayDefault(1, UINT64_MAX);
    bool *_get_para_bool = getBoolArrayDefault(1, false);

    // Action Methods
    // [0] ISense INIT
    watch_inst_conf(ISENSE_INIT, ISENSE_OK, 
        _set_para_num, _get_para_num, 
        (bool[NULL_COUNT]) {true, true, true, true});
    watch_inst_conf(ISENSE_INIT, ISENSE_NULL_PTR, 
        _set_para_num, _get_para_num, 
        (bool[NULL_COUNT]) {true, true, true, true});

    // [1] ISense TASK
    watch_inst_conf(ISENSE_TASK, ISENSE_OK, 
        _set_para_num, _get_para_num, 
        (bool[NULL_COUNT]) {true, true, true, true});
    watch_inst_conf(ISENSE_TASK, ISENSE_NULL_PTR, 
        _set_para_num, _get_para_num, 
        (bool[NULL_COUNT]) {true, true, true, true});
    
    // [2] ISense START
    watch_inst_conf(ISENSE_START, ISENSE_OK, 
        _set_para_num, _get_para_num, 
        (bool[NULL_COUNT]) {true, true, true, true});
    watch_inst_conf(ISENSE_START, ISENSE_NULL_PTR, 
        _set_para_num, _get_para_num, 
        (bool[NULL_COUNT]) {true, true, true, true});
    
    // [3] ISense PROCESS RAW
    watch_inst_conf(ISENSE_PROCESS_RAW, ISENSE_OK, 
        _set_para_num, _get_para_num, 
        (bool[NULL_COUNT]) {true, true, true, true});
    watch_inst_conf(ISENSE_PROCESS_RAW, ISENSE_NULL_PTR, 
        _set_para_num, _get_para_num, 
        (bool[NULL_COUNT]) {true, true, true, true});

    // [4] ISense GET WAIT
    watch_inst_conf(ISENSE_GET_WAIT, ISENSE_OK, 
        _set_para_num, _get_para_num, 
        (bool[NULL_COUNT]) {true, true, true, true});
    watch_inst_conf(ISENSE_GET_WAIT, ISENSE_NULL_PTR, 
        _set_para_num, _get_para_num, 
        (bool[NULL_COUNT]) {true, true, true, true});
    
    // [5] ISense GET GAIN
    watch_inst_conf(ISENSE_GET_GAIN, ISENSE_OK, 
        _set_para_num, _get_para_num, 
        (bool[NULL_COUNT]) {true, true, true, true});
    watch_inst_conf(ISENSE_GET_GAIN, ISENSE_NULL_PTR, 
        _set_para_num, _get_para_num, 
        (bool[NULL_COUNT]) {true, true, true, true});
    
    // [6] ISense GET VAL
    watch_inst_conf(ISENSE_GET_VAL, ISENSE_OK, 
        _set_para_num, _get_para_num, 
        (bool[NULL_COUNT]) {true, true, true, true});
    watch_inst_conf(ISENSE_GET_VAL, ISENSE_NULL_PTR, 
        _set_para_num, _get_para_num, 
        (bool[NULL_COUNT]) {true, true, true, true});
    
    // [7] ISense GET STATE
    watch_inst_conf(ISENSE_GET_STATE, ISENSE_OK, 
        _set_para_num, _get_para_num, 
        (bool[NULL_COUNT]) {true, true, true, true});
    watch_inst_conf(ISENSE_GET_STATE, ISENSE_NULL_PTR, 
        _set_para_num, _get_para_num, 
        (bool[NULL_COUNT]) {true, true, true, true});
    
    // [8] ISense GET DATA DIFF
    watch_inst_conf(ISENSE_GET_DATA_DIFF, ISENSE_OK, 
        _set_para_num, _get_para_num, 
        (bool[NULL_COUNT]) {true, true, true, true});
    watch_inst_conf(ISENSE_GET_DATA_DIFF, ISENSE_NULL_PTR, 
        _set_para_num, _get_para_num, 
        (bool[NULL_COUNT]) {true, true, true, true});
    
    // [9] ISense GET READY FLAG
    watch_inst_conf(ISENSE_GET_READY_FLAG, ISENSE_OK, 
        _set_para_num, _get_para_num, 
        (bool[NULL_COUNT]) {true, true, true, true});
    watch_inst_conf(ISENSE_GET_READY_FLAG, ISENSE_NULL_PTR, 
        _set_para_num, _get_para_num, 
        (bool[NULL_COUNT]) {true, true, true, true});
    
    _set_para_num[0] = 100;
    // [10] ISense SET TIMEOUT
    watch_inst_conf(ISENSE_SET_TIMEOUT, ISENSE_OK, 
        _set_para_num, _get_para_num, 
        (bool[NULL_COUNT]) {true, true, true, true});
    watch_inst_conf(ISENSE_SET_TIMEOUT, ISENSE_NULL_PTR, 
        _set_para_num, _get_para_num, 
        (bool[NULL_COUNT]) {true, true, true, true});
    
    // [11] ISense GET TIMEOUT
    watch_inst_conf(ISENSE_GET_TIMEOUT, ISENSE_OK, 
        _set_para_num, _get_para_num, 
        (bool[NULL_COUNT]) {true, true, true, true});
    watch_inst_conf(ISENSE_GET_TIMEOUT, ISENSE_NULL_PTR, 
        _set_para_num, _get_para_num, 
        (bool[NULL_COUNT]) {true, true, true, true});

    //!NOTE! Add new function to track here
    //// [#] NEW FUNC
    //track_inst_conf(FUNC_ID, STATUS_ID, NULL, NULL, NULL);
    
    asm("NOP"); //!BREAK! Use this as breakpoint
}
#endif