/**
 * @file battery_config.h
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
    uint8_t temp_count;         // number of pack thermistors
    uint8_t balance_ic_count;   // number of active balancing ics (typically number of cells - 1)

    // limits
    uint16_t uv_lim_mV;         // under-voltage limit
    uint16_t ov_lim_mV;         // over-voltage limit
    uint16_t ot_lim_C;          // over-temperature limit
    uint32_t oc_lim_mA;         // over-charge current limit

    // capacity
    uint32_t capacity_mAh;      // nominal capacity in mAh
    uint16_t idle_current_mA;   // idle current of any onboard electronics (bms)

} battery_cfg_t;

typedef struct
{
    const battery_cfg_t *cfg;
} battery_t;

void battery_config_init(battery_t *p_inst, const battery_cfg_t *p_cfg);

#endif