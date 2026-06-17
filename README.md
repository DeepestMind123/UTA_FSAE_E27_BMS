# LV_BMS_V0.2

## Summary
This repository contains features for a custom BMS (Battery Management System).
It supports:
- Passive cell balancing
  - <Insert Tools & Utilization>
- Active cell balancing
  - <Insert Tools & Utilization>

## Notes
You either need the MPLab software to autogenerate .vscode & cmake or MPLab extension in VSCode.

## Software

| Path                           | Purpose                  |
|--------------------------------|-------------------------------------------------------------------------------------------------------------------------------------|
| mplab-xc8 | [MPLAB® XC8 Compiler](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide) |
| mplab-ide | [MPLAB® XC8 IDE](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide) |
| cmake     | [CMake](https://cmake.org/) files. May be deleted if user.cmake has not been added |
| .vscode   | See [VSCode](https://code.visualstudio.com/docs/getstarted/settings) |

## Outputs

| Path                           | Purpose                  |
|--------------------------------|-------------------------------------------------------------------------------------------------------------------------------------|
| _build | The [CMake build tree](https://cmake.org/cmake/help/latest/manual/cmake.1.html#introduction-to-cmake-buildsystems), can be deleted. |
| .vscode\settings.json          | Workspace specific settings |
| .vscode\LV_BMS_V1.0.mplab.json | The MPLAB project file, should not be deleted |
| out                            | Final build artifacts |
# Objective
Battery Management Software intended for use by the UTA Racing Team.

# Requirements
BMS Software must comply with the following rules and standards:
    -FSAE 2026 Rules V1.0 defines the rules for the tractive battery system. Though most of these rules are not specific to the BMS, the design must both comply by these rules and enable physical rules compliance.
    -MISRA C: 2012 defines the best practice for embedded C programming and includes strict guidelines for functional safety.
    -ISO26262 is the international standard for electrical and electronic functional safety on automotive systems. This standard has substantial overlap with the MISRA C:2012 standard, but does define some additional requirements. As this is intended to be used on a EV Formula Car, the automotive standard is both necessary and best practice.

Aside from the required rules and standard compliance, this project also has the following requirements that fall outside the scope of the above:
    -Proper care must be taken to ensure that the BMS can be used on a variety of MCUs (must be system agnostic).
        -As such, if a module needs to use a specific peripheral (SPI, ADC, CAN, etc), a IO module must be made so that that module can function on any system.
    -When possible, each layer must be designed as "non-blocking" asynchronous state-machines.
    -Likewise, all IO, DEV, and non-UTIL or BMS function must return a status, and the system error handle must be able to see, log, and handle every status.
    -The BMS must use a Kalman Filter for SOC and cell Internal Series Resistance calculations.

The following formatting guidelines are required for all layers of the overall system.
    -Strict prefixes will be used to describe each .c/.h pair (or module) and its place in the system.
        -SYS or BMS describes a module that has logic specific to E27 Accumulator and is responsible for making decisions for the pack based on data provided by lower layers. These modules do not know how data is gathered, just what to do with it.
        -DEV describes a module that functions as a device driver. These modules can be for general devices (current sensors, fans, etc) or specific ICs (LTC6803, LTC6813). If a DEV module is for a specific IC, it is recommended to include the name of the device in the file name.
        -IO describes a module that functions as a wrapper for HAL functions provided for a specific MCU.
        -UTIL describes a module that provides a specific utility without requiring any HAL code (tick updates, math, interrupts, etc.)
    -Prefer the following:
        -p_var for pointer variables 
        -struct_t for types
        -lowercase_name for variables
        -Uppercase_Name() for functions
        -ALLCAPS for constants and macros

# Resources
The current version of the BMS hardware (E27 Module V0) requires the use of the Teensy 4.1 board. In order to access this hardware in Arduino IDE, paste the following link in "File">"Preferences">"Additional boards manager URLs":
https://www.pjrc.com/teensy/package_teensy_index.json

# External Libraries
FlexCAN_T4: CAN library for Teensy 4.1
