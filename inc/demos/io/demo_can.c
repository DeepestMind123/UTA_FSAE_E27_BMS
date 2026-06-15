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
 #include "demo_tester.h"

#define DEMO_can
#ifdef DEMO_can
const char *can_func_names[] = {
    "CAN INIT",
    "CAN TRANSMIT",
    "CAN RECIEVE"
};
const char *can_status[] = {
    "OK",
    "NULL POINTER",
    "NOT INIT",
    "TRANSMIT FAIL",
    "RECIEVE FAIL",
    "MAX"
};
#endif

// System to log can data via breakpoints and watches
static char watch[100]; //watch the 'watch' variable to debug
void can_log(const char *p_func_name, const char *p_expected_status_str, int p_status,
        int64_t _val) {
#ifdef DEMO_can
    const char *_status_str;
    _status_str = can_status[p_status];
    snprintf(watch, sizeof(watch), "|Func:%s|Expected Status:%s|Return Status:%s|Data Read: %lld|",
        p_func_name, p_expected_status_str, _status_str, _val);
#endif
}

bool init(uint32_t baudrate)            { return 1; }
bool transmit(const can_msg_t *p_msg)   { return 1; }
bool receive(can_msg_t *p_msg)          { return 1; }
// Tests the can fault system to ensure its accurate
uint32_t demo_can_fault() {
#ifdef DEMO_can
    can_msg_t _can_msg = {
        .id = 1000,
        .dlc = 4,
        .data = {0,0,0,0,0,0,0,0},
        .is_ext = false
    };
    io_can_cfg_t _can_config = {
        .init = init,
        .transmit = transmit,
        .receive = receive
    };
    io_can_t _can_1;
    const io_can_cfg_t *can_config_NULL = NULL;
    io_can_t *_can_NULL = NULL;
    int64_t _out_val = 0;
    int16_t _set_val = 10;
    uint32_t _baudrate = 8000;
    
    // Action Methods
    int _test_id = 0;
    can_log(can_func_names[_test_id],
            can_status[0],
            (int)IO_CAN_Init(&_can_1, &_can_config, _baudrate),
            0);
    can_log(can_func_names[_test_id],
            can_status[1],
            (int)IO_CAN_Init(_can_NULL, &_can_config, _baudrate),
            0);
    
    _test_id++;
    can_log(can_func_names[_test_id],
            can_status[0],
            (int)IO_CAN_Transmit(&_can_1, &_can_msg),
            0);
    can_log(can_func_names[_test_id],
            can_status[1],
            (int)IO_CAN_Transmit(_can_NULL, &_can_msg),
            0);
    
    _test_id++;
    can_log(can_func_names[_test_id],
            can_status[0],
            (int)IO_CAN_Receive(&_can_1, &_can_msg),
            0);
    can_log(can_func_names[_test_id],
            can_status[1],
            (int)IO_CAN_Receive(_can_NULL, &_can_msg),
            0);
    
    // I/O Methods
    return 0;
#endif
}