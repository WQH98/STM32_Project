#include "tim_pwm_cfg.h"

tim_pwm_cfg_type *g_tim_pwm_cfg = NULL;

int tim_pwm_register(tim_pwm_cfg_type *p_cfg, int num) 
{
    int i = 0;
    if(NULL == p_cfg)
    {
        return -1;
    }
    g_tim_pwm_cfg = p_cfg;
    for(i = 0; i < num; i++)
    {
        g_tim_pwm_cfg[i].rcc_enable();
        HAL_TIM_PWM_Init(g_tim_pwm_cfg[i].tim_handle);
        HAL_TIM_PWM_ConfigChannel(g_tim_pwm_cfg[i].tim_handle, \
                                  g_tim_pwm_cfg[i].tim_pwm_ch_handle, \
                                  g_tim_pwm_cfg[i].Channel);
        HAL_GPIO_Init(g_tim_pwm_cfg[i].tim_pwm_pin->port, g_tim_pwm_cfg[i].tim_pwm_pin->gpio_cfg);
        HAL_TIM_PWM_Start(g_tim_pwm_cfg[i].tim_handle, g_tim_pwm_cfg[i].Channel);
    }
    return 0;
}


int set_pwm_compompare(TIM_PWM_CHx_type timx, uint32_t compare) 
{
    if(timx != g_tim_pwm_cfg[timx].id) 
    {
        return -1;
    }

    if(g_tim_pwm_cfg[timx].Channel == TIM_CHANNEL_1)
    {
        g_tim_pwm_cfg[timx].tim_handle->Instance->CCR1 = compare;
        return 0;
    }
    else if(g_tim_pwm_cfg[timx].Channel == TIM_CHANNEL_2)
    {
        g_tim_pwm_cfg[timx].tim_handle->Instance->CCR2 = compare;
        return 0;
    }
    else if(g_tim_pwm_cfg[timx].Channel == TIM_CHANNEL_3)
    {
        g_tim_pwm_cfg[timx].tim_handle->Instance->CCR3 = compare;
        return 0;
    }
    else if(g_tim_pwm_cfg[timx].Channel == TIM_CHANNEL_4)
    {
        g_tim_pwm_cfg[timx].tim_handle->Instance->CCR4 = compare;
        return 0;
    }
    

    return -2;
}
