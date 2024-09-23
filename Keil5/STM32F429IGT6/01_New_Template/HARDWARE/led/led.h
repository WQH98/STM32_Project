#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stm32f4xx_hal.h>

#define GREEN_LED 0
#define RED_LED 1

// GPIO_Init 参数 结构体
typedef struct led_gpioinit_cfg {
    GPIO_TypeDef *port;             // LED 端口
    GPIO_InitTypeDef *gpio_cfg;     // HAL库 GPIO初始化参数
} led_gpioinit_cfg_type;

// LED 参数 结构体
typedef struct led_cfg {
    led_gpioinit_cfg_type *led_pin;    // 初始化配置结构体
    uint8_t id;              // LED ID
    GPIO_PinState polar;     // LED亮的电平
    void (*rcc_enable)(void);    // 使能RCC时钟
} led_cfg_type;

// 打开LED  id：LED id号
int led_on(int id);
// 关闭LED  id：LED id号
int led_off(int id);
// LED状态翻转  id：LED id号
int led_toggle(int id);
// 注册LED设备
// p_cfg LED配置结构体指针
// LED数量
int led_register(led_cfg_type *p_cfg, int num);


#ifdef __cplusplus
}
#endif
#endif
