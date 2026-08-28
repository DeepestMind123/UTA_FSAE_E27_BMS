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

# Structure
## Files
`<SYSTEM>` - This represents the *system name* as it is a field filled in per system.
</br>_STRUCTS_
```
- <SYSTEM>_ctx_t    - A struct containing specific module properties
- <SYSTEM>_cfg_t    - A struct containing the modifiable configuration data
- <SYSTEM>_time_t   - A struct containing timing of each system
- <SYSTEM>_func_t   - A struct containing dependency injected functions
  + These can be firmware functions or hal functions
```

## Paths
| Path                              | Purpose                                                                                                                               |
|-----------------------------------|---------------------------------------------------------------------------------------------------------------------------------------|
| _build                            | The [CMake build tree](https://cmake.org/cmake/help/latest/manual/cmake.1.html#introduction-to-cmake-buildsystems) (can be deleted)   |
| cmake                             | Generated [CMake](https://cmake.org/) files. May be deleted if user.cmake has not been added                                          |
| .vscode                           | See [VSCode](https://code.visualstudio.com/docs/getstarted/settings)                                                                  |
| out                               | Final build artifacts (can be deleted)                                                                                                |