include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(LV_BMS_V1_0_default_library_list )

# Handle files with suffix (s|as|asm|AS|ASM|As|aS|Asm), for group toolchain XC8 3.10
if(LV_BMS_V1_0_default_toolchain_XC8_3_10_FILE_TYPE_assemble)
add_library(LV_BMS_V1_0_default_toolchain_XC8_3_10_assemble OBJECT ${LV_BMS_V1_0_default_toolchain_XC8_3_10_FILE_TYPE_assemble})
    LV_BMS_V1_0_default_toolchain_XC8_3_10_assemble_rule(LV_BMS_V1_0_default_toolchain_XC8_3_10_assemble)
    list(APPEND LV_BMS_V1_0_default_library_list "$<TARGET_OBJECTS:LV_BMS_V1_0_default_toolchain_XC8_3_10_assemble>")

endif()

# Handle files with suffix S, for group toolchain XC8 3.10
if(LV_BMS_V1_0_default_toolchain_XC8_3_10_FILE_TYPE_assemblePreprocess)
add_library(LV_BMS_V1_0_default_toolchain_XC8_3_10_assemblePreprocess OBJECT ${LV_BMS_V1_0_default_toolchain_XC8_3_10_FILE_TYPE_assemblePreprocess})
    LV_BMS_V1_0_default_toolchain_XC8_3_10_assemblePreprocess_rule(LV_BMS_V1_0_default_toolchain_XC8_3_10_assemblePreprocess)
    list(APPEND LV_BMS_V1_0_default_library_list "$<TARGET_OBJECTS:LV_BMS_V1_0_default_toolchain_XC8_3_10_assemblePreprocess>")

endif()

# Handle files with suffix [cC], for group toolchain XC8 3.10
if(LV_BMS_V1_0_default_toolchain_XC8_3_10_FILE_TYPE_compile)
add_library(LV_BMS_V1_0_default_toolchain_XC8_3_10_compile OBJECT ${LV_BMS_V1_0_default_toolchain_XC8_3_10_FILE_TYPE_compile})
    LV_BMS_V1_0_default_toolchain_XC8_3_10_compile_rule(LV_BMS_V1_0_default_toolchain_XC8_3_10_compile)
    list(APPEND LV_BMS_V1_0_default_library_list "$<TARGET_OBJECTS:LV_BMS_V1_0_default_toolchain_XC8_3_10_compile>")

endif()


# Main target for this project
add_executable(LV_BMS_V1_0_default_image_bXySFoyn ${LV_BMS_V1_0_default_library_list})

set_target_properties(LV_BMS_V1_0_default_image_bXySFoyn PROPERTIES
    OUTPUT_NAME "default"
    SUFFIX ".elf"
    ADDITIONAL_CLEAN_FILES "${output_extensions}"
    RUNTIME_OUTPUT_DIRECTORY "${LV_BMS_V1_0_default_output_dir}")
target_link_libraries(LV_BMS_V1_0_default_image_bXySFoyn PRIVATE ${LV_BMS_V1_0_default_toolchain_XC8_3_10_FILE_TYPE_link})

# Add the link options from the rule file.
LV_BMS_V1_0_default_link_rule( LV_BMS_V1_0_default_image_bXySFoyn)


