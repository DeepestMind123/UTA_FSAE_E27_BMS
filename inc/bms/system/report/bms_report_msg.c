/**
 * @file bms_report.c
 * @author notwe
 * @date 2026-07-21
 * @brief bms report services
 */

#include "bms_report_msg.h"

static const uint16_t crc16_table[256] = { 0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301,
		0x03C0, 0x0280, 0xC241, 0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1,
		0xC481, 0x0440, 0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81,
		0x0E40, 0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
		0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40, 0x1E00,
		0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41, 0x1400, 0xD4C1,
		0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641, 0xD201, 0x12C0, 0x1380,
		0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040, 0xF001, 0x30C0, 0x3180, 0xF141,
		0x3300, 0xF3C1, 0xF281, 0x3240, 0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501,
		0x35C0, 0x3480, 0xF441, 0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0,
		0x3E80, 0xFE41, 0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881,
		0x3840, 0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
		0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40, 0xE401,
		0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640, 0x2200, 0xE2C1,
		0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041, 0xA001, 0x60C0, 0x6180,
		0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240, 0x6600, 0xA6C1, 0xA781, 0x6740,
		0xA501, 0x65C0, 0x6480, 0xA441, 0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01,
		0x6FC0, 0x6E80, 0xAE41, 0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1,
		0xA881, 0x6840, 0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80,
		0xBA41, 0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
		0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640, 0x7200,
		0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041, 0x5000, 0x90C1,
		0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241, 0x9601, 0x56C0, 0x5780,
		0x9741, 0x5500, 0x95C1, 0x9481, 0x5440, 0x9C01, 0x5CC0, 0x5D80, 0x9D41,
		0x5F00, 0x9FC1, 0x9E81, 0x5E40, 0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901,
		0x59C0, 0x5880, 0x9841, 0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1,
		0x8A81, 0x4A40, 0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80,
		0x8C41, 0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
		0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040 };

uint16_t BMS_Report_MSG_Get_CRC16(const uint8_t *p_msg, uint8_t len)
{
    uint16_t crc = 0xFFFFU;

    if(p_msg != NULL)
    {
        for(uint8_t i; i < len; i++)
        {
            crc ^= (*p_msg++) & 0x00FFU;
            crc = crc16_table[crc & 0x00FFU] ^ (crc >> 8);
        }
    }
    
    return crc;
}

bool BMS_Report_MSG_Build_STATE(const report_msg_content_t *p_content, can_msg_t *p_msg_out)
{
    bool build_success = false;

    if((p_content != NULL) && (p_msg_out != NULL))
    {
        can_msg_t msg = *p_msg_out;
        report_msg_content_t content =*p_content;

        msg.id_type = CAN_ID_STD;
        msg.id = ID_STATE;
        msg.frame_type = CAN_FRAME_STD;
        msg.dlc = DLC_MEDIUM;

        msg.data[0U] = 0x0000U;
        msg.data[0U] = content.con->state;
        msg.data[DLC_MEDIUM - CRC16_LEN] = BMS_Report_MSG_Get_CRC16(msg.data, (DLC_MEDIUM - CRC16_LEN));

        build_success = true;
    }

    return build_success;
}

bool BMS_Report_MSG_Build_SOC(const report_msg_content_t *p_content, can_msg_t *p_msg_out)
{
    bool build_success = false;

    if((p_content != NULL) && (p_msg_out != NULL))
    {
        can_msg_t msg = *p_msg_out;
        report_msg_content_t content =*p_content;

        msg.id_type = CAN_ID_STD;
        msg.id = ID_SOC;
        msg.frame_type = CAN_FRAME_STD;
        msg.dlc = DLC_MEDIUM;

        msg.data[0U] = 0x0000U;
        msg.data[0U] = content.con->chg_eng.soc;
        msg.data[DLC_MEDIUM - CRC16_LEN] = BMS_Report_MSG_Get_CRC16(msg.data, (DLC_MEDIUM - CRC16_LEN));

        build_success = true;
    }

    return build_success;
}

bool BMS_Report_MSG_Build_SOE(const report_msg_content_t *p_content, can_msg_t *p_msg_out)
{
    bool build_success = false;

    if((p_content != NULL) && (p_msg_out != NULL))
    {
        can_msg_t msg = *p_msg_out;
        report_msg_content_t content =*p_content;

        msg.id_type = CAN_ID_STD;
        msg.id = ID_SOE;
        msg.frame_type = CAN_FRAME_STD;
        msg.dlc = DLC_MEDIUM;

        msg.data[0U] = 0x0000U;
        msg.data[0U] = content.con->chg_eng.soe;
        msg.data[DLC_MEDIUM - CRC16_LEN] = BMS_Report_MSG_Get_CRC16(msg.data, (DLC_MEDIUM - CRC16_LEN));

        build_success = true;
    }

    return build_success;
}

bool BMS_Report_MSG_Build_DRAW(const report_msg_content_t *p_content, can_msg_t *p_msg_out)
{
    bool build_success = false;

    if((p_content != NULL) && (p_msg_out != NULL))
    {
        can_msg_t msg = *p_msg_out;
        report_msg_content_t content =*p_content;

        msg.id_type = CAN_ID_STD;
        msg.id = ID_DRAW;
        msg.frame_type = CAN_FRAME_STD;
        msg.dlc = DLC_LONG;

        msg.data[0U] = 0x000000U;
        msg.data[0U] = content.con->icon.draw;
        msg.data[DLC_LONG - CRC16_LEN] = BMS_Report_MSG_Get_CRC16(msg.data, (DLC_LONG - CRC16_LEN));

        build_success = true;
    }

    return build_success;
}

bool BMS_Report_MSG_Build_V(const report_msg_content_t *p_content, can_msg_t *p_msg_out)
{
    bool build_success = false;

    if((p_content != NULL) && (p_msg_out != NULL))
    {
        can_msg_t msg = *p_msg_out;
        report_msg_content_t content =*p_content;

        msg.id_type = CAN_ID_STD;
        msg.id = ID_V;
        msg.frame_type = CAN_FRAME_STD;
        msg.dlc = DLC_LONG;

        msg.data[0U] = 0x000000U;
        msg.data[0U] = content.con->vcon.total_mV;
        msg.data[DLC_LONG - CRC16_LEN] = BMS_Report_MSG_Get_CRC16(msg.data, (DLC_LONG - CRC16_LEN));

        build_success = true;
    }

    return build_success;
}

bool BMS_Report_MSG_Build_LV(const report_msg_content_t *p_content, can_msg_t *p_msg_out)
{
    bool build_success = false;

    if((p_content != NULL) && (p_msg_out != NULL))
    {
        can_msg_t msg = *p_msg_out;
        report_msg_content_t content =*p_content;

        msg.id_type = CAN_ID_STD;
        msg.id = ID_LV;
        msg.frame_type = CAN_FRAME_STD;
        msg.dlc = DLC_LONG;

        msg.data[0U] = 0x000000U;
        msg.data[0U] = content.con->vcon.low_cell;
        msg.data[2U] = content.con->vcon.low_cell_loc.place;
        msg.data[DLC_LONG - CRC16_LEN] = BMS_Report_MSG_Get_CRC16(msg.data, (DLC_LONG - CRC16_LEN));

        build_success = true;
    }

    return build_success;
}

bool BMS_Report_MSG_Build_HV(const report_msg_content_t *p_content, can_msg_t *p_msg_out)
{
    bool build_success = false;

    if((p_content != NULL) && (p_msg_out != NULL))
    {
        can_msg_t msg = *p_msg_out;
        report_msg_content_t content =*p_content;

        msg.id_type = CAN_ID_STD;
        msg.id = ID_HV;
        msg.frame_type = CAN_FRAME_STD;
        msg.dlc = DLC_LONG;

        msg.data[0U] = 0x000000U;
        msg.data[0U] = content.con->vcon.high_cell;
        msg.data[2U] = content.con->vcon.high_cell_loc.place;
        msg.data[DLC_LONG - CRC16_LEN] = BMS_Report_MSG_Get_CRC16(msg.data, (DLC_LONG - CRC16_LEN));

        build_success = true;
    }

    return build_success;
}

bool BMS_Report_MSG_Build_CCL(const report_msg_content_t *p_content, can_msg_t *p_msg_out)
{
    bool build_success = false;

    if((p_content != NULL) && (p_msg_out != NULL))
    {
        can_msg_t msg = *p_msg_out;
        report_msg_content_t content =*p_content;

        msg.id_type = CAN_ID_STD;
        msg.id = ID_CCL;
        msg.frame_type = CAN_FRAME_STD;
        msg.dlc = DLC_LONG;

        msg.data[0U] = 0x000000U;
        msg.data[0U] = content.con->pwr_lim.ccl_mW;
        msg.data[DLC_LONG - CRC16_LEN] = BMS_Report_MSG_Get_CRC16(msg.data, (DLC_LONG - CRC16_LEN));

        build_success = true;
    }

    return build_success;
}

bool BMS_Report_MSG_Build_DCL(const report_msg_content_t *p_content, can_msg_t *p_msg_out)
{
    bool build_success = false;

    if((p_content != NULL) && (p_msg_out != NULL))
    {
        can_msg_t msg = *p_msg_out;
        report_msg_content_t content =*p_content;

        msg.id_type = CAN_ID_STD;
        msg.id = ID_DCL;
        msg.frame_type = CAN_FRAME_STD;
        msg.dlc = DLC_LONG;

        msg.data[0U] = 0x000000U;
        msg.data[0U] = content.con->pwr_lim.dcl_mW;
        msg.data[DLC_LONG - CRC16_LEN] = BMS_Report_MSG_Get_CRC16(msg.data, (DLC_LONG - CRC16_LEN));

        build_success = true;
    }

    return build_success;
}

bool BMS_Report_MSG_Build_OCV(const report_msg_content_t *p_content, can_msg_t *p_msg_out)
{
    /* currently unsupported*/
    return false;
}

bool BMS_Report_MSG_Build_TEMP(const report_msg_content_t *p_content, can_msg_t *p_msg_out)
{
    bool build_success = false;

    if((p_content != NULL) && (p_msg_out != NULL))
    {
        can_msg_t msg = *p_msg_out;
        report_msg_content_t content =*p_content;

        msg.id_type = CAN_ID_STD;
        msg.id = ID_TEMP;
        msg.frame_type = CAN_FRAME_STD;
        msg.dlc = DLC_LONG;

        msg.data[0U] = 0x000000U;
        msg.data[0U] = content.con->tcon.temp_avg;
        msg.data[DLC_LONG - CRC16_LEN] = BMS_Report_MSG_Get_CRC16(msg.data, (DLC_LONG - CRC16_LEN));

        build_success = true;
    }

    return build_success;
}

bool BMS_Report_MSG_Build_HTEMP(const report_msg_content_t *p_content, can_msg_t *p_msg_out)
{
    bool build_success = false;

    if((p_content != NULL) && (p_msg_out != NULL))
    {
        can_msg_t msg = *p_msg_out;
        report_msg_content_t content =*p_content;

        msg.id_type = CAN_ID_STD;
        msg.id = ID_HTEMP;
        msg.frame_type = CAN_FRAME_STD;
        msg.dlc = DLC_EXTRA_LONG;

        msg.data[0U] = 0x00000000U;
        msg.data[0U] = content.con->tcon.high_temp;
        msg.data[4U] = content.con->tcon.high_temp_loc.place;
        msg.data[DLC_EXTRA_LONG - CRC16_LEN] = BMS_Report_MSG_Get_CRC16(msg.data, (DLC_EXTRA_LONG - CRC16_LEN));

        build_success = true;
    }

    return build_success;   
}

bool BMS_Report_MSG_Build_LTEMP(const report_msg_content_t *p_content, can_msg_t *p_msg_out)
{
    bool build_success = false;

    if((p_content != NULL) && (p_msg_out != NULL))
    {
        can_msg_t msg = *p_msg_out;
        report_msg_content_t content =*p_content;

        msg.id_type = CAN_ID_STD;
        msg.id = ID_LTEMP;
        msg.frame_type = CAN_FRAME_STD;
        msg.dlc = DLC_EXTRA_LONG;

        msg.data[0U] = 0x00000000U;
        msg.data[0U] = content.con->tcon.low_temp;
        msg.data[4U] = content.con->tcon.low_temp_loc.place;
        msg.data[DLC_EXTRA_LONG - CRC16_LEN] = BMS_Report_MSG_Get_CRC16(msg.data, (DLC_EXTRA_LONG - CRC16_LEN));

        build_success = true;
    }

    return build_success;   
}