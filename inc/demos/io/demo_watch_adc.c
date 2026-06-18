/**
 * @file demo_adc.c
 * @author MITurley
 * @date 2026-06-03
 * @brief demo to test adc <SF>
 */
 
#include <stddef.h>
#include <stdbool.h>
 
 #include "io_adc.h"
 #include "demo_watcher.h"

#ifdef DEMO_ADC
// Initialization
static void ADC_channel_select(uint8_t channel) {};
static void ADC_start(void)                     {};
static uint8_t ADC_done(void)                   {};
static uint32_t ADC_get_result(void)            {};
static void ADC_stop(void)                      {};
static io_adc_cfg_t config = {
    .adc_timeout            = 1000,
    .adc_resolution         = 8,
    .adc_offset             = 0,
    .adc_vref_mV            = 3300,
    .channel_id             = 0,

    .ADC_channel_select     = ADC_channel_select,
    .ADC_start              = ADC_start,
    .ADC_done               = ADC_done,
    .ADC_get_result         = ADC_get_result,
    .ADC_stop               = ADC_stop
};
static io_adc_t inst;

// Watch strings
static const char *func_str[] = {
    "ADC INIT",
    "ADC TASK",
    "ADC START",
    "ADC GET VAL",
    "ADC GET RES",
    "ADC SET OFFSET",
    "ADC GET OFFSET",
    "ADC GET VREF",
    "ADC GET STATE",
    "ADC GET TIMEOUT"
    //!NOTE! Add func name here
    //"NEW FUNC NAME"
};
static const char *states_str[] = {
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
static const char *status_str[] = {
    "OK",
    "NULL_POINTER",
    "NOT_INIT",
    "TIMEOUT",
    "ADC_BUSY",
    "UNDEFINED_STATE"
    //!NOTE! Add status here
    //"NEW STATUS"
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
    //!NOTE! Add func id here
    //"NEW FUNC ID"
} func_id;
// Get func output to compare non-null vs. null
static int get_func(int p_func_id, bool p_not_inst_null, bool p_not_conf_null) {
    get_val                 = UINT64_MAX;
    io_adc_t *_inst         = p_not_inst_null ? &inst : NULL;
    io_adc_cfg_t *_config   = p_not_conf_null ? &config : NULL;
    
    switch(p_func_id) {
        case 0: return (uint64_t)IO_ADC_Init(_inst, _config);              break;
        case 1: return (uint64_t)IO_ADC_Task(_inst);                       break;
        case 2: return (uint64_t)IO_ADC_Start(_inst);                      break;
        case 3: return (uint64_t)IO_ADC_Get_Val(_inst, &get_val);          break;
        case 4: return (uint64_t)IO_ADC_Get_Resolution(_inst, &get_val);   break;
        case 5: return (uint64_t)IO_ADC_Set_Offset(_inst, set_val);        break;
        case 6: return (uint64_t)IO_ADC_Get_Offset(_inst, &get_val);       break;
        case 7: return (uint64_t)IO_ADC_Get_Vref(_inst, &get_val);         break;
        case 8: return (uint64_t)IO_ADC_Get_State(_inst, &get_val);        break;
        case 9: return (uint64_t)IO_ADC_Get_Timeout(_inst, &get_val);      break;
        //!NOTE! Add new functions here
        //case #: return (uint64_t)IO_New_Func(&_inst, &get_val);      break;
    }
    set_val = UINT64_MAX;
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

// General ADC test method that goes through every function
void demo_watch_adc_funcs() {
    init_driver_watcher("ADC", 1, &drive_watcher); //!BREAK! Use this as breakpoint
    
    // Action Methods
    // [0] ADC INIT
    watch_inst_conf(ADC_INIT, ADC_STATUS_OK, true, true);
    watch_inst_conf(ADC_INIT, ADC_STATUS_ERROR_NULL_POINTER, true, false);
    watch_inst_conf(ADC_INIT, ADC_STATUS_ERROR_NULL_POINTER, false, true);
    watch_inst_conf(ADC_INIT, ADC_STATUS_ERROR_NULL_POINTER, false, false);
    
    // [1] ADC TASK
    watch_inst_conf(ADC_TASK, ADC_STATUS_OK, true, true);
    watch_inst_conf(ADC_TASK, ADC_STATUS_ERROR_NULL_POINTER, false, false);
    
    // [2] ADC START
    watch_inst_conf(ADC_START, ADC_STATUS_OK, true, true);
    watch_inst_conf(ADC_START, ADC_STATUS_ERROR_NULL_POINTER, false, false);
    
    // [3] ADC GET VAL
    watch_inst_conf(ADC_GET_VAL, ADC_STATUS_OK, true, true);     
    watch_inst_conf(ADC_GET_VAL, ADC_STATUS_ERROR_NULL_POINTER, false, false);
    
    // [4] ADC GET RES
    watch_inst_conf(ADC_GET_RES, ADC_STATUS_OK, true, true);
    watch_inst_conf(ADC_GET_RES, ADC_STATUS_ERROR_NULL_POINTER, false, false);
    
    // [5] ADC SET OFFSET
    watch_inst_conf_set(ADC_SET_OFFSET, ADC_STATUS_OK, 10, true, true);
    watch_inst_conf_set(ADC_SET_OFFSET, ADC_STATUS_ERROR_NULL_POINTER, 20, false, false);
    
    // [6] ADC GET OFFSET
    watch_inst_conf(ADC_GET_OFFSET, ADC_STATUS_OK, true, true);
    watch_inst_conf(ADC_GET_OFFSET, ADC_STATUS_ERROR_NULL_POINTER, false, false);
    
    // [7] ADC GET VREF
    watch_inst_conf(ADC_GET_VREF, ADC_STATUS_OK, true, true);
    watch_inst_conf(ADC_GET_VREF, ADC_STATUS_ERROR_NULL_POINTER, false, false);
    
    // [8] ADC GET STATE
    watch_inst_conf(ADC_GET_STATE, ADC_STATUS_OK, true, true);
    watch_inst_conf(ADC_GET_STATE, ADC_STATUS_ERROR_NULL_POINTER, false, false);
    
    // [9] ADC GET TIMEOUT
    watch_inst_conf(ADC_GET_TIMEOUT, ADC_STATUS_OK, true, true);
    watch_inst_conf(ADC_GET_TIMEOUT, ADC_STATUS_ERROR_NULL_POINTER, false, false);
    
    //!NOTE! Add new function to track here
    //// [#] ADC NEW FUNC
    //track_inst_conf(ADC_FUNC_ID, ADC_STATUS_ID, false, false);
    
    asm("NOP"); //!BREAK! Use this as breakpoint
}

// Demo to test when no init called
void demo_watch_adc_null() {
    init_driver_watcher("ADC", 2, &drive_watcher); //!BREAK! Use this as breakpoint
    
    // [1] ADC TASK
    watch_inst_conf(ADC_TASK, ADC_STATUS_ERROR_NULL_POINTER, true, true);    // State: NULL_POINTER
    
    // [0] ADC INIT
    watch_inst_conf(ADC_INIT, ADC_STATUS_ERROR_NULL_POINTER, true, true);    //
    
    // [1] ADC TASK
    watch_inst_conf(ADC_TASK, ADC_STATUS_ERROR_NULL_POINTER, false, false);  // State: NULL_POINTER
}
 
// Demo to test different task states
void demo_watch_adc_task() {
    init_driver_watcher("ADC", 3, &drive_watcher); //!BREAK! Use this as breakpoint
    
    // [0] ADC INIT
    watch_inst_conf(ADC_INIT, ADC_STATUS_OK, true, true);
    
    // [1] ADC TASK
    watch_inst_conf(ADC_TASK, ADC_STATUS_OK, true, true);
    
    // [2] ADC START
    watch_inst_conf(ADC_START, ADC_STATUS_OK, true, true);
    
    // [1] ADC TASK
    watch_inst_conf(ADC_TASK, ADC_STATUS_OK, true, true);  // State: START>>>WAIT
    watch_inst_conf(ADC_TASK, ADC_STATUS_OK, true, true);  // State: WAIT>>>GET
    watch_inst_conf(ADC_TASK, ADC_STATUS_OK, true, true);  // State: GET>>>READY
    watch_inst_conf(ADC_TASK, ADC_STATUS_OK, true, true);  // State: READY>>>IDLE
    watch_inst_conf(ADC_TASK, ADC_STATUS_OK, true, true);  // State: IDLE
}
#endif