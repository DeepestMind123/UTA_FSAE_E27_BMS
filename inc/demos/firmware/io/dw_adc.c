/**
 * @file demo_adc.c
 * @author MITurley
 * @date 2026-06-03
 * @brief demo to test adc <SF>
 */
 
#include <stddef.h>
#include <stdbool.h>
 
#include "io_adc.h"
#include "dw_manager.h"

#ifdef DEMO_ADC
// Initialization
static void ADC_channel_select(uint8_t channel) {};
static void ADC_start(void)                     {};
static uint8_t ADC_done(void)                   {};
static uint16_t ADC_get_result(void)            {};
static void ADC_stop(void)                      {};
static util_time_t time_inst = {
    .is_init = false,
    .tick = 0U,
    .irq = NULL
};
static adc_cfg_t config = {
    .adc_func_cfg = {
        .ADC_channel_select = ADC_channel_select,
        .ADC_start = ADC_start,
        .ADC_done = ADC_done,
        .ADC_get_result = ADC_get_result,
        .ADC_stop = ADC_stop
    },
    .ctx_cfg = {
        .adc_timeout = 1000,
        .adc_wait_ms = 10,
        .adc_resolution = 8,
        .adc_vref_mV = 3300,
        .adc_offset = 0,
        .channel_id = 0
    },
    .time_cfg = &time_inst
};
static adc_t inst;

// Watch strings
static const char *func_str[] = {
    "INIT",
    "TASK",
    "START",
    "GET VAL",
    "GET RES",
    "SET OFFSET",
    "GET OFFSET",
    "GET VREF",
    "GET STATE",
    "GET TIMEOUT",
    "GET READY FLAG"
    //!NOTE! Add func name here
    //"NEW FUNC NAME"
};
static const char *status_str[] = {
    "OK",
    "NULL_POINTER",
    "NOT_INIT",
    "TIMEOUT",
    "ADC_BUSY",
    "TIME_ERROR",
    "UNDEFINED_STATE",
    "MAX"
    //!NOTE! Add status here
    //"NEW STATUS"
};
static const char *states_str[] = {
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
    ADC_INIT = 0,
    ADC_TASK,
    ADC_START,
    ADC_GET_VAL,
    ADC_GET_RES,
    ADC_SET_OFFSET,
    ADC_GET_OFFSET,
    ADC_GET_VREF,
    ADC_GET_STATE,
    ADC_GET_TIMEOUT,
    ADC_GET_READY_FLAG,
    //!NOTE! Add func id here
    //"NEW FUNC ID"
} func_id;
// Get func output to compare non-null vs. null
static int get_func(int p_func_id, bool* p_not_inst_null) {
    setIntArrayDefault(get_para_num, 1, UINT8_MAX);
    setBoolArrayDefault(not_null_bools, 1, false);
    adc_t *_inst             = p_not_inst_null[0] ? &inst : NULL;
    adc_cfg_t *_config       = p_not_inst_null[1] ? &config : NULL;
    util_time_t *_time_inst     = p_not_inst_null[2] ? &time_inst : NULL;
    switch(p_func_id) {
        case 0: return (uint64_t)IO_ADC_Init(_inst, _config);       break;
        case 1: return (uint64_t)IO_ADC_Task(_inst);                                               break;
        case 2: return (uint64_t)IO_ADC_Start(_inst);                                              break;
        case 3: return (uint64_t)IO_ADC_Get_Val(_inst, (uint16_t*)&get_para_num[0]);                    break;
        case 4: return (uint64_t)IO_ADC_Get_Resolution(_inst, (uint16_t*)&get_para_num[0]);             break;
        case 5: return (uint64_t)IO_ADC_Set_Offset(_inst, (uint16_t)set_para_num[0]);                break;
        case 6: return (uint64_t)IO_ADC_Get_Offset(_inst, (int16_t*)&get_para_num[0]);                 break;
        case 7: return (uint64_t)IO_ADC_Get_Vref(_inst, (uint16_t*)&get_para_num[0]);                   break;
        case 8: return (uint64_t)IO_ADC_Get_State(_inst, (adc_state_t*)&get_para_num[0]);                  break;
        case 9: return (uint64_t)IO_ADC_Get_Timeout(_inst, (uint32_t*)&get_para_num[0]);                break;
        case 10: return (uint64_t)IO_ADC_Get_Ready_Flag(_inst, (bool*)&not_null_bools[0]);           break;
        //!NOTE! Add new functions here
        //case #: return (int64_t)New_Func(&_inst, &get_para_num[0]);      break;
    }
    setIntArrayDefault(set_para_num, 1, UINT8_MAX);
}
static driver_watcher_interface_t drive_watcher = {
    .func_strings       = func_str,
    .state_strings      = states_str,   //!NOTE! Set to NULL if there are no states
    .status_strings     = status_str,
    .execute_func       = get_func,
    .get_state_func_id  = 8          //!NOTE! Set value to get state func id
};

// Tests all adc methods
void demo_watch_adc_all() {
    demo_watch_adc_funcs();
//    demo_watch_adc_null();
//    demo_watch_adc_task();
}

#define NULL_COUNT 3
// General ADC test method that goes through every function
void demo_watch_adc_funcs() {
    init_driver_watcher("ADC", 1, &drive_watcher); //!BREAK! Use this as breakpoint
    
    // Action Methods
    // [0] ADC INIT
    watch_inst_conf(ADC_INIT, ADC_OK,
        (bool[NULL_COUNT]) {true, true, true});
    watch_inst_conf(ADC_INIT, ADC_NULL_PTR,
        (bool[NULL_COUNT]) {false, true, true});
    watch_inst_conf(ADC_INIT, ADC_NULL_PTR,
        (bool[NULL_COUNT]) {true, false, true});
    watch_inst_conf(ADC_INIT, ADC_NULL_PTR,
        (bool[NULL_COUNT]) {true, true, false});
    watch_inst_conf(ADC_INIT, ADC_NULL_PTR,
        (bool[NULL_COUNT]) {true, false, false});
    watch_inst_conf(ADC_INIT, ADC_NULL_PTR,
        (bool[NULL_COUNT]) {false, true, false});
    watch_inst_conf(ADC_INIT, ADC_NULL_PTR,
        (bool[NULL_COUNT]) {false, false, true});
    watch_inst_conf(ADC_INIT, ADC_NULL_PTR,
        (bool[NULL_COUNT]) {false, false, false});
    
    // [1] ADC TASK
    watch_inst_conf(ADC_TASK, ADC_OK,
        (bool[NULL_COUNT]) {true, true, true});
    watch_inst_conf(ADC_TASK, ADC_NULL_PTR,
        (bool[NULL_COUNT]) {false, false, true});
    
    // [2] ADC START
    watch_inst_conf(ADC_START, ADC_OK,
        (bool[NULL_COUNT]) {true, true, true});
    watch_inst_conf(ADC_START, ADC_NULL_PTR,
        (bool[NULL_COUNT]) {false, false, true});
    
    // [3] ADC GET VAL
    watch_inst_conf(ADC_GET_VAL, ADC_OK,
        (bool[NULL_COUNT]) {true, true, true});
    watch_inst_conf(ADC_GET_VAL, ADC_NULL_PTR,
        (bool[NULL_COUNT]) {false, false, true});
    
    // [4] ADC GET RES
    watch_inst_conf(ADC_GET_RES, ADC_OK,
        (bool[NULL_COUNT]) {true, true, true});
    watch_inst_conf(ADC_GET_RES, ADC_NULL_PTR,
        (bool[NULL_COUNT]) {false, false, true});
    
    set_para_num[0] = 100;
    // [5] ADC SET OFFSET
    watch_inst_conf(ADC_SET_OFFSET, ADC_OK,
        (bool[NULL_COUNT]) {true, true, true});
    watch_inst_conf(ADC_SET_OFFSET, ADC_NULL_PTR,
        (bool[NULL_COUNT]) {false, false, true});
    
    // [6] ADC GET OFFSET
    watch_inst_conf(ADC_GET_OFFSET, ADC_OK,
        (bool[NULL_COUNT]) {true, true, true});
    watch_inst_conf(ADC_GET_OFFSET, ADC_NULL_PTR,
        (bool[NULL_COUNT]) {false, false, true});
    
    // [7] ADC GET VREF
    watch_inst_conf(ADC_GET_VREF, ADC_OK,
        (bool[NULL_COUNT]) {true, true, true});
    watch_inst_conf(ADC_GET_VREF, ADC_NULL_PTR,
        (bool[NULL_COUNT]) {false, false, true});
    
    // [8] ADC GET STATE
    watch_inst_conf(ADC_GET_STATE, ADC_OK,
        (bool[NULL_COUNT]) {true, true, true});
    watch_inst_conf(ADC_GET_STATE, ADC_NULL_PTR,
        (bool[NULL_COUNT]) {false, false, true});
    
    // [9] ADC GET TIMEOUT
    watch_inst_conf(ADC_GET_TIMEOUT, ADC_OK,
        (bool[NULL_COUNT]) {true, true, true});
    watch_inst_conf(ADC_GET_TIMEOUT, ADC_NULL_PTR,
        (bool[NULL_COUNT]) {false, false, true});
    
    //!NOTE! Add new function to track here
    //// [#] ADC NEW FUNC
    //track_inst_conf(ADC_FUNC_ID, ADC_STATUS_ID, false, false);
    
    asm("NOP"); //!BREAK! Use this as breakpoint
}

// Demo to test when no init called
void demo_watch_adc_null() {
    init_driver_watcher("ADC", 2, &drive_watcher); //!BREAK! Use this as breakpoint
    
    // [1] ADC TASK
    watch_inst_conf(ADC_TASK, ADC_NULL_PTR,
        (bool[NULL_COUNT]) {true, true, true});    // State: NULL_POINTER
    
    // [0] ADC INIT
    watch_inst_conf(ADC_INIT, ADC_NULL_PTR,
        (bool[NULL_COUNT]) {true, true, true});    //
    
    // [1] ADC TASK
    watch_inst_conf(ADC_TASK, ADC_NULL_PTR,
        (bool[NULL_COUNT]) {false, false, true});  // State: NULL_POINTER
}
 
// Demo to test different task states
void demo_watch_adc_task() {
    init_driver_watcher("ADC", 3, &drive_watcher); //!BREAK! Use this as breakpoint
    
    // [0] ADC INIT
    watch_inst_conf(ADC_INIT, ADC_OK,
        (bool[NULL_COUNT]) {true, true, true});
    
    // [1] ADC TASK
    watch_inst_conf(ADC_TASK, ADC_OK,
        (bool[NULL_COUNT]) {true, true, true});
    
    // [2] ADC START
    watch_inst_conf(ADC_START, ADC_OK,
        (bool[NULL_COUNT]) {true, true, true});
    
    // [1] ADC TASK
    watch_inst_conf(ADC_TASK, ADC_OK,
        (bool[NULL_COUNT]) {true, true, true});  // State: START>>>WAIT
    watch_inst_conf(ADC_TASK, ADC_OK,
        (bool[NULL_COUNT]) {true, true, true});  // State: WAIT>>>GET
    watch_inst_conf(ADC_TASK, ADC_OK,
        (bool[NULL_COUNT]) {true, true, true});  // State: GET>>>READY
    watch_inst_conf(ADC_TASK, ADC_OK,
        (bool[NULL_COUNT]) {true, true, true});  // State: READY>>>IDLE
    watch_inst_conf(ADC_TASK, ADC_OK,
        (bool[NULL_COUNT]) {true, true, true});  // State: IDLE
}
#endif