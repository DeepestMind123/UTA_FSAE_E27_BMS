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
static bool ADC_done(void)                      { return 0; };
static uint16_t ADC_get_result(void)            { return 0; };
static void ADC_stop(void)                      {};
static util_time_t time_inst = {
    .is_init = false,
    .tick = 0U,
    .irq = NULL
};
static adc_cfg_t config = {
    .adc_func_cfg = {
        .ADC_channel_select = ADC_channel_select,
        .ADC_start          = ADC_start,
        .ADC_done           = ADC_done,
        .ADC_get_result     = ADC_get_result,
        .ADC_stop           = ADC_stop
    },
    .ctx_cfg = {
        .adc_timeout    = 1000,
        .adc_wait_ms    = 10,
        .adc_resolution = 8,
        .adc_vref_mV    = 3300,
        .adc_offset     = 0,
        .channel_id     = 0
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
    "NULL_FUNC",
    "NOT_INIT",
    "DBL_INIT",
    "TIMEOUT",
    "ADC_BUSY",
    "TIME_FAULT",
    "UNDEF_STATE",
    "INVALID_CFG",
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
#define NULL_COUNT 2
// Get func output to compare non-null vs. null
static int get_func(int p_func_id) {
    setIntArrayDefault(get_para_int, PARAMSSIZE, UINT8_MAX);
    
    adc_t *_inst        = not_null_bools[0] ? &inst : NULL;
    adc_cfg_t *_config  = not_null_bools[1] ? &config : NULL;
    
    switch(p_func_id) {
        case 0: return (uint64_t)IO_ADC_Init(_inst, _config);
        case 1: return (uint64_t)IO_ADC_Task(_inst);
        case 2: return (uint64_t)IO_ADC_Start(_inst);
        case 3: return (uint64_t)IO_ADC_Get_Val(_inst, (uint16_t*)&get_para_int[0]);
        case 4: return (uint64_t)IO_ADC_Get_Resolution(_inst, (uint16_t*)&get_para_int[0]);
        case 5: return (uint64_t)IO_ADC_Set_Offset(_inst, (uint16_t)set_para_int[0]);
        case 6: return (uint64_t)IO_ADC_Get_Offset(_inst, (int16_t*)&get_state);
        case 7: return (uint64_t)IO_ADC_Get_Vref(_inst, (uint16_t*)&get_para_int[0]);
        case 8: return (uint64_t)IO_ADC_Get_State(_inst, (adc_state_t*)&get_state);
        case 9: return (uint64_t)IO_ADC_Get_Timeout(_inst, (uint32_t*)&get_para_int[0]);
        case 10: return (uint64_t)IO_ADC_Get_Ready_Flag(_inst, (bool*)&not_null_bools[0]);
        //!NOTE! Add new functions here
        //case #: return (int64_t)New_Func(&_inst, &get_para_num[0]);
    }
    setBoolArrayDefault(not_null_bools, NULL_COUNT, false);
    setIntArrayDefault(set_para_int, PARAMSSIZE, UINT8_MAX);
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
    demo_watcher_init(NULL_COUNT);
    // demo_watch_adc_funcs();
//    demo_watch_adc_init();
   demo_watch_adc_task();
}
// General ADC test method that goes through every function
void demo_watch_adc_funcs() {
    init_driver_watcher("ADC", 1, &drive_watcher); //!BREAK! Use this as breakpoint
    
    // Action Methods
    // [0] ADC INIT
    SET_NULL_FLAGS(true, true);
    watch_inst_conf(ADC_INIT, ADC_OK);
    SET_NULL_FLAGS(false, true);
    watch_inst_conf(ADC_INIT, ADC_NULL_PTR);
    SET_NULL_FLAGS(true, false);
    watch_inst_conf(ADC_INIT, ADC_NULL_PTR);
    SET_NULL_FLAGS(false, false);
    watch_inst_conf(ADC_INIT, ADC_NULL_PTR);
    
    // [1] ADC TASK
    SET_NULL_FLAGS(true, true);
    watch_inst_conf(ADC_TASK, ADC_OK);
    SET_NULL_FLAGS(false, false);
    watch_inst_conf(ADC_TASK, ADC_NULL_PTR);
    
    // [2] ADC START
    SET_NULL_FLAGS(true, true);
    watch_inst_conf(ADC_START, ADC_OK);
    SET_NULL_FLAGS(false, false);
    watch_inst_conf(ADC_START, ADC_NULL_PTR);
    
    // [3] ADC GET VAL
    SET_NULL_FLAGS(true, true);
    watch_inst_conf(ADC_GET_VAL, ADC_OK);
    SET_NULL_FLAGS(false, false);
    watch_inst_conf(ADC_GET_VAL, ADC_NULL_PTR);
    
    // [4] ADC GET RES
    SET_NULL_FLAGS(true, true);
    watch_inst_conf(ADC_GET_RES, ADC_OK);
    SET_NULL_FLAGS(false, false);
    watch_inst_conf(ADC_GET_RES, ADC_NULL_PTR);
    
    set_para_int[0] = 100;
    // [5] ADC SET OFFSET
    SET_NULL_FLAGS(true, true);
    watch_inst_conf(ADC_SET_OFFSET, ADC_OK);
    SET_NULL_FLAGS(false, false);
    watch_inst_conf(ADC_SET_OFFSET, ADC_NULL_PTR);
    
    // [6] ADC GET OFFSET
    SET_NULL_FLAGS(true, true);
    watch_inst_conf(ADC_GET_OFFSET, ADC_OK);
    SET_NULL_FLAGS(false, false);
    watch_inst_conf(ADC_GET_OFFSET, ADC_NULL_PTR);
    
    // [7] ADC GET VREF
    SET_NULL_FLAGS(true, true);
    watch_inst_conf(ADC_GET_VREF, ADC_OK);
    SET_NULL_FLAGS(false, false);
    watch_inst_conf(ADC_GET_VREF, ADC_NULL_PTR);
    
    // [8] ADC GET STATE
    SET_NULL_FLAGS(true, true);
    watch_inst_conf(ADC_GET_STATE, ADC_OK);
    SET_NULL_FLAGS(false, false);
    watch_inst_conf(ADC_GET_STATE, ADC_NULL_PTR);
    
    // [9] ADC GET TIMEOUT
    SET_NULL_FLAGS(true, true);
    watch_inst_conf(ADC_GET_TIMEOUT, ADC_OK);
    SET_NULL_FLAGS(false, false);
    watch_inst_conf(ADC_GET_TIMEOUT, ADC_NULL_PTR);
    
    //!NOTE! Add new function to track here
    //// [#] ADC NEW FUNC
    //track_inst_conf(ADC_FUNC_ID, ADC_STATUS_ID, false, false);
    
    asm("NOP"); //!BREAK! Use this as breakpoint
}

// Demo to test when no init called
void demo_watch_adc_init() {
    init_driver_watcher("ADC", 2, &drive_watcher); //!BREAK! Use this as breakpoint
    
    // [1] ADC TASK
    SET_NULL_FLAGS(true, true, true);
    watch_inst_conf(ADC_TASK, ADC_NOT_INIT);    // State: WAIT
    
    // [0] ADC INIT
    SET_NULL_FLAGS(true, true, true);
    watch_inst_conf(ADC_INIT, ADC_OK);          // State: OK
    
    // [1] ADC TASK
    SET_NULL_FLAGS(false, false, false);
    watch_inst_conf(ADC_TASK, ADC_NULL_PTR);    // State: IDLE
}
 
// Demo to test different task states
void demo_watch_adc_task() {
    init_driver_watcher("ADC", 3, &drive_watcher); //!BREAK! Use this as breakpoint
    
    // [0] ADC INIT
    SET_NULL_FLAGS(true, true, true);
    watch_inst_conf(ADC_INIT, ADC_OK);
    
    // [1] ADC TASK
    SET_NULL_FLAGS(true, true, true);
    watch_inst_conf(ADC_TASK, ADC_OK);
    
    // [2] ADC START
    SET_NULL_FLAGS(true, true, true);
    watch_inst_conf(ADC_START, ADC_OK);
    
    // [1] ADC TASK
    SET_NULL_FLAGS(true, true, true);
    watch_inst_conf(ADC_TASK, ADC_OK);  // State: START>>>WAIT
    SET_NULL_FLAGS(true, true, true);
    watch_inst_conf(ADC_TASK, ADC_OK);  // State: WAIT>>>GET
    SET_NULL_FLAGS(true, true, true);
    watch_inst_conf(ADC_TASK, ADC_OK);  // State: GET>>>READY
    SET_NULL_FLAGS(true, true, true);
    watch_inst_conf(ADC_TASK, ADC_OK);  // State: READY>>>IDLE
    SET_NULL_FLAGS(true, true, true);
    watch_inst_conf(ADC_TASK, ADC_OK);  // State: IDLE
}
#endif