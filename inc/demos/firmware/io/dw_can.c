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
int get_func(int p_func_id, bool* p_not_inst_null) {
    get_para_num                = getIntArrayDefault(1, UINT64_MAX);
    not_null_bools              = getBoolArrayDefault(1, false);
    io_can_t *_inst             = p_not_inst_null[0] ? &inst : NULL;
    io_can_cfg_t *_config       = p_not_inst_null[1] ? &config : NULL;
    
    switch(p_func_id) {
        case 0: return (int64_t)IO_CAN_Init(_inst, _config, set_para_num[0]);  break;
        case 1: return (int64_t)IO_CAN_Transmit(_inst, &msg);                          break;
        case 2: return (int64_t)IO_CAN_Receive(_inst, &msg);                           break;
        //!NOTE! Add new functions here
        //case #: return (int64_t)New_Func(&_inst, &get_para_num[0]);      break;
    }
    set_para_num = getIntArrayDefault(1, UINT64_MAX);
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
#define NULL_COUNT 2
// Tests the can fault system to ensure its accurate
void demo_watch_can_funcs() {
    uint32_t _baudrate = 8000;
    
    init_driver_watcher("CAN", 1, &drive_watcher); //!BREAK! Use this as breakpoint
    
    // Action Methods
    // [0] CAN INIT
    watch_inst_conf(CAN_INIT, CAN_STATUS_OK,
        (bool[NULL_COUNT]) {true, true, true, true});
    watch_inst_conf(CAN_INIT, CAN_STATUS_ERROR_NULL_POINTER,
        (bool[NULL_COUNT]) {false, true, true, true});
    watch_inst_conf(CAN_INIT, CAN_STATUS_ERROR_NULL_POINTER,
        (bool[NULL_COUNT]) {true, false, true, true});
    watch_inst_conf(CAN_INIT, CAN_STATUS_ERROR_NULL_POINTER,
        (bool[NULL_COUNT]) {false, false, true, true});
    
    // I/O Methods
    // [1] CAN TRANSMIT
    watch_inst_conf(CAN_TRANSMIT, CAN_STATUS_OK,
        (bool[NULL_COUNT]) {true, true, true, true});
    watch_inst_conf(CAN_TRANSMIT, CAN_STATUS_ERROR_NULL_POINTER,
        (bool[NULL_COUNT]) {false, false, true, true});
    
    // [2] CAN RECEIVE
    watch_inst_conf(CAN_RECIEVE, CAN_STATUS_OK,
        (bool[NULL_COUNT]) {true, true, true, true});
    watch_inst_conf(CAN_RECIEVE, CAN_STATUS_ERROR_NULL_POINTER,
        (bool[NULL_COUNT]) {false, false, true, true});
    
    //!NOTE! Add new function to track here
    //// [#] NEW FUNC
    //track_inst_conf(FUNC_ID, STATUS_ID, NULL, NULL, NULL);
    
    asm("NOP"); //!BREAK! Use this as breakpoint
}
#endif