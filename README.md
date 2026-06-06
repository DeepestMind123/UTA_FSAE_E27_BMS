
# LV_BMS_V1.0

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
