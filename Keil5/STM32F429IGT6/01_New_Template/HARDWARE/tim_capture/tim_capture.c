#include "tim_capture_cfg.h"

tim_capture_cfg_type *g_tim_capture_cfg = NULL;

int tim_capture_register(tim_capture_cfg_type *p_cfg, int num)
{
    if(NULL == p_cfg)
    {
        return -1;
    }
    int i = 0;
    g_tim_capture_cfg = p_cfg;
    for(i = 0; i < num; i++)
    {
        g_tim_capture_cfg[i].rcc_enable();
        HAL_TIM_IC_Init(g_tim_capture_cfg[i].tim_handle);
        HAL_TIM_IC_ConfigChannel(g_tim_capture_cfg[i].tim_handle, \
                                 g_tim_capture_cfg[i].tim_capture_ch_handle, \
                                 g_tim_capture_cfg[i].channel);
        HAL_TIM_IC_Start_IT(g_tim_capture_cfg[i].tim_handle, \
                            g_tim_capture_cfg[i].channel);
        __HAL_TIM_ENABLE_IT(g_tim_capture_cfg[i].tim_handle, TIM_IT_UPDATE);
        HAL_GPIO_Init(g_tim_capture_cfg[i].tim_capture_pin->port, \
                      g_tim_capture_cfg[i].tim_capture_pin->gpio_cfg);
        HAL_NVIC_SetPriority(g_tim_capture_cfg[i].IRQn, 
                             g_tim_capture_cfg[i].PreemptPriority, 
                             g_tim_capture_cfg[i].SubPriority);
        HAL_NVIC_EnableIRQ(g_tim_capture_cfg[i].IRQn);
    }
    return 0;
}


void TIM5_IRQHandler(void)
{    
    if(__HAL_TIM_GET_FLAG(g_tim_capture_cfg[TIM5_CH1].tim_handle, TIM_FLAG_UPDATE))
    {
        __HAL_TIM_CLEAR_FLAG(g_tim_capture_cfg[TIM5_CH1].tim_handle, TIM_FLAG_UPDATE);

        if((g_tim_capture_cfg[TIM5_CH1].STA & 0x80) == 0)
        {
            if(g_tim_capture_cfg[TIM5_CH1].STA & 0x40)
            {
                if((g_tim_capture_cfg[TIM5_CH1].STA & 0x3F) == 0x3F)
                {
                    g_tim_capture_cfg[TIM5_CH1].STA |= 0x80;
                    g_tim_capture_cfg[TIM5_CH1].VAL = 0XFFFFFFFF;
                }
                else
                {
                    g_tim_capture_cfg[TIM5_CH1].STA++;
                }
            }
        }
    }
    if(__HAL_TIM_GET_FLAG(g_tim_capture_cfg[TIM5_CH1].tim_handle, TIM_FLAG_CC1))
    {
        __HAL_TIM_CLEAR_FLAG(g_tim_capture_cfg[TIM5_CH1].tim_handle, TIM_FLAG_CC1);

        if((g_tim_capture_cfg[TIM5_CH1].STA & 0x80) == 0)
        {
            if(g_tim_capture_cfg[TIM5_CH1].STA & 0x40)
            {
                g_tim_capture_cfg[TIM5_CH1].STA |= 0x80;
                g_tim_capture_cfg[TIM5_CH1].VAL = HAL_TIM_ReadCapturedValue(g_tim_capture_cfg[TIM5_CH1].tim_handle, \
                                                                            g_tim_capture_cfg[TIM5_CH1].channel);
                TIM_RESET_CAPTUREPOLARITY(g_tim_capture_cfg[TIM5_CH1].tim_handle, \
                                        g_tim_capture_cfg[TIM5_CH1].channel);
                TIM_SET_CAPTUREPOLARITY(g_tim_capture_cfg[TIM5_CH1].tim_handle, \
                                        g_tim_capture_cfg[TIM5_CH1].channel, \
                                        TIM_ICPOLARITY_RISING);
            }
            else
            {
                g_tim_capture_cfg[TIM5_CH1].STA = 0;
                g_tim_capture_cfg[TIM5_CH1].VAL = 0;
                g_tim_capture_cfg[TIM5_CH1].STA |= 0x40;
                __HAL_TIM_DISABLE(g_tim_capture_cfg[TIM5_CH1].tim_handle);
                __HAL_TIM_SET_COUNTER(g_tim_capture_cfg[TIM5_CH1].tim_handle, 0);
                TIM_RESET_CAPTUREPOLARITY(g_tim_capture_cfg[TIM5_CH1].tim_handle, \
                                        g_tim_capture_cfg[TIM5_CH1].channel);
                TIM_SET_CAPTUREPOLARITY(g_tim_capture_cfg[TIM5_CH1].tim_handle, \
                                        g_tim_capture_cfg[TIM5_CH1].channel, \
                                        TIM_ICPOLARITY_FALLING);
                __HAL_TIM_ENABLE(g_tim_capture_cfg[TIM5_CH1].tim_handle);
            }
        }
    }
    HAL_TIM_IRQHandler(g_tim_capture_cfg[TIM5_CH1].tim_handle);
}

int get_capture_state(TIM_CAPTURE_CHx_Type timx)
{
    return g_tim_capture_cfg[timx].STA;
}

int get_capture_val(TIM_CAPTURE_CHx_Type timx)
{
    return g_tim_capture_cfg[timx].VAL;
}

void set_capture_state(TIM_CAPTURE_CHx_Type timx, int data)
{
    g_tim_capture_cfg[timx].STA = data;
}

void set_capture_val(TIM_CAPTURE_CHx_Type timx, int data)
{
    g_tim_capture_cfg[timx].VAL = data;
}

