#ifndef __KEY_H
#define __KEY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

// KEY枚举
typedef enum {
    WK_UP = 0,
    KEY0,
    KEY1,
    KEY2
} key_type;

// GPIO_Init 参数结构体
typedef struct key_gpioinit_cfg {
    GPIO_TypeDef *port;                 // KEY端口
    GPIO_InitTypeDef *gpio_cfg;         // HAL库 GPIO初始化参数
} key_gpioinit_cfg_type;

// KEY 参数 结构体
typedef struct key_cfg {
    key_gpioinit_cfg_type *key_pin;     // 初始化配置结构体
    uint8_t id;                         // KEY ID
    void (*rcc_enable)(void);           // 使能RCC时钟
} key_cfg_type;

// 得到按键的状态
// id：KEY值
int key_get_state(int id);

// KEY 注册
// p_cfg：KEY配置结构体指针
// num：KEY数量
int key_register(key_cfg_type *p_cfg, int num);



#ifdef __cplusplus
}
#endif
#endif
