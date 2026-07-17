/**
 * @file demo_i2c.c
 * @author MITurley
 * @date 2026-06-03
 * @brief demo to test i2c <SF>
 */
 
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
 
 #include "io_i2c.h"
#include "dw_manager.h"

#ifdef DEMO_I2C
// Initialization
static bool init(uint8_t addr)                  {};
static bool set_clock(uint32_t speed)           {};
static bool transmit(const i2c_msg_t *p_msg)    {};
static bool receive(i2c_msg_t *p_msg)           {};
static i2c_msg_t msg = {
    .addr = 1000,
    .comm = 4,
    .data = {0,1,0,1,0,1,1,0},
};
static io_i2c_cfg_t config = {
    .init       = init,
    .set_clock  = set_clock,
    .transmit   = transmit,
    .receive    = receive
};
static io_i2c_t inst;

// Watch strings
static const char *func_str[] = {
    "INIT",
    "SET CLOCK",
    "TRANSMIT",
    "RECIEVE"
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
    I2C_INIT = 0,
    I2C_SET_CLOCK,
    I2C_TRANSMIT,
    I2C_RECIEVE,
    //!NOTE! Add func id here
    //"NEW FUNC ID"
} func_id;
// Get func output to compare non-null vs. null
int get_func(int p_func_id, bool* p_not_inst_null) {
    get_para_num                = getIntArrayDefault(1, UINT64_MAX);
    not_null_bools              = getBoolArrayDefault(1, false);
    io_i2c_t *_inst             = p_not_inst_null[0] ? &inst : NULL;
    io_i2c_cfg_t *_config       = p_not_inst_null[1] ? &config : NULL;
    
    switch(p_func_id) {
        case 0: return (int64_t)IO_I2C_Init(_inst, _config, set_para_num[0]);      break;
        case 1: return (int64_t)IO_I2C_Set_Clock(_inst, set_para_num[0]);               break;
        case 2: return (int64_t)IO_I2C_Transmit(_inst, &msg);                          break;
        case 3: return (int64_t)IO_I2C_Receive(_inst, &msg);                           break;
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
void demo_watch_i2c_all() {
    demo_watch_i2c_funcs();
}
#define NULL_COUNT 2
// Tests the i2c fault system to ensure its accurate
void demo_watch_i2c_funcs() {
    init_driver_watcher("I2C", 1, &drive_watcher); //!BREAK! Use this as breakpoint
    
    // Action Methods
    // [0] I2C INIT
    watch_inst_conf(I2C_INIT, I2C_STATUS_OK,
        (bool[NULL_COUNT]) {true, true, true});
    watch_inst_conf(I2C_INIT, I2C_STATUS_ERROR_NULL_POINTER,
        (bool[NULL_COUNT]) {false, true, true});
    watch_inst_conf(I2C_INIT, I2C_STATUS_ERROR_NULL_POINTER,
        (bool[NULL_COUNT]) {true, false, true});
    watch_inst_conf(I2C_INIT, I2C_STATUS_ERROR_NULL_POINTER,
        (bool[NULL_COUNT]) {false, false, false});
    
    set_para_num[0] = 100;
    // I/O Methods
    // [1] I2C SET CLOCK
    watch_inst_conf(I2C_SET_CLOCK, I2C_STATUS_OK,
        (bool[NULL_COUNT]) {true, true, true, true});
    watch_inst_conf(I2C_SET_CLOCK, I2C_STATUS_ERROR_NULL_POINTER,
        (bool[NULL_COUNT]) {true, true, true, true});

    // [2] I2C TRANSMIT
    watch_inst_conf(I2C_TRANSMIT, I2C_STATUS_OK,
        (bool[NULL_COUNT]) {true, true, true, true});
    watch_inst_conf(I2C_TRANSMIT, I2C_STATUS_ERROR_NULL_POINTER,
        (bool[NULL_COUNT]) {false, false, true, true});
    
    // [3] I2C RECEIVE
    watch_inst_conf(I2C_RECIEVE, I2C_STATUS_OK,
        (bool[NULL_COUNT]) {true, true, true, true});
    watch_inst_conf(I2C_RECIEVE, I2C_STATUS_ERROR_NULL_POINTER,
        (bool[NULL_COUNT]) {false, false, true, true});
    
    //!NOTE! Add new function to track here
    //// [#] NEW FUNC
    //track_inst_conf(FUNC_ID, STATUS_ID, NULL, NULL, NULL);
    
    asm("NOP"); //!BREAK! Use this as breakpoint
}
#endif