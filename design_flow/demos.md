<span style="font-family: Helvetica, Montserrat, Arial, sans-serif; color: light grey">
<div style="text-align: center;">

# E27 Master Repository
</div>

<div style="text-align: center;">

## Navigation
</div>
<div style="text-align: center; color: grey; font-size:16;">

*| [General](General.md) |*
</div>
<div style="text-align: center; color: grey; font-size:16;">

*| [Demos](Demos.md) | [Setup](Setup.md) | [Structure](Structure.md) | [Syntax](Syntax.md) |*
</div>

# Demos
## Terms
dw - Demo Watcher
<br>Demo: Unit test file
<br>Watcher: Usage of strings to monitor states

System Struct: A struct from the original system
<br>Wrapper Struct: A struct encapsulating a system struct

## General Info
Demos are grouped into two categories:
<br>*| Hardware | Firmware |*

Hardware  -> Unit tests done through actual hardware to independantly test systems
<br>Firmware  &nbsp;-> Unit tests done through purely the firmware without any hardware involvement
### *`Hardware`*
Does not utilize `Demo Watcher` system. This system directly interacts with hardware devices to test and debug hardware devices. Systems in the devices folder will all be tested within these tests.

<br>Currently Not Implemented...
### *`Firmware`*
Firmware demos utilize the `Demo Watcher` system. This system utilizes wrappers to encapsulate each system with the hals being the exception. Firmware testing abstracts from the usage of hardware and purely focuses on testing logic. These tests primarily consist of checking conditionals, faults, and any syntax bugs. The system is made from scratch, but might be moved over to another firmware.
##### *Variables (Global)*
_OUTPUT_
```
- watch                 [String]   (Stores all the variable strings in a format)
- driver_name_watch     [String]   (Name of system being watched)
- test_id_watch         [String]   (ID of unit test function group)
- func_str_watch        [String]   (Function name)
- status_exp_watch      [String]   (Expected status from unit test)
- status_str_watch      [String]   (Returned status from unit test)
- state_str_watch       [String]   (The systems state after unit test)
```
_STRING_PARAMETERS_
```
- set_para_int_watch    [String]   (Stores the string of #set_para_int#)
- get_para_int_watch    [String]   (Stores the string of #get_para_int#)
- set_para_float_watch  [String]   (Stores the string of #set_para_float#)
- get_para_float_watch  [String]   (Stores the string of #get_para_float#)
```
_VALUE_PARAMETERS_
```
- get_state             [int32]    (If a system has states it stores the current state of that system)
- set_para_int          [int64*]   (Stores an array of integer set parameters to pass into the function)
- get_para_int          [int64*]   (Stores an array of integer get parameters to pass into the function)
- set_para_float        [float*]   (Stores an array of float set parameters to pass into the function)
- get_para_float        [float*]   (Stores an array of float get parameters to pass into the function)
- not_null_bool         [bool*]    (Stores an array of booleans with each being tied to making a System Struct null)
```
_TEST_DATA_
```
- driver_name           [String]   (Stores the name of the driver)
- test_id               [int32]    (Stores the id of the current unit test function group)
- active_driver         [struct]   (Stores the currently tested system in a struct)
```
