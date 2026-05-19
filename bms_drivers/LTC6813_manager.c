#include "LTC6813_manager.h"
#include "ADI.h"
#include "LTC6813.h"
#include "spi_hal.h"

void LTC6813_manager_init(LTC6813_manager_t *p_inst, LTC6813_manager_cfg_t *p_cfg)
{
    if((p_inst != NULL) && (p_cfg != NULL))
    {
        p_inst->cfg = p_cfg;

        p_inst->md = 2;
        p_inst->dcp = 0;
        p_inst->ch = 0;
        p_inst->pup = 0;
        p_inst->st = 1;
        p_inst->chg = 0;
        p_inst->chst = 0;

        p_inst->cfgar[6] = 0;
        p_inst->cfgbr[6] = 0;
        p_inst->cvar[6] = 0;
        p_inst->cvbr[6] = 0;
        p_inst->cvcr[6] = 0;
        p_inst->cvdr[6] = 0;
        p_inst->cver[6] = 0;
        p_inst->cvfr[6] = 0;
        p_inst->avar[6] = 0;
        p_inst->avbr[6] = 0;
        p_inst->avcr[6] = 0;
        p_inst->avdr[6] = 0;
        p_inst->star[6] = 0;
        p_inst->stbr[6] = 0;
        p_inst->comm[6] = 0;
        p_inst->sctrl[6] = 0;
        p_inst->pwmr[6] = 0;
        p_inst->psr[6] = 0;

        p_inst->command_packet[4] = 0;
        p_inst->len = 0;

        p_inst->command_buffer = 0;
        p_inst->cv_buffer[18] = 0;
        p_inst->aux_buffer[12] = 0;

        p_inst->busy_flag = 0;
        p_inst->ready_flag = 0;

        p_inst->state = LTC6813_STATE_IDLE;
        p_inst->cmd = LTC6813_CMD_NONE;
    }
    else
    {
        // insert fault update function
    }

    init_PEC15_Table();
}

void LTC6813_manager_task(LTC6813_manager_t *p_inst)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
         if(!p_inst->busy_flag)
        {
            switch(p_inst->cmd)
            {
                case LTC6813_CMD_NONE:

                break;

                case LTC6813_CMD_WRITE_CFG:

                break;

                case LTC6813_CMD_POLL_CELLS:

                if(!p_inst->busy_flag)
                {
                    LTC6813_manager_poll_cells(p_inst);
                }

                if(p_inst->ready_flag)
                {
                    p_inst->cmd = LTC6813_CMD_NONE;
                }

                break;

                case LTC6813_CMD_POLL_GPIOS:

                break;

                case LTC6813_CMD_BALANCE:

                break;
            }
        }
    }
    else
    {
        // error handler
    } 
}

bool LTC6813_manager_cmd_request(LTC6813_manager_t *p_inst, LTC6813_cmd_t cmd)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        if(p_inst->state == LTC6813_STATE_IDLE)
        {
            p_inst->busy_flag = 1;
        }

    return p_inst->busy_flag;
    }
    else
    {
        // error handler
    }
    
}

void LTC6813_manager_poll_cells(LTC6813_manager_t *p_inst)
{

    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        switch(p_inst->state)
        {
            case LTC6813_STATE_IDLE:

            p_inst->busy_flag = 0;

            if(p_inst->command_buffer == LTC6813_CMD_POLL_CELLS)
            {
                p_inst->state = LTC6813_STATE_START;
            }

            break;

            case LTC6813_STATE_START:

            p_inst->ready_flag = 0;

            // need a send function but again, don't have spi figured out

            p_inst->delay_ms = 0U; // zero delay for safety

            p_inst->delay_ms = LTC6813_get_delay_ms(p_inst->md, p_inst->cfg->cell_num);

            uint32_t start_time = get_tick();

            break;

            case LTC6813_STATE_WAIT:

            if(get_tick() - start_time >= p_inst->delay_ms)
            {
                p_inst->state = LTC6813_STATE_GET;
            }

            break;

            case LTC6813_STATE_GET:

            // need a getter function or smth. this part sucks because i still don't have the spi shit figured out

            break;

            case LTC6813_STATE_READY:

            p_inst->ready_flag = 1;

            p_inst->state = LTC6813_STATE_IDLE;

            break;
        }
    }
    else 
    {
        // error handler
    }
}

void LTC6813_manager_get_command_packet(LTC6813_manager_t *p_inst, uint16_t val1, uint16_t val2)
{
    if((p_inst != NULL) && (p_inst->cfg != NULL))
    {
        p_inst->command_packet[4] = 0U;

        // msb command bit packing
        p_inst->command_packet[0] = (uint8_t)(val1 >> 8U);
        p_inst->command_packet[1] = (uint8_t)(val1 & 0x00FFU);
        p_inst->command_packet[2] = (uint8_t)(val2 >> 8U);
        p_inst->command_packet[3] = (uint8_t)(val2 & 0x00FFU);

        p_inst->len = 4U;
    }
}
