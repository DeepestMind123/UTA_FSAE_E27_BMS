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
#include "dw_manager.h"

#ifdef DEMO_CAN
// Initialization
static bool init(uint32_t baudrate)            {}
static bool transmit(const can_msg_t *p_msg)   {}
static bool receive(can_msg_t *p_msg)          {}
static can_msg_t msg = {
    .id         = 1000,
    .dlc        = 4,
    .data       = {0,1,0,1,0,1,1,0},
    .is_ext     = false
};
static io_can_cfg_t config = {
    .init       = init,
    .transmit   = transmit,
    .receive    = receive
};
static io_can_t inst;

// Watch strings
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
// Helpful For ID Matching
typedef enum {
    CAN_INIT = 0,
    CAN_TRANSMIT,
    CAN_RECIEVE,
    //!NOTE! Add func id here
    //"NEW FUNC ID"
} func_id;
// Get func output to compare non-null vs. null
static int get_func(int p_func_id, bool p_not_inst_null, bool p_not_conf_null, bool p_not_null_time_inst) {
    get_para_num_1              = UINT32_MAX;
    get_para_num_2              = UINT32_MAX;
    get_para_bool_1             = false;
    io_can_t *_inst             = p_not_inst_null ? &inst : NULL;
    io_can_cfg_t *_config       = p_not_conf_null ? &config : NULL;
    
    switch(p_func_id) {
        case 0: return (uint32_t)IO_CAN_Init(_inst, _config, set_para_num_1);  break;
        case 1: return (uint32_t)IO_CAN_Transmit(_inst, &msg);          break;
        case 2: return (uint32_t)IO_CAN_Receive(_inst, &msg);           break;
        //!NOTE! Add new functions here
        //case #: return (uint32_t)IO_New_Func(&_inst, &get_para_num_1);      break;
    }
    set_para_num_1 = UINT32_MAX;
    set_para_num_2 = UINT32_MAX;
}
static driver_watcher_interface_t drive_watcher = {
    .func_strings       = func_str,
    .state_strings      = NULL,
    .status_strings     = status_str,
    .execute_func       = get_func,
    .get_state_func_id  = -1
};

// Tests all i2c methods
void demo_watch_can_all() {
    demo_watch_can_funcs();
}
// Tests the can fault system to ensure its accurate
void demo_watch_can_funcs() {
    uint32_t _baudrate = 8000;
    
    init_driver_watcher("CAN", 1, &drive_watcher); //!BREAK! Use this as breakpoint
    
    // Action Methods
    // [0] CAN INIT
    watch_inst_conf(CAN_INIT, CAN_STATUS_OK, true, true, true);
    watch_inst_conf(CAN_INIT, CAN_STATUS_ERROR_NULL_POINTER, false, true, true);
    watch_inst_conf(CAN_INIT, CAN_STATUS_ERROR_NULL_POINTER, true, false, true);
    watch_inst_conf(CAN_INIT, CAN_STATUS_ERROR_NULL_POINTER, false, false, true);
    
    // I/O Methods
    // [1] CAN TRANSMIT
    watch_inst_conf(CAN_TRANSMIT, CAN_STATUS_OK, true, true, true);
    watch_inst_conf(CAN_TRANSMIT, CAN_STATUS_ERROR_NULL_POINTER, false, false, true);
    
    // [2] CAN RECEIVE
    watch_inst_conf(CAN_RECIEVE, CAN_STATUS_OK, true, true, true);
    watch_inst_conf(CAN_RECIEVE, CAN_STATUS_ERROR_NULL_POINTER, false, false, true);
    
    //!NOTE! Add new function to track here
    //// [#] ADC NEW FUNC
    //track_inst_conf(ADC_FUNC_ID, ADC_STATUS_ID, false, false);
    
    asm("NOP"); //!BREAK! Use this as breakpoint
}
#endif