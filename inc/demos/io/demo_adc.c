/**
 * @file demo_adc.c
 * @author MITurley
 * @date 2026-06-03
 * @brief demo to test adc <SF>
 */
 
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
 
 #include "io_adc.h"
 #include "demo_tester.h"

#define DEMO_ADC
#ifdef DEMO_ADC
const char *adc_func_names[] = {
    "ADC INIT",
    "ADC TASK",
    "ADC START",
    "ADC GET VAL",
    "ADC GET RES",
    "ADC SET OFFSET",
    "ADC GET OFFSET",
    "ADC GET VREF",
    "ADC GET STATE",
    "ADC GET TIMEOUT",
};
const char *adc_states[] = {
    "IDLE",
    "START",
    "WAIT",
    "GET",
    "READY",
    "ERROR",
    "UNDEFINED",
};
const char *adc_status[] = {
    "OK",
    "NULL_POINTER",
    "NOT_INIT",
    "TIMEOUT",
    "ADC_BUSY",
    "UNDEFINED_STATE",
};
#endif

// System to log adc data via breakpoints and watches
static char watch[100]; //watch the 'watch' variable to debug
void adc_log(const char *p_func_name, const char *p_expected_status_str, int p_status, const io_adc_t *p_adc, const io_adc_cfg_t *p_adc_config, int64_t _val) {
    const char *_state_str, *_status_str;
    _status_str = adc_status[p_status];
    _state_str = adc_states[(int)IO_ADC_Get_State(p_adc, p_adc_config)];
    snprintf(log, sizeof(log), "|Func:%s|Expected Status:%s|Return Status:%s|State:%s|Data Read: %lld|", p_func_name, p_expected_status_str, _status_str, _state_str, _val);
}

// Some dummy functions for adc actions
void dummy_1(void)      { asm("NOP"); }
void dummy_2(uint8_t _) { asm("NOP"); }
uint8_t dummy_3(void)   { return 0; }
uint32_t dummy_4(void)  { return 0; }
// Tests the adc fault system to ensure its accurate
uint32_t demo_adc_fault() {
#ifdef DEMO_ADC
    io_adc_cfg_t _adc_config = {
        .adc_timeout = 1000,
        .adc_resolution = 8,
        .adc_offset = 0,
        .adc_vref_mV = 3300,
        .channel_id = 0,
        
        .ADC_channel_select = dummy_2,
        .ADC_start = dummy_1,
        .ADC_done = dummy_3,
        .ADC_get_result = dummy_4,
        .ADC_stop = dummy_1,
    };
    io_adc_t _adc_1;
    const io_adc_cfg_t *adc_config_NULL = NULL;
    io_adc_t *_adc_NULL = NULL;
    int64_t _out_val = 0;
    int16_t _set_val = 10;
    
    // Action Methods
    int _test_id = 0;
    adc_log(adc_func_names[_test_id],
            adc_status[0],
            (int)IO_ADC_Init(&_adc_1, &_adc_config),
            &_adc_1,
            &_adc_config,
            0);
    adc_log(adc_func_names[_test_id],
            adc_status[1],
            (int)IO_ADC_Init(_adc_NULL, &_adc_config),
            &_adc_1,
            &_adc_config,
            0);
    adc_log(adc_func_names[_test_id],
            adc_status[1],
            (int)IO_ADC_Init(&_adc_1, adc_config_NULL),
            &_adc_1,
            &_adc_config,
            0);
    adc_log(adc_func_names[_test_id],
            adc_status[1],
            (int)IO_ADC_Init(_adc_NULL, adc_config_NULL),
            &_adc_1,
            &_adc_config,
            0);
    
    _test_id++;
    adc_log(adc_func_names[_test_id],
            adc_status[0],
            (int)IO_ADC_Task(&_adc_1),
            &_adc_1,
            &_adc_config,
            0);
    adc_log(adc_func_names[_test_id],
            adc_status[1],
            (int)IO_ADC_Task(_adc_NULL),
            &_adc_1,
            &_adc_config,
            0);
    
    _test_id++;
    adc_log(adc_func_names[_test_id],
            adc_status[0],
            (int)IO_ADC_Start(&_adc_1),
            &_adc_1,
            &_adc_config,
            0);
    adc_log(adc_func_names[_test_id],
            adc_status[1],
            (int)IO_ADC_Start(_adc_NULL),
            &_adc_1,
            &_adc_config,
            0);
    
    // I/O Methods
    _test_id++;
    adc_log(adc_func_names[_test_id],
            adc_status[0],
            (int)IO_ADC_Get_Val(&_adc_1, &_out_val),
            &_adc_1,
            &_adc_config,
            _out_val);
    adc_log(adc_func_names[_test_id],
            adc_status[1],
            (int)IO_ADC_Get_Val(_adc_NULL, &_out_val),
            &_adc_1,
            &_adc_config,
            _out_val);
    
    
    _test_id++;
    adc_log(adc_func_names[_test_id],
            adc_status[0],
            (int)IO_ADC_Get_Resolution(&_adc_1, &_out_val),
            &_adc_1,
            &_adc_config,
            _out_val);
    adc_log(adc_func_names[_test_id],
            adc_status[1],
            (int)IO_ADC_Get_Resolution(_adc_NULL, &_out_val),
            &_adc_1,
            &_adc_config,
            _out_val);
    
    _test_id++;
    adc_log(adc_func_names[_test_id],
            adc_status[0],
            (int)IO_ADC_Set_Offset(&_adc_1, _set_val),
            &_adc_1,
            &_adc_config,
            _set_val);
    adc_log(adc_func_names[_test_id],
            adc_status[1],
            (int)IO_ADC_Set_Offset(_adc_NULL, _set_val),
            &_adc_1,
            &_adc_config,
            _set_val);
    
    _test_id++;
    adc_log(adc_func_names[_test_id],
            adc_status[0],
            (int)IO_ADC_Get_Offset(&_adc_1, &_out_val),
            &_adc_1,
            &_adc_config,
            _out_val);
    adc_log(adc_func_names[_test_id],
            adc_status[1],
            (int)IO_ADC_Get_Offset(_adc_NULL, &_out_val),
            &_adc_1,
            &_adc_config,
            _out_val);
    
    _test_id++;
    adc_log(adc_func_names[_test_id],
            adc_status[0],
            (int)IO_ADC_Get_Vref(&_adc_1, &_out_val),
            &_adc_1,
            &_adc_config,
            _out_val);
    adc_log(adc_func_names[_test_id],
            adc_status[1],
            (int)IO_ADC_Get_Vref(_adc_NULL, &_out_val),
            &_adc_1,
            &_adc_config,
            _out_val);
    
    _test_id++;
    adc_log(adc_func_names[_test_id],
            adc_status[0],
            (int)IO_ADC_Get_State(&_adc_1, &_out_val),
            &_adc_1,
            &_adc_config,
            _out_val);
    adc_log(adc_func_names[_test_id],
            adc_status[1],
            (int)IO_ADC_Get_State(_adc_NULL, &_out_val),
            &_adc_1,
            &_adc_config,
            _out_val);
    
    _test_id++;
    adc_log(adc_func_names[_test_id],
            adc_status[0],
            (int)IO_ADC_Get_Timeout(&_adc_1, &_out_val),
            &_adc_1,
            &_adc_config,
            _out_val);
    adc_log(adc_func_names[_test_id],
            adc_status[1],
            (int)IO_ADC_Get_Timeout(_adc_NULL, &_out_val),
            &_adc_1,
            &_adc_config,
            _out_val);
    return 0;
#endif
}
 
uint32_t demo_adc_get(uint32_t p_cycles) {   
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