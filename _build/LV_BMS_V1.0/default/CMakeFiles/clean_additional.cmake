# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "C:\\Users\\notwe\\MPLABProjects\\LV_BMS_V1.0\\out\\LV_BMS_V1.0\\default.cmf"
  "C:\\Users\\notwe\\MPLABProjects\\LV_BMS_V1.0\\out\\LV_BMS_V1.0\\default.hex"
  "C:\\Users\\notwe\\MPLABProjects\\LV_BMS_V1.0\\out\\LV_BMS_V1.0\\default.hxl"
  "C:\\Users\\notwe\\MPLABProjects\\LV_BMS_V1.0\\out\\LV_BMS_V1.0\\default.mum"
  "C:\\Users\\notwe\\MPLABProjects\\LV_BMS_V1.0\\out\\LV_BMS_V1.0\\default.o"
  "C:\\Users\\notwe\\MPLABProjects\\LV_BMS_V1.0\\out\\LV_BMS_V1.0\\default.sdb"
  "C:\\Users\\notwe\\MPLABProjects\\LV_BMS_V1.0\\out\\LV_BMS_V1.0\\default.sym"
  )
endif()
