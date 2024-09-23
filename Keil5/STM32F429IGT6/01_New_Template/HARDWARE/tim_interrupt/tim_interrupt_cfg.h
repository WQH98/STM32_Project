#ifndef __TIM_INTERRUPT_CFG_H
#define __TIM_INTERRUPT_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tim_interrupt.h"

static void tim3_rcc_enable(void)
{
    __HAL_RCC_TIM3_CLK_ENABLE();
}

static TIM_HandleTypeDef tim3_cfg = {
    .Instance = TIM3,
    .Init.Period = 999,
    .Init.Prescaler = 8999,
    .Init.CounterMode = TIM_COUNTERMODE_UP,
    .Init.ClockDivision = TIM_CLOCKDIVISION_DIV1
};

static tim_interrupt_cfg_type tim_interrupt_cfg_info[] = {
    {
        .tim_handle = &tim3_cfg,
        .rcc_enable = tim3_rcc_enable,
        .IRQn = TIM3_IRQn,
        .arr = 5000 - 1,   // 现在这个值是500ms进一次中断
        .PreemptPriority = 1,
        .SubPriority = 3
    },
};

static int tim_interrupt_init(void)
{
    return tim_interrupt_register(tim_interrupt_cfg_info, sizeof(tim_interrupt_cfg_info) / sizeof(tim_interrupt_cfg_info[0]));
}


#ifdef __cplusplus
}
#endif
#endif
