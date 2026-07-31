/**
 * @file teensy_hal_adc.hpp
 * @author notwe
 * @date 2026-06-18
 * @brief teensy 4.1 adc hal wrapper basics
 */

#ifndef TEENSY_HAL_ADC_HPP
#define TEENSY_HAL_ADC_HPP

#include <Arduino.h>
#include <ADC.h>

class TEENSY_ADC
{
    private:
    inline static ADC *adc = nullptr;
    inline static uint8_t target_pin;

    public:
    TEENSY_ADC() = delete;

    static void Init(uint8_t pin)
    {
        target_pin = pin;

        if(!adc)
        {
            adc = new ADC();

            pinMode(target_pin, INPUT_DISABLE);
            adc->adc0->setAveraging(4); 
            adc->adc0->setResolution(12); 
            adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::MED_SPEED);
            adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::MED_SPEED);
        }
    }

    static void Start_Read()
    {
        adc->adc0->startSingleRead(target_pin);
    }

    static void Stop_Read()
    {
        // intentionally left blank
    }

    static bool Check_Read_State()
    {
        return adc->adc0->isComplete();
    }

    static uint16_t Get_Read()
    {
        return (uint16_t)adc->adc0->readSingle();
    }

    static void Channel_Select()
    {
        // Intentionally left blank
    }
};

#endif

