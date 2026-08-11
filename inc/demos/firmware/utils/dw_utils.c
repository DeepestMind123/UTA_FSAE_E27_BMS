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

static void Disable(void)    {};
static void Enable(void)                {};
static uint32_t Get_State(void)         { return 0; }
static void Set_State(uint32_t state)   {};
// Initialization
static util_irq_cfg_t util_irq_cfg = {
    .Disable = Disable,
    .Enable = Enable,
    .Get_State = Get_State,
    .Set_State = Set_State
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
#define NULL_COUNT 5
// Get func output to compare non-null vs. null
static int get_func(int p_func_id) {
    setIntArrayDefault(get_para_int, PARAMSSIZE, UINT8_MAX);
    setFloatArrayDefault(get_para_float, PARAMSSIZE, FLT_MAX);

    util_irq_t *_inst           = not_null_bools[0] ? &util_irq_inst : NULL;
    util_irq_cfg_t *_config     = not_null_bools[1] ? &util_irq_cfg : NULL;
    util_time_t *_time          = not_null_bools[2] ? &util_time_inst : NULL;
    pid_ctrl_cfg_t *_cfg        = not_null_bools[3] ? &pid_ctrl_cfg : NULL;
    pid_ctrl_t *_pid_ctrl       = not_null_bools[4] ? &pid_ctrl_inst : NULL;
    
    switch(p_func_id) {
        case UTIL_IRQ_Init_ID: return (int64_t)UTIL_IRQ_Init(_inst, _config);
        case UTIL_IRQ_Enter_Critical_ID: return (int64_t)UTIL_IRQ_Enter_Critical(_inst, (uint32_t*)&get_para_int[0]);
        case UTIL_IRQ_Exit_Critical_ID: return (int64_t)UTIL_IRQ_Exit_Critical(_inst, (uint32_t)set_para_int[0]);
        case UTIL_Time_Init_ID: return (int64_t)UTIL_Time_Init(_time, _inst);
        case UTIL_Time_Tick_Up_ID: return (int64_t)UTIL_Time_Tick_Up(_time);
        case UTIL_Time_Get_Tick_ID: return (int64_t)UTIL_Time_Get_Tick(_time, (uint32_t*)&get_para_int[0]);
        case UTIL_PID_Ctrl_Init_ID: return (int64_t)UTIL_PID_Ctrl_Init(_pid_ctrl, _cfg, _time);
        case UTIL_PID_Ctrl_Task_ID: return (int64_t)UTIL_PID_Ctrl_Task(_pid_ctrl, (float)set_para_float[0], (float*)&get_para_float[0]);
        case UTIL_PID_Ctrl_Get_Val_ID: return (int64_t)UTIL_PID_Ctrl_Get_Val(_pid_ctrl, (float*)&get_para_float[0]);
        //!NOTE! Add new functions here
        //case #: return (int64_t)New_Func(&_inst, &get_para_num[0]);
    }
    setBoolArrayDefault(not_null_bools, NULL_COUNT, false);
    setIntArrayDefault(set_para_int, PARAMSSIZE, UINT8_MAX);
    setFloatArrayDefault(set_para_float, PARAMSSIZE, FLT_MAX);
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
    demo_watcher_init(NULL_COUNT);
    demo_watch_utils_funcs();
    demo_watch_utils_init();
}

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
    SET_NULL_FLAGS(true, true, true, true, true);
    watch_inst_conf(UTIL_IRQ_Init_ID, IRQ_STATUS_OK);
    SET_NULL_FLAGS(false, false, false, false, false);
    watch_inst_conf(UTIL_IRQ_Init_ID, IRQ_STATUS_ERROR_NULL_POINTER);

    // [1] UTILS IRQ ENTER CRITICAL
    SET_NULL_FLAGS(true, true, true, true, true);
    watch_inst_conf(UTIL_IRQ_Enter_Critical_ID, IRQ_STATUS_OK);
    SET_NULL_FLAGS(false, false, false, false, false);
    watch_inst_conf(UTIL_IRQ_Enter_Critical_ID, IRQ_STATUS_ERROR_NULL_POINTER);
    
    set_para_int[0] = 111;
    // [2] UTILS IRQ EXIT CRITICAL
    SET_NULL_FLAGS(true, true, true, true, true);
    watch_inst_conf(UTIL_IRQ_Exit_Critical_ID, IRQ_STATUS_OK);
    SET_NULL_FLAGS(false, false, false, false, false);
    watch_inst_conf(UTIL_IRQ_Exit_Critical_ID, IRQ_STATUS_ERROR_NULL_POINTER);
    
    asm("NOP"); //!BREAK! Use this as breakpoint
}

void demo_watch_utils_time_funcs() {
    init_driver_watcher("UTILS", 2, &drive_watcher);
    // UTIL TIME
    // [3] UTILS TIME INIT
    SET_NULL_FLAGS(true, true, true, true, true);
    watch_inst_conf(UTIL_Time_Init_ID, TIME_STATUS_OK);
    SET_NULL_FLAGS(false, false, false, false, false);
    watch_inst_conf(UTIL_Time_Init_ID, TIME_STATUS_ERROR_NULL_POINTER);

    // [4] UTILS TIME TICK UP
    SET_NULL_FLAGS(true, true, true, true, true);
    watch_inst_conf(UTIL_Time_Tick_Up_ID, TIME_STATUS_OK);
    SET_NULL_FLAGS(false, false, false, false, false);
    watch_inst_conf(UTIL_Time_Tick_Up_ID, TIME_STATUS_ERROR_NULL_POINTER);
    
    // [5] UTILS TIME GET TICK
    SET_NULL_FLAGS(true, true, true, true, true);
    watch_inst_conf(UTIL_Time_Get_Tick_ID, TIME_STATUS_OK);
    SET_NULL_FLAGS(false, false, false, false, false);
    watch_inst_conf(UTIL_Time_Get_Tick_ID, TIME_STATUS_ERROR_NULL_POINTER);
    
    asm("NOP"); //!BREAK! Use this as breakpoint
}

void demo_watch_utils_pid_ctrl_funcs() {
    init_driver_watcher("UTILS", 3, &drive_watcher);
    // UTIL PID CTRL
    // [6] UTILS PID CTRL INIT
    SET_NULL_FLAGS(true, true, true, true, true);
    watch_inst_conf(UTIL_PID_Ctrl_Init_ID, PID_STATUS_OK);
    SET_NULL_FLAGS(false, false, false, false, false);
    watch_inst_conf(UTIL_PID_Ctrl_Init_ID, PID_STATUS_ERROR_NULL_POINTER);

    set_para_float[0] = 11.11;
    // [7] UTILS PID CTRL TASK
    SET_NULL_FLAGS(true, true, true, true, true);
    watch_inst_conf(UTIL_PID_Ctrl_Task_ID, PID_STATUS_OK);
    SET_NULL_FLAGS(false, false, false, false, false);
    watch_inst_conf(UTIL_PID_Ctrl_Task_ID, PID_STATUS_ERROR_NULL_POINTER);
    
    // [8] UTILS PID CTRL GET VAL
    SET_NULL_FLAGS(true, true, true, true, true);
    watch_inst_conf(UTIL_PID_Ctrl_Get_Val_ID, PID_STATUS_OK);
    SET_NULL_FLAGS(false, false, false, false, false);
    watch_inst_conf(UTIL_PID_Ctrl_Get_Val_ID, PID_STATUS_ERROR_NULL_POINTER);
    
    asm("NOP"); //!BREAK! Use this as breakpoint
}

// Demo to test when no init called
void demo_watch_utils_init() {
    init_driver_watcher("UTILS", 4, &drive_watcher); //!BREAK! Use this as breakpoint
    
    // [7] UTILS TASK
    memcpy(not_null_bools, (bool[]){ true, true, true, true, true }, sizeof(bool) * NULL_COUNT);
    watch_inst_conf(UTIL_PID_Ctrl_Task_ID, PID_STATUS_ERROR_NULL_POINTER);    // State: NULL_POINTER
    
    // [6] UTILS INIT
    memcpy(not_null_bools, (bool[]){ true, true, true, true, true }, sizeof(bool) * NULL_COUNT);
    watch_inst_conf(UTIL_PID_Ctrl_Init_ID, PID_STATUS_OK);
    memcpy(not_null_bools, (bool[]){ false, false, false, false, false }, sizeof(bool) * NULL_COUNT);
    watch_inst_conf(UTIL_PID_Ctrl_Init_ID, PID_STATUS_ERROR_NULL_POINTER);

    // [7] UTILS TASK
    memcpy(not_null_bools, (bool[]){ true, true, true, true, true }, sizeof(bool) * NULL_COUNT);
    watch_inst_conf(UTIL_PID_Ctrl_Task_ID, PID_STATUS_ERROR_NULL_POINTER);  // State: NULL_POINTER
}
#endif