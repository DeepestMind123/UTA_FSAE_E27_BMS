/**
 * @file demo_can.c
 * @author MITurley
 * @date 2026-06-03
 * @brief demo to test can <SF>
 */
 
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
 
 #include "io_can.h"
 #include "demo_watcher.h"

#ifdef DEMO_CAN
// Initialization
bool init(uint32_t baudrate)            { return 1; }
bool transmit(const can_msg_t *p_msg)   { return 1; }
bool receive(can_msg_t *p_msg)          { return 1; }
static int dummy(void) { return 0; }    // Dummy empty function
static int dummy2(int _) { return 0; }  // Dummy with a number parameter since it needs a place for that value
static can_msg_t msg = {
    .id = 1000,
    .dlc = 4,
    .data = {0,0,0,0,0,0,0,0},
    .is_ext = false
};
static io_can_cfg_t config = {
    .init = init,
    .transmit = transmit,
    .receive = receive
};
static io_can_t inst;
static const io_can_cfg_t *config_null  = NULL;
static const io_can_t *inst_null        = NULL;

static const char *func_str[] = {
    "CAN INIT",
    "CAN TRANSMIT",
    "CAN RECIEVE"
    //!NOTE! Add func name here
    //"NEW FUNC NAME"
};
static const char *status_str[] = {
    "OK",
    "NULL POINTER",
    "NOT INIT",
    "TRANSMIT FAIL",
    "RECIEVE FAIL",
    "MAX"
    //!NOTE! Add status here
    //"NEW STATUS"
};
// Get func output to compare non-null vs. null
int get_func(int p_func_id, bool p_not_inst_null, bool p_not_conf_null) {
    get_val         = UINT64_MAX;
    io_can_t *_inst    = p_not_inst_null ? &inst : NULL;
    io_can_cfg_t *_config = p_not_conf_null ? &config : NULL;
    
    switch(p_func_id) {
        case 0: return (uint64_t)IO_CAN_Init(_inst, _config, set_val);   break;
        case 1: return (uint64_t)IO_CAN_Transmit(_inst, &msg);        break;
        case 2: return (uint64_t)IO_CAN_Receive(_inst, &msg);         break;
        //!NOTE! Add new functions here
        //case #: return (uint64_t)IO_New_Func(&_inst, &get_val);      break;
    }
    set_val = UINT64_MAX;
}
static periph_watcher_t adc_test_interface = {
    .func_strings   = func_str,
    .state_strings  = NULL,
    .status_strings = status_str,
    .execute_func   = get_func,
    .get_state_func_id = -1
};
// Tests the can fault system to ensure its accurate
uint32_t demo_watch_can_fault() {
    int64_t _out_val = 0;
    int16_t _set_val = 10;
    uint32_t _baudrate = 8000;
    
    init_driver(&adc_test_interface);
    
    // Action Methods
    int _func_id = 0, _status_exp = 0; //!BREAK! Use this as breakpoint
    _func_id = 0;   // CAN INIT
    watch_inst_conf(_func_id, _status_exp, true, true);
    watch_inst_conf(_func_id, _status_exp, true, false);
    watch_inst_conf(_func_id, _status_exp, false, true);
    watch_inst_conf(_func_id, _status_exp, false, false);
    
    _func_id++;     // CAN TRANSMIT
    watch_inst_conf(_func_id, _status_exp, true, true);
    watch_inst_conf(_func_id, _status_exp, false, false);
    
    _func_id++;     // CAN RECEIVE
    watch_inst_conf(_func_id, _status_exp, true, true);
    watch_inst_conf(_func_id, _status_exp, false, false);
    
    // I/O Methods
    return 0;
}
#endif