/**
 * @file bms_config.h
 * @author notwe
 * @date 2026-05-03
 * @brief battery configuration header
 */

#ifndef BMS_CONFIG_H
#define BMS_CONFIG_H

#define MOD_CELL_NUM 28
#define MOD_TEMP_NUM 16
#define MOD_BAL_IC_NUM 0
#define MOD_BMS_IC_NUM 2
#define MOD_NUM 5

#define TOTAL_BMS_IC_NUM (MOD_BMS_IC_NUM * MOD_NUM)
#define TOTAL_CELL_NUM (MOD_CELL_NUM * MOD_NUM)
#define CELLS_PER_BMS_IC (MOD_CELL_NUM / MOD_BMS_IC_NUM)

#define UV_LIM_MV 2500
#define OV_LIM_MV 4200
#define OT_LIM_DC 600
#define OC_LIM_MA 600000

#define TEMP_BETA_VAL 3345
#define TEMP_RS 10000
#define TEMP_RN 10000
#define TEMP_NOM_dK 29815

#endif