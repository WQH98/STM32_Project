#ifndef __TIM_PWM_CFG_H
#define __TIM_PWM_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tim_pwm.h"

static void tim3_ch4_pwm_rcc_enable(void)
{
    __HAL_RCC_TIM3_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
}

static GPIO_InitTypeDef tim3_ch4_pwm_pin_cfg = {
    .Alternate = GPIO_AF2_TIM3,
    .Mode = GPIO_MODE_AF_PP,
    .Pin = GPIO_PIN_1,
    .Pull = GPIO_PULLUP,
    .Speed = GPIO_SPEED_HIGH,
};

static tim_pwm_gpio_cfg_type tim3_ch4_pwm_cfg = {
    .port = GPIOB,
    .gpio_cfg = &tim3_ch4_pwm_pin_cfg,
};

static TIM_HandleTypeDef tim3_ch4_pwm_handle = {
    .Instance = TIM3,
    .Init.Prescaler = 89,
    .Init.CounterMode = TIM_COUNTERMODE_UP,
    .Init.Period = 499,
    .Init.ClockDivision = TIM_CLOCKDIVISION_DIV1,
};

static TIM_OC_InitTypeDef tim4_ch4_pwm_oc_cfg = {
    .OCMode = TIM_OCMODE_PWM1,
    .Pulse = (499 / 2.0),
    .OCPolarity = TIM_OCPOLARITY_LOW,
};

static tim_pwm_cfg_type tim_pwm_cfg_info[] = {
    {
        .rcc_enable = tim3_ch4_pwm_rcc_enable,
        .tim_handle = &tim3_ch4_pwm_handle,
        .tim_pwm_ch_handle = &tim4_ch4_pwm_oc_cfg,
        .tim_pwm_pin = &tim3_ch4_pwm_cfg,
        .Channel = TIM_CHANNEL_4,
        .id = 0,
    },
};

static int tim_pwm_init(void)
{
    return tim_pwm_register(tim_pwm_cfg_info, sizeof(tim_pwm_cfg_info) / sizeof(tim_pwm_cfg_info[0]));
}


#ifdef __cplusplus
}
#endif
#endif
