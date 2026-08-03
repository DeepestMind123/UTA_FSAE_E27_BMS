/**
 * @file demo_spi.c
 * @author MITurley
 * @date 2026-06-03
 * @brief demo to test spi <SF>
 */
 
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
 
 #include "io_spi.h"
#include "dw_manager.h"

#ifdef DEMO_SPI
// Initialization
void spi_cs_high(uint8_t cs_pin)          {};
void spi_cs_low(uint8_t cs_pin)           {};
uint8_t spi_byte_exchange(uint8_t data)   {};
static io_spi_cfg_t config = {
    .spi_cs_high        = spi_cs_high,
    .spi_cs_low         = spi_cs_low,
    .spi_byte_exchange  = spi_byte_exchange,
};
static io_spi_t inst;

// Watch strings
static const char *func_str[] = {
    "SPI INIT",
    "SPI TRANSFER WORD",
    "SPI TRANSFER SENTENCE"
    //!NOTE! Add func name here
    //"NEW FUNC NAME"
};
static const char *status_str[] = {
    "OK",
    "NULL POINTER",
    "NOT INIT"
    //!NOTE! Add status here
    //"NEW STATUS"
};
// Helpful For ID Matching
typedef enum {
    SPI_INIT = 0,
    SPI_TRANSFER_WORD,
    SPI_TRANSFER_SENTENCE
    //!NOTE! Add func id here
    //"NEW FUNC ID"
} func_id;
// Get func output to compare non-null vs. null
static int get_func(int p_func_id, bool* p_not_inst_null) {
    setIntArrayDefault(get_para_num, 1, UINT8_MAX);
    setBoolArrayDefault(not_null_bools, 1, false);
    io_spi_t *_inst             = p_not_inst_null[0] ? &inst : NULL;
    io_spi_cfg_t *_config       = p_not_inst_null[1] ? &config : NULL;
    
    switch(p_func_id) {
        case 0: return (int64_t)IO_SPI_Init(_inst, _config);                                                                                           break;
        case 1: return (int64_t)IO_SPI_Transfer_Word(_inst, (uint8_t)set_para_num[0], (uint8_t*)&get_para_num[0], (uint8_t*)&get_para_num[1]);                             break;
        case 2: return (int64_t)IO_SPI_Transfer_Sentence(_inst, (uint8_t)set_para_num[2], (uint8_t*)&get_para_num[0], (uint8_t*)&get_para_num[1], (uint8_t)set_para_num[1]);     break;
        //!NOTE! Add new functions here
        //case #: return (int64_t)New_Func(&_inst, &get_para_num_1);                                                                                            break;
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

// Tests all spi methods
void demo_watch_spi_all() {
    demo_watch_spi_funcs();
}
#define NULL_COUNT 2
// Tests the spi fault system to ensure its accurate
void demo_watch_spi_funcs() {
    init_driver_watcher("SPI", 1, &drive_watcher); //!BREAK! Use this as breakpoint
    
    // Action Methods
    // [0] SPI INIT
    watch_inst_conf(SPI_INIT, SPI_STATUS_OK,
        (bool[NULL_COUNT]) {true, true, true});
    watch_inst_conf(SPI_INIT, SPI_STATUS_ERROR_NULL_POINTER,
        (bool[NULL_COUNT]) {true, false, true});
    watch_inst_conf(SPI_INIT, SPI_STATUS_ERROR_NULL_POINTER,
        (bool[NULL_COUNT]) {false, true, true});
    watch_inst_conf(SPI_INIT, SPI_STATUS_ERROR_NULL_POINTER,
        (bool[NULL_COUNT]) {false, false, true});
    
    // I/O Methods
    // [1] SPI TRANSFER WORD
    watch_inst_conf(SPI_TRANSFER_WORD, SPI_STATUS_OK,
        (bool[NULL_COUNT]) {true, true, true});
    watch_inst_conf(SPI_TRANSFER_WORD, SPI_STATUS_ERROR_NULL_POINTER,
        (bool[NULL_COUNT]) {false, false, true});

    // [2] SPI TRANSFER SENTENCE
    watch_inst_conf(SPI_TRANSFER_SENTENCE, SPI_STATUS_OK,
        (bool[NULL_COUNT]) {true, true, true});
    watch_inst_conf(SPI_TRANSFER_SENTENCE, SPI_STATUS_ERROR_NULL_POINTER,
        (bool[NULL_COUNT]) {false, false, true});
    
    //!NOTE! Add new function to track here
    //// [#] NEW FUNC
    //track_inst_conf(FUNC_ID, STATUS_ID, NULL, NULL, NULL);
    
    asm("NOP"); //!BREAK! Use this as breakpoint
}
#endif