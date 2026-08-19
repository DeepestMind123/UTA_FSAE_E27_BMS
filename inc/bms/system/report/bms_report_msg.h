/**
 * @file bms_report.h
 * @author notwe
 * @date 2026-07-21
 * @brief bms report over can header
 */

#ifndef BMS_REPORT_MSG_H
#define BMS_REPORT_MSG_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "io_can.h"
#include "bms_data.h"

#define DLC_SHORT 2U
#define DLC_MEDIUM 4U
#define DLC_LONG 6U
#define DLC_EXTRA_LONG 8U
#define MAX_ALIVE 255U
#define CRC16_LEN 2U

typedef enum
{
    ID_NONE = 0U,
    ID_STATE = 0x103U,
    ID_SOC = 0x104U,
    ID_SOE = 0x105U,
    ID_DRAW = 0x106U,
    ID_V = 0x107U,
    ID_LV = 0x108U,
    ID_HV = 0x109U,
    ID_CCL = 0x123U,
    ID_DCL = 0x124U,
    ID_OCV = 0x133U,
    ID_TEMP = 0x134U,
    ID_HTEMP = 0x135U,
    ID_LTEMP = 0x136U
} report_msg_id_t;

typedef struct
{
    const daq_data_t *data;
    const eval_con_t *con;
} report_msg_content_t;

uint16_t BMS_Report_MSG_Get_CRC16(const uint8_t *p_msg, uint8_t len);

bool BMS_Report_MSG_Build_STATE(const report_msg_content_t *p_content, can_msg_t *p_msg_out);

bool BMS_Report_MSG_Build_SOC(const report_msg_content_t *p_content, can_msg_t *p_msg_out);

bool BMS_Report_MSG_Build_SOE(const report_msg_content_t *p_content, can_msg_t *p_msg_out);

bool BMS_Report_MSG_Build_DRAW(const report_msg_content_t *p_content, can_msg_t *p_msg_out);

bool BMS_Report_MSG_Build_V(const report_msg_content_t *p_content, can_msg_t *p_msg_out);

bool BMS_Report_MSG_Build_LV(const report_msg_content_t *p_content, can_msg_t *p_msg_out);

bool BMS_Report_MSG_Build_CCL(const report_msg_content_t *p_content, can_msg_t *p_msg_out);

bool BMS_Report_MSG_Build_DCL(const report_msg_content_t *p_content, can_msg_t *p_msg_out);

bool BMS_Report_MSG_Build_OCV(const report_msg_content_t *p_content, can_msg_t *p_msg_out);

bool BMS_Report_MSG_Build_TEMP(const report_msg_content_t *p_content, can_msg_t *p_msg_out);

bool BMS_Report_MSG_Build_HTEMP(const report_msg_content_t *p_content, can_msg_t *p_msg_out);

bool BMS_Report_MSG_Build_LTEMP(const report_msg_content_t *p_content, can_msg_t *p_msg_out);

#endif