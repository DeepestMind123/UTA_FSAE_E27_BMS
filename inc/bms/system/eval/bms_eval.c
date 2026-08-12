/**
 * @file bms_eval.c
 * @author notwe
 * @date 2026-07-08
 * @brief bms eval module source file
*/

#include "bms_eval.h"

typedef struct
{
    bool icon_done;
    bool vcon_done;
    bool tcon_done;
    bool plim_done;
} eval_ctx_t;


typedef struct
{
    bool is_init;

    eval_ctx_t ctx;
    eval_con_t con;
    daq_data_t data;

    eval_state_t state;
    eval_status_t status;
} eval_t;

static eval_t s_eval;

eval_status_t BMS_Eval_Init(void)
{
    eval_status_t status = EVAL_NOT_INIT;

    if(!s_eval.is_init)
    {
        s_eval.state = EVAL_STATE_IDLE;

        s_eval.ctx.icon_done = false;
        s_eval.ctx.vcon_done = false;
        s_eval.ctx.tcon_done = false;
        s_eval.ctx.plim_done = false;

        s_eval.is_init = true;
        status = EVAL_OK;
    }
    else
    {
        status = EVAL_DBL_INIT;
    }

    return status;
}

eval_status_t BMS_Eval_Task(const daq_data_t *p_data_in, eval_con_t *p_out)
{
    s_eval.status = EVAL_OK;

    if((p_data_in != NULL) && (p_out != NULL))
    {
        daq_data_t data = *p_data_in;

        if(s_eval.is_init)
        {
            if((s_eval.state >= EVAL_STATE_MAX) || (s_eval.state == EVAL_STATE_UNDEF))
            {
                s_eval.status = EVAL_UNDEF_STATE;

                s_eval.state = EVAL_STATE_ERROR;
            }

            switch(s_eval.state)
            {
                case EVAL_STATE_IDLE:
                {
                    if(!s_eval.ctx.icon_done)
                    {
                        s_eval.state = EVAL_STATE_ICON;
                    }
                    else if(!s_eval.ctx.vcon_done)
                    {
                        s_eval.state = EVAL_STATE_VCON;
                    }
                    else if(!s_eval.ctx.tcon_done)
                    {
                        s_eval.state = EVAL_STATE_TCON;
                    }
                    else if(!s_eval.ctx.plim_done)
                    {
                        s_eval.state = EVAL_STATE_PLIM;
                    }
                    else
                    {
                        s_eval.state = EVAL_STATE_DONE;
                    }

                    break;
                }

                case EVAL_STATE_ICON:
                {
                    s_eval.status = BMS_Eval_Icon(&data, p_out);

                    if(s_eval.status == EVAL_OK)
                    {
                        s_eval.ctx.icon_done = true;

                        s_eval.state = EVAL_STATE_IDLE;
                    }
                    else
                    {
                        s_eval.state = EVAL_STATE_ERROR;
                    }

                    break;
                }

                case EVAL_STATE_VCON:
                {
                    s_eval.status = BMS_Eval_Vcon(&data, p_out);

                    if(s_eval.status == EVAL_OK)
                    {
                        s_eval.ctx.vcon_done = true;

                        s_eval.state = EVAL_STATE_IDLE;
                    }
                    else
                    {
                        s_eval.state = EVAL_STATE_ERROR;
                    }

                    break;
                }

                case EVAL_STATE_TCON:
                {
                    s_eval.status = BMS_Eval_Tcon(&data, p_out);

                    if(s_eval.status == EVAL_OK)
                    {
                        s_eval.ctx.tcon_done = true;

                        s_eval.state = EVAL_STATE_IDLE;
                    }
                    else
                    {
                        s_eval.state = EVAL_STATE_ERROR;
                    }

                    break;
                }

                case EVAL_STATE_PLIM:
                {
                    s_eval.status = BMS_Eval_PLim(&data, p_out);

                    if(s_eval.status == EVAL_OK)
                    {
                        s_eval.ctx.plim_done = true;

                        s_eval.state = EVAL_STATE_IDLE;
                    }
                    else
                    {
                        s_eval.state = EVAL_STATE_ERROR;
                    }

                    break;
                }

                case EVAL_STATE_DONE:
                {
                    s_eval.ctx.icon_done = false;
                    s_eval.ctx.vcon_done = false;
                    s_eval.ctx.tcon_done = false;
                    s_eval.ctx.plim_done = false;

                    s_eval.state = EVAL_STATE_IDLE;

                    break;
                }

                case EVAL_STATE_ERROR:
                {
                    if(s_eval.status == EVAL_OK)
                    {
                        s_eval.status = EVAL_UNKNOWN_ERROR;
                    }

                    break;
                }
            }
        }
        else
        {
            s_eval.status = EVAL_NOT_INIT;
        }
    }
    else
    {
        s_eval.status = EVAL_NULL_PTR;
    }

    return s_eval.status;
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

eval_status_t BMS_Eval_PLim(const daq_data_t *p_data_in, eval_con_t *p_out)
{
    eval_status_t status = EVAL_OK;

    uint32_t limit_mA = 0U;
    uint32_t plim_mW = 0U;

    if((p_data_in != NULL) && (p_out != NULL))
    {
        daq_data_t data = *p_data_in;

        eval_con_t eval = *p_out;

        if(s_eval.is_init)
        {
            if((eval.batt_state == BATT_STATE_DCHG) || (eval.batt_state == BATT_STATE_IDLE))
            {
                plim_mW = DCL_W;
            }
            else if(eval.batt_state == BATT_STATE_CHG)
            {
                plim_mW = CCL_W;
            }
            else
            {
                status = EVAL_UNDEF_STATE;
            }

            if(status == EVAL_OK)
            {
                uint32_t pack_mV = 0U;

                if(data.v_data.v_valid)
                {
                    for(uint8_t i = 0U; i < MOD_NUM; i++)
                    {
                        for(uint8_t j = 0U; j < MOD_CELL_NUM; j++)
                        {
                            pack_mV += data.v_data.vmod[i].cell_val_mV[j];
                        }
                    }
                }
                else
                {
                    status = EVAL_VDATA_INVALID;
                }

                if((pack_mV <= UINT32_MAX) && (pack_mV != 0U))
                {
                    limit_mA = plim_mW / pack_mV;
                }
                else
                {
                    status = EVAL_VDATA_INVALID;
                }

                if((eval.batt_state == BATT_STATE_DCHG) || (eval.batt_state == BATT_STATE_IDLE))
                {
                    p_out->pwr_lim.dcl_mW = limit_mA;
                }
                else if(eval.batt_state == BATT_STATE_CHG)
                {
                    p_out->pwr_lim.ccl_mW = limit_mA;
                }
            }
        }
        else
        {
            status = EVAL_NOT_INIT;
        }
    }
    else
    {
        status = EVAL_NULL_PTR;
    }

    return status;
}

eval_status_t BMS_Eval_Get_State(eval_state_t *p_out)
{
    eval_status_t status = EVAL_OK;   

    eval_state_t state = EVAL_STATE_UNDEF;

    if(p_out != NULL)
    {
        if(s_eval.is_init)
        {
            state = s_eval.state;
        }
        else
        {
            status = EVAL_NOT_INIT;
        }

        *p_out = state;
    }
    else
    {
        status = EVAL_NULL_PTR;
    }

    return status;
}