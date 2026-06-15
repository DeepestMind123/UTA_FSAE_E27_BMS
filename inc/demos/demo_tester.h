/**
 * @file demo_tester.h
 * @author MITurley
 * @date 2026-06-03
 * @brief demo tester <HF>
 */

#ifndef DEMO_TESTER_H
#define DEMO_TESTER_H

#include <stdint.h>

static char watch[100]; //watch the 'watch' variable to debug

uint32_t demo_tester();

uint32_t demo_adc_fault();
uint32_t demo_adc_get(uint32_t p_cycles);


uint32_t demo_can_fault();


uint32_t demo_i2c_fault();


uint32_t demo_pwm_fault();


uint32_t demo_spi_fault();

#endif