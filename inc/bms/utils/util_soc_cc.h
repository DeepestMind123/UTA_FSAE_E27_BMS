/**
 * @file util_soc_cc.h
 * @author notwe
 * @date 2026-05-03
 * @brief utility header for coulomb counting counting-state-of-charge calcuations
 */

 #ifndef UTIL_SOC_CC_H
 #define UTIL_SOC_CC_H

 #include <stdint.h>

 #define HR_TO_S (3600.0f)
 #define MS_TO_S (1000.0f)

 typedef float float_t;

 static inline float_t UTIL_SOC_Coulomb_Count(int16_t current_mA, uint32_t dt_ms, float_t last_soc, uint16_t max_capacity_mAh)
 {
    float_t new_soc = last_soc;

    if(max_capacity_mAh > 0)
    {
        float_t current_draw = (float_t)current_mA * (float_t)dt_ms / MS_TO_S;

        float_t max_capacity_mAs = max_capacity_mAh * HR_TO_S;

        float_t soc_delta = current_draw / max_capacity_mAs;

        new_soc = last_soc - soc_delta;

        if(new_soc > 100.0f)
        {
            new_soc = 100.0f;
        }

        if(new_soc < 0.0f)
        {
            new_soc = 0.0f;
        }
    }

    return new_soc;
 }

 #endif