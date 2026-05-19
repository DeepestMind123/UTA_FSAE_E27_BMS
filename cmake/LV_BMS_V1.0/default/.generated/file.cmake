# The following variables contains the files used by the different stages of the build process.
set(LV_BMS_V1_0_default_toolchain_XC8_3_10_FILE_TYPE_assemble)
set_source_files_properties(${LV_BMS_V1_0_default_toolchain_XC8_3_10_FILE_TYPE_assemble} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${LV_BMS_V1_0_default_toolchain_XC8_3_10_FILE_TYPE_assemble})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(LV_BMS_V1_0_default_toolchain_XC8_3_10_FILE_TYPE_assemblePreprocess)
set_source_files_properties(${LV_BMS_V1_0_default_toolchain_XC8_3_10_FILE_TYPE_assemblePreprocess} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${LV_BMS_V1_0_default_toolchain_XC8_3_10_FILE_TYPE_assemblePreprocess})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(LV_BMS_V1_0_default_toolchain_XC8_3_10_FILE_TYPE_compile
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../bms_drivers/ADI.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../bms_drivers/LTC6813.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../bms_drivers/LTC6813_manager.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../bms_drivers/current_manager.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../bms_drivers/sys_manager.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../inc/adc_manager.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../inc/battery_config.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../inc/sys_time.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../sys_fault.c")
set_source_files_properties(${LV_BMS_V1_0_default_toolchain_XC8_3_10_FILE_TYPE_compile} PROPERTIES LANGUAGE C)
set(LV_BMS_V1_0_default_toolchain_XC8_3_10_FILE_TYPE_link)
set(LV_BMS_V1_0_default_image_name "default.elf")
set(LV_BMS_V1_0_default_image_base_name "default")

# The output directory of the final image.
set(LV_BMS_V1_0_default_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/LV_BMS_V1.0")

# The full path to the final image.
set(LV_BMS_V1_0_default_full_path_to_image ${LV_BMS_V1_0_default_output_dir}/${LV_BMS_V1_0_default_image_name})

# Potential output file extensions
set(output_extensions
    .hex
    .hxl
    .mum
    .o
    .sdb
    .sym
    .cmf)
list(TRANSFORM output_extensions PREPEND "${LV_BMS_V1_0_default_output_dir}/${LV_BMS_V1_0_default_image_base_name}")
