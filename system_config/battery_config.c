/**
 * @file main.c
 * @author notwe
 * @date 2026-05-03
 * @brief battery configuration source
 */

 #include "battery_config.h"

void battery_config_init(void)
{
    battery_config_t cfg = {
        .cell_count = 4,
        .balance_ic_count = 0,
        .temp_count = 2,
        .uv_lim_mV = 2500,
        .ov_lim_mV = 4200,
        .ot_lim_C = 80,
        .capacity_mAh = 6000,
        .idle_current_mA = 14
    };

    // comment out this line if no active-balancing ics are being used
    cfg.balance_ic_count = cfg.cell_count - 1; 
}








 