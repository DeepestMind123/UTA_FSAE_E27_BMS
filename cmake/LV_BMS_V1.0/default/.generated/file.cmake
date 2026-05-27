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
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../inc/bms/devices/current sensor/dev_current.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../inc/bms/devices/ltc6813/ADI.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../inc/bms/devices/ltc6813/LTC6813.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../inc/bms/devices/ltc6813/dev_ltc6813.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../inc/bms/io/adc io/io_adc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../inc/bms/system/bms_manager.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../inc/bms/system/sys_fault.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../inc/bms/utils/util_time.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../main.c")
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
