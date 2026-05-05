/**
 * @file main.c
 * @author notwe
 * @date 2026-05-03
 * @brief battery configuration header
 */

#ifndef BATTERY_CONFIG_H
#define BATTERY_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    // physical configuration
    uint8_t cell_count;         // number of cells
    uint8_t balance_ic_count;   // number of balancing ics (typically number of cells - 1)
    uint8_t temp_count;         // number of pack thermistors

    // limits
    uint16_t uv_lim_mV;         // under-voltage limit
    uint16_t ov_lim_mV;         // over-voltage limit
    uint16_t ot_lim_C;          // over-temperature limit

    // capacity
    uint32_t capacity_mAh;      // nominal capacity in mAh
    uint16_t idle_current_mA;   // idle current of any onboard electronics (bms)

} battery_config_t;

void battery_config_init(void);

extern battery_config_t battery_config;

#endif