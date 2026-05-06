/**
 * @file current_manager.h
 * @author notwe
 * @date 2026-05-03
 * @brief current sensor manager source
 */

#include "current_manager.h"

static current_manager_t cur;
static current_manager_cfg_t cur_cfg;

void current_manager_init(current_manager_t *p_cur, current_manager_cfg_t *p_cur_cfg)
{
    //alias pointers
    cur = *p_cur;
    cur_cfg = *p_cur_cfg;

    cur.state = CURRENT_IDLE;

}

void current_manager_task(current_manager_t *p_cur)
{
    switch(cur.state)
    {
        case CURRENT_IDLE:

        break;

        case CURRENT_START:

        cur_cfg.set_pin();

        cur_cfg.ADC_start();

        cur.state = CURRENT_WAIT;

        break;

        case CURRENT_WAIT:

        if(cur_cfg.ADC_done())
        {
            cur.state = CURRENT_GET;
        }

        break;

        case CURRENT_GET:

        cur.raw = cur_cfg.ADC_get_result();



        break;
    }
}

int32_t process_current(current_manager_t *p_cur, uint32_t raw)
{

    uint32_t meas = raw * 1; // fix this later

    
    return 0;
}