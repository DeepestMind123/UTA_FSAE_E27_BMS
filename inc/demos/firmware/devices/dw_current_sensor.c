/**
 * @file demo_current_sensor.c
 * @author MITurley
 * @date 2026-06-03
 * @brief demo to test current_sensor <SF>
 */
 
#include <stddef.h>
#include <stdbool.h>
 
#include "dev_isense.h"
#include "io_adc.h"
#include "dw_manager.h"

#ifdef DEMO_ISENSE


    // adc_t *adc_cfg;
    // const util_time_t *time_cfg;
    // const isense_ctx_t isense_fund_cfg;
// Initialization
static isense_ctx_t ctx = {
    .timeout_ms = 1000,
    .isense_gain_uV = 500,
    .isense_raw_cutoff = 100
};
static isense_t inst;
static adc_t adc_inst;
static util_time_t time_inst;
static isense_cfg_t config = {
    .adc_cfg = &adc_inst,
    .time_cfg = &time_inst,
    .isense_fund_cfg = {
        .timeout_ms = 1000,
        .isense_gain_uV = 500,
        .isense_raw_cutoff = 100
    }
};

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
static int get_func(int p_func_id, bool* p_not_inst_null) {
    setIntArrayDefault(get_para_num, 1, UINT8_MAX);
    setBoolArrayDefault(not_null_bools, 1, false);
    isense_t *_inst             = p_not_inst_null[0] ? &inst : NULL;
    isense_cfg_t *_config       = p_not_inst_null[1] ? &config : NULL;
    
    switch(p_func_id) {
        case 0: return (int64_t)DEV_Isense_Init(_inst, _config);
        case 1: return (int64_t)DEV_Isense_Task(_inst);
        case 2: return (int64_t)DEV_Isense_Start(_inst);
        case 3: return (int64_t)DEV_Isense_Process_Raw(_inst);
        case 4: return (int64_t)DEV_Isense_Get_Wait(_inst, (uint32_t*)&get_para_num[0]);
        case 5: return (int64_t)DEV_Isense_Get_Gain(_inst, (uint32_t*)&get_para_num[0]);
        case 6: return (int64_t)DEV_Isense_Get_Val(_inst, (int32_t*)&get_para_num[0]);
        case 7: return (int64_t)DEV_Isense_Get_State(_inst, (isense_state_t*)&get_para_num[0]);
        case 8: return (int64_t)DEV_Isense_Get_Data_Diff(_inst, (bool*)&get_para_num[0]);
        case 9: return (int64_t)DEV_Isense_Get_Ready_Flag(_inst, (bool*)&get_para_num[0]);
        case 10: return (int64_t)DEV_Isense_Set_Timeout(_inst, (uint32_t)set_para_num[0]);
        case 11: return (int64_t)DEV_Isense_Get_Timeout(_inst, (uint32_t*)&get_para_num[0]);
        //!NOTE! Add new functions here
        //case #: return (int64_t)New_Func(&_inst, &get_para_num[0]);
    }
    setIntArrayDefault(set_para_num, 1, UINT8_MAX);
}
static driver_watcher_interface_t isense_test_interface = {
    .func_strings   = func_str,
    .state_strings  = states_str,
    .status_strings = status_str,
    .execute_func   = get_func,
    .get_state_func_id = 7
};

// Tests all adc methods
void demo_watch_isense_all() {
    demo_watch_isense_fault();
}

#define NULL_COUNT 2
// General ISense test method that goes through every function
void demo_watch_isense_fault() {
    init_driver_watcher("ISense", 1, &isense_test_interface);

    // Action Methods
    // [0] ISense INIT
    watch_inst_conf(ISENSE_INIT, ISENSE_OK, 
        (bool[NULL_COUNT]) {true, true});
    watch_inst_conf(ISENSE_INIT, ISENSE_NULL_PTR, 
        (bool[NULL_COUNT]) {false, false});

    // [1] ISense TASK
    watch_inst_conf(ISENSE_TASK, ISENSE_OK, 
        (bool[NULL_COUNT]) {true, true});
    watch_inst_conf(ISENSE_TASK, ISENSE_NULL_PTR, 
        (bool[NULL_COUNT]) {false, false});
    
    // [2] ISense START
    watch_inst_conf(ISENSE_START, ISENSE_OK, 
        (bool[NULL_COUNT]) {true, true});
    watch_inst_conf(ISENSE_START, ISENSE_NULL_PTR, 
        (bool[NULL_COUNT]) {false, false});
    
    // [3] ISense PROCESS RAW
    watch_inst_conf(ISENSE_PROCESS_RAW, ISENSE_OK, 
        (bool[NULL_COUNT]) {true, true});
    watch_inst_conf(ISENSE_PROCESS_RAW, ISENSE_NULL_PTR, 
        (bool[NULL_COUNT]) {false, false});

    // [4] ISense GET WAIT
    watch_inst_conf(ISENSE_GET_WAIT, ISENSE_OK, 
        (bool[NULL_COUNT]) {true, true});
    watch_inst_conf(ISENSE_GET_WAIT, ISENSE_NULL_PTR,
        (bool[NULL_COUNT]) {false, false});
    
    // [5] ISense GET GAIN
    watch_inst_conf(ISENSE_GET_GAIN, ISENSE_OK,
        (bool[NULL_COUNT]) {true, true});
    watch_inst_conf(ISENSE_GET_GAIN, ISENSE_NULL_PTR,
        (bool[NULL_COUNT]) {false, false});
    
    // [6] ISense GET VAL
    watch_inst_conf(ISENSE_GET_VAL, ISENSE_OK,
        (bool[NULL_COUNT]) {true, true});
    watch_inst_conf(ISENSE_GET_VAL, ISENSE_NULL_PTR,
        (bool[NULL_COUNT]) {false, false});
    
    // [7] ISense GET STATE
    watch_inst_conf(ISENSE_GET_STATE, ISENSE_OK,
        (bool[NULL_COUNT]) {true, true});
    watch_inst_conf(ISENSE_GET_STATE, ISENSE_NULL_PTR,
        (bool[NULL_COUNT]) {false, false});
    
    // [8] ISense GET DATA DIFF
    watch_inst_conf(ISENSE_GET_DATA_DIFF, ISENSE_OK,
        (bool[NULL_COUNT]) {true, true});
    watch_inst_conf(ISENSE_GET_DATA_DIFF, ISENSE_NULL_PTR,
        (bool[NULL_COUNT]) {false, false});
    
    // [9] ISense GET READY FLAG
    watch_inst_conf(ISENSE_GET_READY_FLAG, ISENSE_OK,
        (bool[NULL_COUNT]) {true, true});
    watch_inst_conf(ISENSE_GET_READY_FLAG, ISENSE_NULL_PTR,
        (bool[NULL_COUNT]) {false, false});
    
    set_para_num[0] = 100;
    // [10] ISense SET TIMEOUT
    watch_inst_conf(ISENSE_SET_TIMEOUT, ISENSE_OK,
        (bool[NULL_COUNT]) {true, true});
    watch_inst_conf(ISENSE_SET_TIMEOUT, ISENSE_NULL_PTR,
        (bool[NULL_COUNT]) {false, false});
    
    // [11] ISense GET TIMEOUT
    watch_inst_conf(ISENSE_GET_TIMEOUT, ISENSE_OK, 
        (bool[NULL_COUNT]) {true, true});
    watch_inst_conf(ISENSE_GET_TIMEOUT, ISENSE_NULL_PTR,
        (bool[NULL_COUNT]) {false, false});

    //!NOTE! Add new function to track here
    //// [#] NEW FUNC
    //track_inst_conf(FUNC_ID, STATUS_ID, NULL, NULL, NULL);
    
    asm("NOP"); //!BREAK! Use this as breakpoint
}
#endif