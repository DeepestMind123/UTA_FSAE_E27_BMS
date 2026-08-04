/**
 * @file dw_utils_time.c
 * @author MITurley
 * @date 2026-07-30
 * @brief demo to test dw_utils_time <SF>
 */
 
#include <stddef.h>
#include <stdbool.h>

#include "util_pid_ctrl.h"
#include "dw_manager.h"

#ifdef DEMO_UTILS

// Initialization
static util_irq_cfg_t util_irq_cfg = {
    .Disable = NULL,
    .Enable = NULL,
    .Get_State = NULL,
    .Set_State = NULL
};
static util_irq_t util_irq_inst = {
    .is_init = false,
    .cfg = &util_irq_cfg
};
static util_time_t util_time_inst = {
    .is_init = false,
    .tick = 0,
    .irq = &util_irq_inst
};
static pid_ctrl_cfg_t pid_ctrl_cfg = {
    .setpoint = 0.0f
};
static pid_ctrl_t pid_ctrl_inst = {
    .now_val = 0.0f,
    .duty_value = 0,
    .now_time_ms = 0,
    .last_time_ms = 0,
    .error = 0.0f,
    .is_init = false,
    .status = 0,
    .time_inst = &util_time_inst,
    .cfg = &pid_ctrl_cfg
};

// Watch strings
static const char *func_str[] = {
    "UTIL IRQ Init",
    "UTIL IRQ Enter Critical",
    "UTIL IRQ Exit Critical",

    "UTIL Time Init",
    "UTIL Time Tick Up",
    "UTIL Time Get Tick",

    "UTIL PID CTRL Init",
    "UTIL PID CTRL Task",
    "UTIL PID CTRL GET VAL",
    //!NOTE! Add func name here
    //"NEW FUNC NAME"
};
static const char *status_str[] = {
    "IRQ_STATUS_OK",
    "IRQ_STATUS_ERROR_NOT_INIT",
    "IRQ_STATUS_ERROR_NULL_POINTER",
    "TIME_STATUS_OK",
    
    "TIME_STATUS_ERROR_NOT_INIT",
    "TIME_STATUS_ERROR_NULL_POINTER",
    "TIME_STATUS_ERROR_IRQ_ERROR",
    
    "PID_STATUS_OK",
    "PID_STATUS_ERROR_NOT_INIT",
    "PID_STATUS_ERROR_NULL_POINTER",
    "PID_STATUS_ERROR_TIME_ERROR",
    "PID_STATUS_MAX",
    //!NOTE! Add status here
    //"NEW STATUS"
};

// Helpful For ID Matching
typedef enum {
    UTIL_IRQ_Init_ID = 0,
    UTIL_IRQ_Enter_Critical_ID,
    UTIL_IRQ_Exit_Critical_ID,

    UTIL_Time_Init_ID,
    UTIL_Time_Tick_Up_ID,
    UTIL_Time_Get_Tick_ID,

    UTIL_PID_Ctrl_Init_ID,
    UTIL_PID_Ctrl_Task_ID,
    UTIL_PID_Ctrl_Get_Val_ID,
    //!NOTE! Add func id here
    //"NEW FUNC ID"
} func_id;
// Get func output to compare non-null vs. null
static int get_func(int p_func_id, bool* p_not_inst_null) {
    setIntArrayDefault(get_para_num, 1, UINT8_MAX);
    setBoolArrayDefault(not_null_bools, 1, false);

    util_irq_t *_inst           = p_not_inst_null[0] ? &util_irq_inst : NULL;
    util_irq_cfg_t *_config     = p_not_inst_null[1] ? &util_irq_cfg : NULL;
    util_time_t *_time          = p_not_inst_null[2] ? &util_time_inst : NULL;
    pid_ctrl_cfg_t *_cfg        = p_not_inst_null[3] ? &pid_ctrl_cfg : NULL;
    pid_ctrl_t *_pid_ctrl       = p_not_inst_null[4] ? &pid_ctrl_inst : NULL;
    
    switch(p_func_id) {
        case UTIL_IRQ_Init_ID: return (int64_t)UTIL_IRQ_Init(_inst, _config);
        case UTIL_IRQ_Enter_Critical_ID: return (int64_t)UTIL_IRQ_Enter_Critical(_inst, (uint32_t*)&get_para_num[0]);
        case UTIL_IRQ_Exit_Critical_ID: return (int64_t)UTIL_IRQ_Exit_Critical(_inst, (uint32_t)set_para_num[0]);
        case UTIL_Time_Init_ID: return (int64_t)UTIL_Time_Init(_time, _inst);
        case UTIL_Time_Tick_Up_ID: return (int64_t)UTIL_Time_Tick_Up(_time);
        case UTIL_Time_Get_Tick_ID: return (int64_t)UTIL_Time_Get_Tick(_time, (uint32_t*)&get_para_num[0]);
        case UTIL_PID_Ctrl_Init_ID: return (int64_t)UTIL_PID_Ctrl_Init(_pid_ctrl, _cfg, _time);
        case UTIL_PID_Ctrl_Task_ID: return (int64_t)UTIL_PID_Ctrl_Task(_pid_ctrl, (float)set_para_num[0], (float*)&get_para_num[0]);
        case UTIL_PID_Ctrl_Get_Val_ID: return (int64_t)UTIL_PID_Ctrl_Get_Val(_pid_ctrl, (float*)&get_para_num[0]);
        //!NOTE! Add new functions here
        //case #: return (int64_t)New_Func(&_inst, &get_para_num[0]);
    }
    setIntArrayDefault(set_para_num, 1, UINT8_MAX);
}
static driver_watcher_interface_t drive_watcher = {
    .func_strings       = func_str,
    .state_strings      = NULL,
    .status_strings     = status_str,
    .execute_func       = get_func,
    .get_state_func_id  = -1
};

// Tests all adc methods
void demo_watch_utils_all() {
    demo_watch_utils_funcs();
}

#define NULL_COUNT 5
// General utils test method that goes through every function
void demo_watch_utils_funcs() {
    init_driver_watcher("UTILS", 0, &drive_watcher);

    demo_watch_utils_irq_funcs();
    demo_watch_utils_time_funcs();
    demo_watch_utils_pid_ctrl_funcs();

    //!NOTE! Add new function to track here
    //// [#] NEW FUNC
    //track_inst_conf(FUNC_ID, STATUS_ID, NULL, NULL, NULL);
    
    asm("NOP"); //!BREAK! Use this as breakpoint
}

void demo_watch_utils_irq_funcs() {
    init_driver_watcher("UTILS", 1, &drive_watcher);
    // UTIL IRQ
    // [0] UTILS IRQ INIT
    watch_inst_conf(UTIL_IRQ_Init_ID, IRQ_STATUS_OK, 
        (bool[NULL_COUNT]) {true, true, true, true, true});
    watch_inst_conf(UTIL_IRQ_Init_ID, IRQ_STATUS_ERROR_NULL_POINTER, 
        (bool[NULL_COUNT]) {false, false, false, false, false});

    // [1] UTILS IRQ ENTER CRITICAL
    watch_inst_conf(UTIL_IRQ_Enter_Critical_ID, IRQ_STATUS_OK, 
        (bool[NULL_COUNT]) {true, true, true, true, true});
    watch_inst_conf(UTIL_IRQ_Enter_Critical_ID, IRQ_STATUS_ERROR_NULL_POINTER, 
        (bool[NULL_COUNT]) {false, false, false, false, false});
    
    // [2] UTILS IRQ EXIT CRITICAL
    watch_inst_conf(UTIL_IRQ_Exit_Critical_ID, IRQ_STATUS_OK, 
        (bool[NULL_COUNT]) {true, true, true, true, true});
    watch_inst_conf(UTIL_IRQ_Exit_Critical_ID, IRQ_STATUS_ERROR_NULL_POINTER, 
        (bool[NULL_COUNT]) {false, false, false, false, false});
    
    asm("NOP"); //!BREAK! Use this as breakpoint
}

void demo_watch_utils_time_funcs() {
    init_driver_watcher("UTILS", 2, &drive_watcher);
    // UTIL TIME
    // [3] UTILS TIME INIT
    watch_inst_conf(UTIL_Time_Init_ID, TIME_STATUS_OK, 
        (bool[NULL_COUNT]) {true, true, true, true, true});
    watch_inst_conf(UTIL_Time_Init_ID, TIME_STATUS_ERROR_NULL_POINTER, 
        (bool[NULL_COUNT]) {false, false, false, false, false});

    // [4] UTILS TIME TICK UP
    watch_inst_conf(UTIL_Time_Tick_Up_ID, TIME_STATUS_OK, 
        (bool[NULL_COUNT]) {true, true, true, true, true});
    watch_inst_conf(UTIL_Time_Tick_Up_ID, TIME_STATUS_ERROR_NULL_POINTER, 
        (bool[NULL_COUNT]) {false, false, false, false, false});
    
    // [5] UTILS TIME GET TICK
    watch_inst_conf(UTIL_Time_Get_Tick_ID, TIME_STATUS_OK, 
        (bool[NULL_COUNT]) {true, true, true, true, true});
    watch_inst_conf(UTIL_Time_Get_Tick_ID, TIME_STATUS_ERROR_NULL_POINTER, 
        (bool[NULL_COUNT]) {false, false, false, false, false});
    
    asm("NOP"); //!BREAK! Use this as breakpoint
}

void demo_watch_utils_pid_ctrl_funcs() {
    init_driver_watcher("UTILS", 3, &drive_watcher);
    // UTIL PID CTRL
    // [6] UTILS PID CTRL INIT
    watch_inst_conf(UTIL_PID_Ctrl_Init_ID, PID_STATUS_OK, 
        (bool[NULL_COUNT]) {true, true, true, true, true});
    watch_inst_conf(UTIL_PID_Ctrl_Init_ID, PID_STATUS_ERROR_NULL_POINTER, 
        (bool[NULL_COUNT]) {false, false, false, false, false});
        
    // [7] UTILS PID CTRL TASK
    watch_inst_conf(UTIL_PID_Ctrl_Task_ID, PID_STATUS_OK, 
        (bool[NULL_COUNT]) {true, true, true, true, true});
    watch_inst_conf(UTIL_PID_Ctrl_Task_ID, PID_STATUS_ERROR_NULL_POINTER, 
        (bool[NULL_COUNT]) {false, false, false, false, false});
    
    // [8] UTILS PID CTRL GET VAL
    watch_inst_conf(UTIL_PID_Ctrl_Get_Val_ID, PID_STATUS_OK, 
        (bool[NULL_COUNT]) {true, true, true, true, true});
    watch_inst_conf(UTIL_PID_Ctrl_Get_Val_ID, PID_STATUS_ERROR_NULL_POINTER, 
        (bool[NULL_COUNT]) {false, false, false, false, false});
    
    asm("NOP"); //!BREAK! Use this as breakpoint
}

// Demo to test when no init called
void demo_watch_utils_init() {
    init_driver_watcher("UTILS", 4, &drive_watcher); //!BREAK! Use this as breakpoint
    
    // [7] UTILS TASK
    watch_inst_conf(UTIL_PID_Ctrl_Task_ID, PID_STATUS_ERROR_NULL_POINTER,
        (bool[NULL_COUNT]) {true, true, true, true, true});    // State: NULL_POINTER
    
    // [6] UTILS INIT
    watch_inst_conf(UTIL_PID_Ctrl_Init_ID, PID_STATUS_OK,
        (bool[NULL_COUNT]) {true, true, true, true, true});    // State: NULL_POINTER
    
    // [7] UTILS TASK
    watch_inst_conf(UTIL_PID_Ctrl_Task_ID, PID_STATUS_ERROR_NULL_POINTER,
        (bool[NULL_COUNT]) {false, false, false, false, true});  // State: NULL_POINTER
}
 
// Demo to test different task states
void demo_watch_utils_task() {
    init_driver_watcher("UTILS", 5, &drive_watcher); //!BREAK! Use this as breakpoint
    
    // [0] UTILS IRQ INIT
    watch_inst_conf(UTIL_IRQ_Init_ID, IRQ_STATUS_OK, 
        (bool[NULL_COUNT]) {true, true, true, true, true});
    watch_inst_conf(UTIL_IRQ_Init_ID, IRQ_STATUS_ERROR_NULL_POINTER, 
        (bool[NULL_COUNT]) {false, false, false, false, false});
    // [3] UTILS TIME INIT
    watch_inst_conf(UTIL_Time_Init_ID, TIME_STATUS_OK, 
        (bool[NULL_COUNT]) {true, true, true, true, true});
    watch_inst_conf(UTIL_Time_Init_ID, TIME_STATUS_ERROR_NULL_POINTER, 
        (bool[NULL_COUNT]) {false, false, false, false, false});
    // [7] UTILS TASK
    watch_inst_conf(UTIL_PID_Ctrl_Task_ID, PID_STATUS_ERROR_NOT_INIT,
        (bool[NULL_COUNT]) {true, true, true, true, true});

    // [6] UTILS INIT
    watch_inst_conf(UTIL_PID_Ctrl_Task_ID, PID_STATUS_OK,
        (bool[NULL_COUNT]) {true, true, true, true, true});
    // [7] UTILS TASK
    watch_inst_conf(UTIL_PID_Ctrl_Task_ID, PID_STATUS_ERROR_NOT_INIT,
        (bool[NULL_COUNT]) {true, true, true, true, true});
    
    // [6] UTILS INIT
    watch_inst_conf(UTIL_PID_Ctrl_Task_ID, PID_STATUS_OK,
        (bool[NULL_COUNT]) {true, true, true, true, true});
    // [7] UTILS TASK
    watch_inst_conf(UTIL_PID_Ctrl_Task_ID, PID_STATUS_OK,
        (bool[NULL_COUNT]) {true, true, true, true, true});
    watch_inst_conf(UTIL_PID_Ctrl_Task_ID, PID_STATUS_OK,
        (bool[NULL_COUNT]) {true, true, true, true, true});
    watch_inst_conf(UTIL_PID_Ctrl_Task_ID, PID_STATUS_OK,
        (bool[NULL_COUNT]) {true, true, true, true, true});
    watch_inst_conf(UTIL_PID_Ctrl_Task_ID, PID_STATUS_OK,
        (bool[NULL_COUNT]) {true, true, true, true, true});
    watch_inst_conf(UTIL_PID_Ctrl_Task_ID, PID_STATUS_OK,
        (bool[NULL_COUNT]) {true, true, true, true, true});
}
#endif