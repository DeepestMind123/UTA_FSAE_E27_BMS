# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "C:\\Users\\mediocre\\Documents\\GitHub\\uta_fsae_bms_v1.1\\out\\LV_BMS_V1.0\\default.cmf"
  "C:\\Users\\mediocre\\Documents\\GitHub\\uta_fsae_bms_v1.1\\out\\LV_BMS_V1.0\\default.hex"
  "C:\\Users\\mediocre\\Documents\\GitHub\\uta_fsae_bms_v1.1\\out\\LV_BMS_V1.0\\default.hxl"
  "C:\\Users\\mediocre\\Documents\\GitHub\\uta_fsae_bms_v1.1\\out\\LV_BMS_V1.0\\default.mum"
  "C:\\Users\\mediocre\\Documents\\GitHub\\uta_fsae_bms_v1.1\\out\\LV_BMS_V1.0\\default.o"
  "C:\\Users\\mediocre\\Documents\\GitHub\\uta_fsae_bms_v1.1\\out\\LV_BMS_V1.0\\default.sdb"
  "C:\\Users\\mediocre\\Documents\\GitHub\\uta_fsae_bms_v1.1\\out\\LV_BMS_V1.0\\default.sym"
  )
endif()
