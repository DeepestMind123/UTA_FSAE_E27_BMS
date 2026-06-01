#ifndef BMS_FAULT_H
#define BMS_FAULT_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "util_time.h"
#include "util_irq.h"

typedef enum
{
    BMS_FAULT_NONE = 0,
    BMS_FAULT_NULL_POINTER,
    BMS_FAULT_ADC_TIMEOUT,
    BMS_FAULT_CELL_OVERVOLT,
    BMS_FAULT_CELL_UNDERVOLT,
    BMS_FAULT_OVERCURRENT,
    BMS_FAULT_MODULE_OVERTEMP,
    BMS_FAULT_SPI_OFF,
    BMS_FAULT_CAN_OFF,
    BMS_FAULT_I2C_OFF,
    BMS_FAULT_NOT_INIT,
    BMS_FAULT_CURRENT_TIMEOUT,
    BMS_FAULT_VOLT_TIMEOUT,
    BMS_FAULT_TEMP_TIMEOUT,
    BMS_FAULT_TIMING_LAPSE,
    BMS_FAULT_MAX
} bms_fault_id_t;

typedef enum
{
    BMS_FAULT_STATUS_CLEAR = 0,
    BMS_FAULT_STATUS_PENDING,
    BMS_FAULT_STATUS_ACTIVE
} bms_fault_status_t;

typedef struct
{
    uint16_t counter;
    uint16_t threshold;
    uint32_t start_time;
    uint32_t max_time;

    bms_fault_status_t status;
} bms_fault_record_t;

static bms_fault_record_t bms_fault_table[BMS_FAULT_MAX];

void BMS_Fault_Update(bms_fault_id_t id, bool is_error);

bms_fault_status_t BMS_Fault_Get_Status(bms_fault_id_t id);

bool BMS_Is_Fault(void);

#endif