#ifndef __LED_CFG_H
#define __LED_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "led.h"

// LED时钟初始化回调
static void gpio_rcc_init(void) 
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
}

// 绿灯配置
static GPIO_InitTypeDef greed_led_cfg = {
    .Mode  = GPIO_MODE_OUTPUT_PP,
    .Pin   = GPIO_PIN_0,
    .Pull  = GPIO_PULLUP,
    .Speed = GPIO_SPEED_FREQ_VERY_HIGH
};

static led_gpioinit_cfg_type green_led_gpio_cfg = {
    .port     = GPIOB,
    .gpio_cfg = &greed_led_cfg
};

// 红灯配置
static GPIO_InitTypeDef red_led_cfg = {
    .Mode  = GPIO_MODE_OUTPUT_PP,
    .Pin   = GPIO_PIN_1,
    .Pull  = GPIO_PULLUP,
    .Speed = GPIO_SPEED_FREQ_VERY_HIGH
};

static led_gpioinit_cfg_type red_led_gpio_cfg = {
    .port     = GPIOB,
    .gpio_cfg = &red_led_cfg
};


// LED设备配置信息
static led_cfg_type g_led_cfg[] = {
    {
        .id = 0,
        .polar = GPIO_PIN_RESET,
        .led_pin = &green_led_gpio_cfg,
        .rcc_enable = gpio_rcc_init
    },
    {
        .id = 1,
        .polar = GPIO_PIN_RESET,
        .led_pin = &red_led_gpio_cfg,
        .rcc_enable = gpio_rcc_init
    }
};

// LED初始化函数
static int led_init(void) 
{
    return led_register(g_led_cfg, sizeof(g_led_cfg) / sizeof(g_led_cfg[0]));
}


#ifdef __cplusplus
}
#endif
#endif

