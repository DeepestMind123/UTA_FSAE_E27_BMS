# The following functions contains all the flags passed to the different build stages.

set(PACK_REPO_PATH "C:/Users/notwe/.mchp_packs" CACHE PATH "Path to the root of a pack repository.")

function(LV_BMS_V1_0_default_toolchain_XC8_3_10_assemble_rule target)
    set(options
        "-c"
        "${MP_EXTRA_AS_PRE}"
        "-mcpu=18LF26K83"
        "-mdfp=${PACK_REPO_PATH}/Microchip/PIC18F-K_DFP/1.16.308/xc8"
        "-fno-short-double"
        "-fno-short-float"
        "-memi=wordwrite"
        "-O0"
        "-maddrqual=ignore"
        "-mwarn=-3"
        "-msummary=-psect,-class,+mem,-hex,-file"
        "-ginhx32"
        "-Wl,--data-init"
        "-mno-keep-startup"
        "-mno-download"
        "-mno-default-config-bits"
        "-std=c99"
        "-gdwarf-3"
        "-mstack=compiled:auto:auto:auto")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__18LF26K83__"
        PRIVATE "XPRJ_default=default")
    target_include_directories(${target}
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/bms_drivers"
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/inc"
        PRIVATE "c:/Users/mediocre/Documents/GitHub/uta_fsae_bms_v1.1"
        PRIVATE "c:/Users/mediocre/Documents/GitHub/uta_fsae_bms_v1.1/inc"
        PRIVATE "c:/Users/mediocre/Documents/GitHub/uta_fsae_bms_v1.1/bms_drivers"
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/inc/bms"
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/inc/bms/devices"
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/inc/bms/io"
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/inc/bms/system"
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/inc/bms/utils")
endfunction()
function(LV_BMS_V1_0_default_toolchain_XC8_3_10_assemblePreprocess_rule target)
    set(options
        "-c"
        "${MP_EXTRA_AS_PRE}"
        "-mcpu=18LF26K83"
        "-x"
        "assembler-with-cpp"
        "-mdfp=${PACK_REPO_PATH}/Microchip/PIC18F-K_DFP/1.16.308/xc8"
        "-fno-short-double"
        "-fno-short-float"
        "-memi=wordwrite"
        "-O0"
        "-maddrqual=ignore"
        "-mwarn=-3"
        "-msummary=-psect,-class,+mem,-hex,-file"
        "-ginhx32"
        "-Wl,--data-init"
        "-mno-keep-startup"
        "-mno-download"
        "-mno-default-config-bits"
        "-std=c99"
        "-gdwarf-3"
        "-mstack=compiled:auto:auto:auto")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__18LF26K83__"
        PRIVATE "XPRJ_default=default")
    target_include_directories(${target}
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/bms_drivers"
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/inc"
        PRIVATE "c:/Users/mediocre/Documents/GitHub/uta_fsae_bms_v1.1"
        PRIVATE "c:/Users/mediocre/Documents/GitHub/uta_fsae_bms_v1.1/inc"
        PRIVATE "c:/Users/mediocre/Documents/GitHub/uta_fsae_bms_v1.1/bms_drivers"
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/inc/bms"
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/inc/bms/devices"
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/inc/bms/io"
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/inc/bms/system"
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/inc/bms/utils")
endfunction()
function(LV_BMS_V1_0_default_toolchain_XC8_3_10_compile_rule target)
    set(options
        "-c"
        "${MP_EXTRA_CC_PRE}"
        "-mcpu=18LF26K83"
        "-mdfp=${PACK_REPO_PATH}/Microchip/PIC18F-K_DFP/1.16.308/xc8"
        "-fno-short-double"
        "-fno-short-float"
        "-memi=wordwrite"
        "-O0"
        "-maddrqual=ignore"
        "-mwarn=-3"
        "-msummary=-psect,-class,+mem,-hex,-file"
        "-ginhx32"
        "-Wl,--data-init"
        "-mno-keep-startup"
        "-mno-download"
        "-mno-default-config-bits"
        "-std=c99"
        "-gdwarf-3"
        "-mstack=compiled:auto:auto:auto")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__18LF26K83__"
        PRIVATE "XPRJ_default=default")
    target_include_directories(${target}
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/bms_drivers"
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/inc"
        PRIVATE "c:/Users/mediocre/Documents/GitHub/uta_fsae_bms_v1.1"
        PRIVATE "c:/Users/mediocre/Documents/GitHub/uta_fsae_bms_v1.1/inc"
        PRIVATE "c:/Users/mediocre/Documents/GitHub/uta_fsae_bms_v1.1/bms_drivers"
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/inc/bms"
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/inc/bms/devices"
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/inc/bms/io"
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/inc/bms/system"
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/inc/bms/utils")
endfunction()
function(LV_BMS_V1_0_default_link_rule target)
    set(options
        "-Wl,-Map=mem.map"
        "${MP_EXTRA_LD_PRE}"
        "-mcpu=18LF26K83"
        "-Wl,--defsym=__MPLAB_BUILD=1"
        "-mdfp=${PACK_REPO_PATH}/Microchip/PIC18F-K_DFP/1.16.308/xc8"
        "-fno-short-double"
        "-fno-short-float"
        "-memi=wordwrite"
        "-O0"
        "-maddrqual=ignore"
        "-mwarn=-3"
        "-msummary=-psect,-class,+mem,-hex,-file"
        "-ginhx32"
        "-Wl,--data-init"
        "-mno-keep-startup"
        "-mno-download"
        "-mno-default-config-bits"
        "-std=c99"
        "-gdwarf-3"
        "-mstack=compiled:auto:auto:auto"
        "-Wl,--memorysummary,memoryfile.xml")
    list(REMOVE_ITEM options "")
    target_link_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target} PRIVATE "XPRJ_default=default")
    target_include_directories(${target}
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/bms_drivers"
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/inc"
        PRIVATE "c:/Users/mediocre/Documents/GitHub/uta_fsae_bms_v1.1"
        PRIVATE "c:/Users/mediocre/Documents/GitHub/uta_fsae_bms_v1.1/inc"
        PRIVATE "c:/Users/mediocre/Documents/GitHub/uta_fsae_bms_v1.1/bms_drivers"
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/inc/bms"
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/inc/bms/devices"
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/inc/bms/io"
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/inc/bms/system"
        PRIVATE "c:/Users/notwe/MPLABProjects/uta_fsae_bms_v1.1/inc/bms/utils")
endfunction()
