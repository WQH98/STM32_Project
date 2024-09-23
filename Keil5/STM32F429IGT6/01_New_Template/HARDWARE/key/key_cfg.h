#ifndef __KEY_CFG_H
#define __KEY_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "key.h"

// WU_UP时钟初始化回调函数
static void wk_up_rcc_enable(void) 
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
}

// KEY0时钟初始化回调函数
static void key0_rcc_enable(void) 
{
    __HAL_RCC_GPIOH_CLK_ENABLE();
}

// KEY1时钟初始化回调函数
static void key1_rcc_enable(void) 
{
    __HAL_RCC_GPIOH_CLK_ENABLE();
}

// KEY2时钟初始化回调函数
static void key2_rcc_enable(void) 
{
    __HAL_RCC_GPIOC_CLK_ENABLE();
}

// WK_UP配置
static GPIO_InitTypeDef wk_up_cfg = {
    .Mode = GPIO_MODE_INPUT,
    .Pin = GPIO_PIN_0,
    .Pull = GPIO_PULLDOWN,
    .Speed = GPIO_SPEED_FREQ_HIGH,
};

static key_gpioinit_cfg_type wu_up_gpio_cfg = {
    .port = GPIOA,
    .gpio_cfg = &wk_up_cfg,  
};

// KEY0配置
static GPIO_InitTypeDef key0_cfg = {
    .Mode = GPIO_MODE_INPUT,
    .Pin = GPIO_PIN_3,
    .Pull = GPIO_PULLUP,
    .Speed = GPIO_SPEED_FREQ_HIGH,
};

static key_gpioinit_cfg_type key0_gpio_cfg = {
    .port = GPIOH,
    .gpio_cfg = &key0_cfg,  
};

// KEY1配置
static GPIO_InitTypeDef key1_cfg = {
    .Mode = GPIO_MODE_INPUT,
    .Pin = GPIO_PIN_2,
    .Pull = GPIO_PULLUP,
    .Speed = GPIO_SPEED_FREQ_HIGH,
};

static key_gpioinit_cfg_type key1_gpio_cfg = {
    .port = GPIOH,
    .gpio_cfg = &key1_cfg,  
};

// KEY2配置
static GPIO_InitTypeDef key2_cfg = {
    .Mode = GPIO_MODE_INPUT,
    .Pin = GPIO_PIN_13,
    .Pull = GPIO_PULLUP,
    .Speed = GPIO_SPEED_FREQ_HIGH,
};

static key_gpioinit_cfg_type key2_gpio_cfg = {
    .port = GPIOC,
    .gpio_cfg = &key2_cfg,  
};



// KEY配置信息
static key_cfg_type g_key_cfg[] = {
    {
        .id = WK_UP,
        .key_pin = &wu_up_gpio_cfg,
        .rcc_enable = wk_up_rcc_enable,
    },
    {
        .id = KEY0,
        .key_pin = &key0_gpio_cfg,
        .rcc_enable = key0_rcc_enable,
    },
    {
        .id = KEY1,
        .key_pin = &key1_gpio_cfg,
        .rcc_enable = key1_rcc_enable,
    },
    {
        .id = KEY2,
        .key_pin = &key2_gpio_cfg,
        .rcc_enable = key2_rcc_enable,
    },
};

// KEY 设备 初始化
static int key_init(void) {
    return key_register(g_key_cfg, sizeof(g_key_cfg) / sizeof(g_key_cfg[0]));
}

#ifdef __cplusplus
}
#endif
#endif
