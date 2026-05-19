#ifndef SYS_FAULT_H
#define SYS_FAULT_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "sys_time.h"

typedef enum
{
    FAULT_NONE = 0,
    FAULT_NULL_POINTER,
    FAULT_ADC_TIMEOUT,
    FAULT_CELL_OVERVOLTAGE,
    FAULT_CELL_UNDERVOLTAGE,
    FAULT_OVERCURRENT,
    FAULT_MODULE_OVERTEMP,
    FAULT_SPI_OFF,
    FAULT_CAN_OFF,
    FAULT_I2C_OFF,
    FAULT_MAX
} fault_id_t;

typedef enum
{
    FAULT_STATUS_CLEAR = 0,
    FAULT_STATUS_PENDING,
    FAULT_STATUS_ACTIVE
} fault_status_t;

typedef struct
{
    uint16_t counter;
    uint16_t threshold;
    uint32_t start_time;
    uint32_t max_time;

    fault_status_t status;
} fault_record_t;

static fault_record_t fault_table[FAULT_MAX];

void sys_fault_update(fault_id_t id, bool is_error);

fault_status_t sys_fault_get_status(fault_id_t id);

#endif