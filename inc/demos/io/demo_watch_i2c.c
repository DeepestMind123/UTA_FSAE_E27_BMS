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
 #include "demo_watcher.h"

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
    "I2C INIT",
    "I2C SET CLOCK",
    "I2C TRANSMIT",
    "I2C RECIEVE"
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
static int get_func(int p_func_id, bool p_not_inst_null, bool p_not_conf_null) {
    get_val                 = UINT64_MAX;
    io_i2c_t *_inst         = p_not_inst_null ? &inst : NULL;
    io_i2c_cfg_t *_config   = p_not_conf_null ? &config : NULL;
    
    switch(p_func_id) {
        case 0: return (uint64_t)IO_I2C_Init(_inst, _config, set_val);    break;
        case 1: return (uint64_t)IO_I2C_Set_Clock(_inst, set_val);                  break;
        case 2: return (uint64_t)IO_I2C_Transmit(_inst, &msg);                  break;
        case 3: return (uint64_t)IO_I2C_Receive(_inst, &msg);                   break;
        //!NOTE! Add new functions here
        //case #: return (uint64_t)IO_New_Func(&_inst, &get_val);      break;
    }
    set_val = UINT64_MAX;
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
// Tests the i2c fault system to ensure its accurate
void demo_watch_i2c_funcs() {
    init_driver_watcher("I2C", 1, &drive_watcher); //!BREAK! Use this as breakpoint
    
    // Action Methods
    // [0] I2C INIT
    watch_inst_conf(I2C_INIT, I2C_STATUS_OK, true, true);
    watch_inst_conf(I2C_INIT, I2C_STATUS_ERROR_NULL_POINTER, true, false);
    watch_inst_conf(I2C_INIT, I2C_STATUS_ERROR_NULL_POINTER, false, true);
    watch_inst_conf(I2C_INIT, I2C_STATUS_ERROR_NULL_POINTER, false, false);
    
    // I/O Methods
    // [1] I2C SET CLOCK
    watch_inst_conf_set(I2C_SET_CLOCK, I2C_STATUS_OK, 100, true, true);
    watch_inst_conf_set(I2C_SET_CLOCK, I2C_STATUS_ERROR_NULL_POINTER, 100, false, false);

    // [2] I2C TRANSMIT
    watch_inst_conf(I2C_TRANSMIT, I2C_STATUS_OK, true, true);
    watch_inst_conf(I2C_TRANSMIT, I2C_STATUS_ERROR_NULL_POINTER, false, false);
    
    // [3] I2C RECEIVE
    watch_inst_conf(I2C_RECIEVE, I2C_STATUS_OK, true, true);
    watch_inst_conf(I2C_RECIEVE, I2C_STATUS_ERROR_NULL_POINTER, false, false);
    
    //!NOTE! Add new function to track here
    //// [#] ADC NEW FUNC
    //track_inst_conf(ADC_FUNC_ID, ADC_STATUS_ID, false, false);
    
    asm("NOP"); //!BREAK! Use this as breakpoint
}
#endif