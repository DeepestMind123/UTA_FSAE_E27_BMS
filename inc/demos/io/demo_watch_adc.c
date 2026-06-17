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
static int dummy(void) { return 0; }    // Dummy empty function
static int dummy2(int _) { return 0; }  // Dummy with a number parameter since it needs a place for that value
static io_adc_cfg_t config = {
    .adc_timeout            = 1000,
    .adc_resolution         = 8,
    .adc_offset             = 0,
    .adc_vref_mV            = 3300,
    .channel_id             = 0,

    .ADC_channel_select     = (void (*)(uint8_t))dummy2,
    .ADC_start              = (void (*)(void))dummy,
    .ADC_done               = (uint8_t (*)(void))dummy,
    .ADC_get_result         = (uint32_t (*)(void))dummy,
    .ADC_stop               = (void (*)(void))dummy
};
static io_adc_t inst;
static const io_adc_cfg_t *config_null  = NULL;
static const io_adc_t *inst_null        = NULL;

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
// Get func output to compare non-null vs. null
int get_func(int p_func_id, bool p_not_inst_null, bool p_not_conf_null) {
    get_val         = UINT64_MAX;
    io_adc_t *_inst    = p_not_inst_null ? &inst : NULL;
    io_adc_cfg_t *_config = p_not_conf_null ? &config : NULL;
    
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
static periph_watcher_t adc_test_interface = {
    .func_strings   = func_str,
    .state_strings  = states_str,
    .status_strings = status_str,
    .execute_func   = get_func,
    .get_state_func_id = 8
};

// General ADC Test Method
uint32_t demo_watch_adc_fault() {
    init_driver(&adc_test_interface);
    
    // Action Methods
    int _func_id = 0, _status_exp = 0; //!BREAK! Use this as breakpoint
    _func_id = 0;   // ADC INIT
    watch_inst_conf(_func_id, _status_exp, true, true);
    watch_inst_conf(_func_id, _status_exp, true, false);
    watch_inst_conf(_func_id, _status_exp, false, true);
    watch_inst_conf(_func_id, _status_exp, false, false);
    
    _func_id++;     // ADC TASK
    watch_inst_conf(_func_id, _status_exp, true, true);
    watch_inst_conf(_func_id, _status_exp, false, false);
    
    _func_id++;     // ADC START
    watch_inst_conf(_func_id, _status_exp, true, true);
    watch_inst_conf(_func_id, _status_exp, false, false);
    
    _func_id++;     // ADC GET VAL
    watch_inst_conf(_func_id, _status_exp, true, true);     
    watch_inst_conf(_func_id, _status_exp, false, false);
    
    _func_id++;     // ADC GET RES
    watch_inst_conf(_func_id, _status_exp, true, true);
    watch_inst_conf(_func_id, _status_exp, false, false);
    
    _func_id++;     // ADC SET OFFSET
    watch_inst_conf(_func_id, _status_exp, true, true);
    watch_inst_conf(_func_id, _status_exp, false, false);
    
    _func_id++;     // ADC GET OFFSET
    watch_inst_conf_set(_func_id, _status_exp, 10, true, true);
    watch_inst_conf_set(_func_id, _status_exp, 20, false, false);
    
    _func_id++;     // ADC GET VREF
    watch_inst_conf(_func_id, _status_exp, true, true);
    watch_inst_conf(_func_id, _status_exp, false, false);
    
    _func_id++;     // ADC GET STATE
    watch_inst_conf(_func_id, _status_exp, true, true);
    watch_inst_conf(_func_id, _status_exp, false, false);
    
    _func_id++;     // ADC GET TIMEOUT
    watch_inst_conf(_func_id, _status_exp, true, true);
    watch_inst_conf(_func_id, _status_exp, false, false);
    
    //!NOTE! Add new function to track here
    //_test_id++;     // ADC NEW FUNC
    //track_inst_conf(_test_id, _status_exp, false, false);
    
    asm("NOP"); //!BREAK! Use this as breakpoint
    return 0;
}
 
// !LATER! Currently unimplemented get value method
uint32_t demo_watch_adc_get(uint32_t p_cycles) {
//        IO_ADC_Task(&_adc_1);
//        snprintf(log, sizeof(log), "|Cycle: %lu|ADC State:%s|Data Read: %lu|", (unsigned long)i, state, (unsigned long)_val);
//        IO_ADC_Start(&_adc_1);
//    
//    IO_ADC_Init(&_adc1, &adc_config);
//    for(uint32_t i = 1; i <= p_cycles+1; i++) {
//        IO_ADC_Task(&_adc1);
//        uint32_t _val;
//        char *state = adc_states[IO_ADC_Get_Val(&_adc1, &_val)];
//        char log[100];
//        snprintf(log, sizeof(log), "|Cycle: %lu|ADC State:%s|Data Read: %lu|", (unsigned long)i, state, (unsigned long)_val);
//        IO_ADC_Start(&_adc1);
//    }
}
#endif