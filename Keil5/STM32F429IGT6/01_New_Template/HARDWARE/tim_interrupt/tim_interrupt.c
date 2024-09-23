#include "tim_interrupt_cfg.h"

tim_interrupt_cfg_type *g_tim_cfg = NULL;

int tim_interrupt_register(tim_interrupt_cfg_type *p_cfg, int num)
{
    int i = 0;
    if(NULL == p_cfg)
    {
        return -1;
    }
    g_tim_cfg = p_cfg;
    for(i = 0; i < num; i++)
    {
        g_tim_cfg[i].rcc_enable();
        g_tim_cfg[i].tim_handle->Init.Period = g_tim_cfg[i].arr;
        HAL_TIM_Base_Init(g_tim_cfg[i].tim_handle);
        HAL_TIM_Base_Start_IT(g_tim_cfg[i].tim_handle);
        HAL_NVIC_SetPriority(g_tim_cfg[i].IRQn, g_tim_cfg[i].PreemptPriority, g_tim_cfg[i].SubPriority);
        HAL_NVIC_EnableIRQ(g_tim_cfg[i].IRQn);
    }
    return 0;
}


void TIM3_IRQHandler(void)
{
    if(__HAL_TIM_GET_FLAG(&tim3_cfg, TIM_IT_UPDATE))
    {
        __HAL_TIM_CLEAR_FLAG(&tim3_cfg, TIM_IT_UPDATE);
        // todo: 中断里的用户代码
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
    }
}

