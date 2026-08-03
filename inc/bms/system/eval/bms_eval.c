/**
 * @file bms_eval.c
 * @author notwe
 * @date 2026-07-08
 * @brief bms eval module source file
*/

#include "bms_eval.h"

typedef struct
{
    eval_con_t con;
    daq_data_t data;
} eval_t;

static eval_t s_eval;

eval_status_t BMS_Eval_Task(const daq_data_t *p_data_in, eval_con_t *p_out)
{
    eval_status_t status = EVAL_OK;

    if((p_data_in != NULL) && (p_out != NULL))
    {
        s_eval.data = *p_data_in;

        status = BMS_Eval_Icon(p_data_in, &s_eval.con);

        if(status == EVAL_OK)
        {
            status = BMS_Eval_Vcon(p_data_in, &s_eval.con);

            if(status == EVAL_OK)
            {
                status = BMS_Eval_Tcon(p_data_in, &s_eval.con);
            }
        }
    }
    else
    {
        status = EVAL_NULL_PTR;
    }

    return status;
}

eval_status_t BMS_Eval_Icon(const daq_data_t *p_data_in, eval_con_t *p_out)
{
    eval_status_t status = EVAL_OK;

    if((p_data_in != NULL) && (p_out != NULL))
    {
        s_eval.data = *p_data_in;
        s_eval.con = *p_out;

        if(s_eval.data.i_data.i_valid)
        {
            if(s_eval.data.i_data.ival_mA > 0)
            {
                s_eval.con.batt_state = BATT_STATE_DCHG;

                if(s_eval.data.i_data.ival_mA > OC_DCHG_LIM_MA)
                {
                    s_eval.con.icon.ipack_fault = true;
                    s_eval.con.icon.icon = EVAL_ICON_DCHG_OC;
                }
                else
                {
                    s_eval.con.icon.ipack_fault = false;
                    s_eval.con.icon.icon = EVAL_ICON_OK;
                }
            }
            else if(s_eval.data.i_data.ival_mA < 0)
            {
                s_eval.con.batt_state = BATT_STATE_CHG;

                if(labs(s_eval.data.i_data.ival_mA) > OC_CHG_LIM_MA)
                {
                    s_eval.con.icon.ipack_fault = true;
                    s_eval.con.icon.icon = EVAL_ICON_CHG_OC;
                }
                else
                {
                    s_eval.con.icon.ipack_fault = false;
                    s_eval.con.icon.icon = EVAL_ICON_OK;
                }
            }
            else
            {
                s_eval.con.batt_state = BATT_STATE_IDLE;
                s_eval.con.icon.ipack_fault = false;
                s_eval.con.icon.icon = EVAL_ICON_OK;
            }

            *p_out = s_eval.con;
        }
        else
        {
            status = EVAL_IDATA_INVALID;
        }
    }
    else
    {
        status = EVAL_NULL_PTR;
    }

    return status;
}

eval_status_t BMS_Eval_Vcon(const daq_data_t *p_data_in, eval_con_t *p_out)
{
    eval_status_t status = EVAL_OK;

    if((p_data_in != NULL) && (p_out != NULL))
    {
        s_eval.data = *p_data_in;
        
        s_eval.con = *p_out;

        if(s_eval.data.v_data.v_valid)
        {
            s_eval.con.vcon.vpack_fault = false;          

            for(uint8_t i = 0U; i < MOD_NUM; i++)
            {
                for(uint8_t j = 0U; j < MOD_CELL_NUM; j++)
                {
                    if(s_eval.data.v_data.vmod[i].cell_val_mV[j] > OV_LIM_MV)
                    {
                        s_eval.con.vcon.vpack_fault = true;
                        s_eval.con.vcon.vmod[i].cell_con[j] = EVAL_VCON_OV;
                    }
                    else if(s_eval.data.v_data.vmod[i].cell_val_mV[j] < UV_LIM_MV)
                    {
                        s_eval.con.vcon.vpack_fault = true;
                        s_eval.con.vcon.vmod[i].cell_con[j] = EVAL_VCON_UV;                        
                    }
                    else
                    {
                        s_eval.con.vcon.vmod[i].cell_con[j] = EVAL_VCON_OK;     
                    }
                }
            }

            *p_out = s_eval.con;
        }
        else
        {
            status = EVAL_VDATA_INVALID;
        }
    }
    else
    {
        status = EVAL_NULL_PTR;
    }

    return status;
}

eval_status_t BMS_Eval_Tcon(const daq_data_t *p_data_in, eval_con_t *p_out)
{
    eval_status_t status = EVAL_OK;

    float sum;
    float avg;

    if((p_data_in != NULL) && (p_out != NULL))
    {
        s_eval.data = *p_data_in;
        s_eval.con = *p_out;

        if(s_eval.data.t_data.t_valid)
        {
            s_eval.con.tcon.tpack_fault = false;          

            for(uint8_t i = 0U; i < MOD_NUM; i++)
            {
                for(uint8_t j = 0U; j < MOD_TEMP_NUM; j++)
                {
                    float temp = s_eval.data.t_data.tmod[i].tsense_val_C[j];

                    if(temp > OT_LIM_C)
                    {
                        s_eval.con.tcon.tpack_fault = true;
                        s_eval.con.tcon.tmod[i].temp_con[j] = EVAL_TCON_OT;
                    }
                    else if(temp < UT_LIM_C)
                    {
                        s_eval.con.tcon.tpack_fault = true;
                        s_eval.con.tcon.tmod[i].temp_con[j] = EVAL_TCON_UT;
                    }
                    else if((temp > (T_IDEAL_C + T_WIGGLE_C)) ||
                        (temp < (T_IDEAL_C - T_WIGGLE_C)))
                    {
                        s_eval.con.tcon.tmod[i].temp_con[j] = EVAL_TCON_NOT_OPTIMAL;
                    }
                    else
                    {
                        s_eval.con.tcon.tmod[i].temp_con[j] = EVAL_TCON_OK;
                    }
                }
            }

            *p_out = s_eval.con;
        }
        else
        {
            status = EVAL_TDATA_INVALID;
        }
    }
    else
    {
        status = EVAL_NULL_PTR;
    }

    return status;
}