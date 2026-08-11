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
#define NULL_COUNT 2
// Get func output to compare non-null vs. null
static int get_func(int p_func_id) {
    setIntArrayDefault(get_para_int, PARAMSSIZE, UINT8_MAX);
    io_spi_t *_inst             = not_null_bools[0] ? &inst : NULL;
    io_spi_cfg_t *_config       = not_null_bools[1] ? &config : NULL;
    
    switch(p_func_id) {
        case 0: return (int64_t)IO_SPI_Init(_inst, _config);
        case 1: return (int64_t)IO_SPI_Transfer_Word(_inst, (uint8_t)set_para_int[0], (uint8_t*)&get_para_int[0], (uint8_t*)&get_para_int[1]);
        case 2: return (int64_t)IO_SPI_Transfer_Sentence(_inst, (uint8_t)set_para_int[2], (uint8_t*)&get_para_int[0], (uint8_t*)&get_para_int[1], (uint8_t)set_para_int[1]);
        //!NOTE! Add new functions here
        //case #: return (int64_t)New_Func(&_inst, &get_para_num_1);
    }
    setBoolArrayDefault(not_null_bools, NULL_COUNT, false);
    setIntArrayDefault(set_para_int, PARAMSSIZE, UINT8_MAX);
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
    demo_watcher_init(NULL_COUNT);
    demo_watch_spi_funcs();
}
// Tests the spi fault system to ensure its accurate
void demo_watch_spi_funcs() {
    init_driver_watcher("SPI", 1, &drive_watcher); //!BREAK! Use this as breakpoint
    
    // Action Methods
    // [0] SPI INIT
    SET_NULL_FLAGS(true, true);
    watch_inst_conf(SPI_INIT, SPI_STATUS_OK);
    SET_NULL_FLAGS(true, true);
    watch_inst_conf(SPI_INIT, SPI_STATUS_ERROR_NULL_POINTER);
    SET_NULL_FLAGS(true, true);
    watch_inst_conf(SPI_INIT, SPI_STATUS_ERROR_NULL_POINTER);
    SET_NULL_FLAGS(true, true);
    watch_inst_conf(SPI_INIT, SPI_STATUS_ERROR_NULL_POINTER);
    
    // I/O Methods
    // [1] SPI TRANSFER WORD
    SET_NULL_FLAGS(true, true);
    watch_inst_conf(SPI_TRANSFER_WORD, SPI_STATUS_OK);
    SET_NULL_FLAGS(true, true);
    watch_inst_conf(SPI_TRANSFER_WORD, SPI_STATUS_ERROR_NULL_POINTER);

    // [2] SPI TRANSFER SENTENCE
    SET_NULL_FLAGS(true, true);
    watch_inst_conf(SPI_TRANSFER_SENTENCE, SPI_STATUS_OK);
    SET_NULL_FLAGS(true, true);
    watch_inst_conf(SPI_TRANSFER_SENTENCE, SPI_STATUS_ERROR_NULL_POINTER);
    
    //!NOTE! Add new function to track here
    //// [#] NEW FUNC
    //track_inst_conf(FUNC_ID, STATUS_ID, NULL, NULL, NULL);
    
    asm("NOP"); //!BREAK! Use this as breakpoint
}
#endif