#ifndef __TIM_CAPTURE_CFG_H
#define __TIM_CAPTURE_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tim_capture.h"

static void tim5_ch1_capture_rcc_enable(void)
{
    __HAL_RCC_TIM5_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
}

static GPIO_InitTypeDef tim5_ch1_capture_pin_cfg = {
    .Alternate = GPIO_AF2_TIM5,
    .Mode = GPIO_MODE_AF_PP,
    .Pin = GPIO_PIN_0,
    .Pull = GPIO_PULLDOWN,
    .Speed = GPIO_SPEED_HIGH,
};

static tim_capture_gpio_cfg_type tim5_ch1_capture_gpio_cfg = {
    .port = GPIOA,
    .gpio_cfg = &tim5_ch1_capture_pin_cfg,
};

static TIM_HandleTypeDef tim5_ch1_capture_handle = {
    .Instance = TIM5,               // 通用定时器5
    .Init.Prescaler = 89,           // 分频系数
    .Init.CounterMode = TIM_COUNTERMODE_UP,      // 向上计数器
    .Init.Period = 0xFFFFFFFF,           // 自动装载值
    .Init.ClockDivision = TIM_CLOCKDIVISION_DIV1,     // 时钟分频因子
};

static TIM_IC_InitTypeDef tim5_ch1_capture_ic_handle = {
    .ICPolarity = TIM_ICPOLARITY_RISING,      // 上升沿捕获
    .ICSelection = TIM_ICSELECTION_DIRECTTI,   // IC1映射到TI1上
    .ICPrescaler = TIM_ICPSC_DIV1,           // 配置输入分频 不分频
    .ICFilter = 0,           //不滤波
};

static tim_capture_cfg_type tim_capture_cfg_info[] = {
    {
        .rcc_enable = tim5_ch1_capture_rcc_enable,
        .tim_handle = &tim5_ch1_capture_handle,
        .tim_capture_ch_handle = &tim5_ch1_capture_ic_handle,
        .tim_capture_pin = &tim5_ch1_capture_gpio_cfg,
        .channel = TIM_CHANNEL_1,
        .id = 0,
        .IRQn = TIM5_IRQn,
        .PreemptPriority = 2,
        .SubPriority = 0,
        .STA = 0,
        .VAL = 0,
    },
};

static int tim_capture_init(void)
{
    return tim_capture_register(tim_capture_cfg_info, sizeof(tim_capture_cfg_info) / sizeof(tim_capture_cfg_info[0]));
}

#ifdef __cplusplus
}
#endif
#endif
