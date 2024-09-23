#ifndef __TIM_CAPTURE_H
#define __TIM_CAPTURE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

typedef enum TIM_CAPTURE_CHx {
    TIM5_CH1 = 0,
} TIM_CAPTURE_CHx_Type;

typedef struct tim_capture_gpio_cfg {
    GPIO_TypeDef *port;
    GPIO_InitTypeDef *gpio_cfg;
} tim_capture_gpio_cfg_type;

typedef struct tim_capture_cfg {
    void (*rcc_enable)(void);
    TIM_HandleTypeDef *tim_handle;
    tim_capture_gpio_cfg_type *tim_capture_pin;
    TIM_IC_InitTypeDef *tim_capture_ch_handle;
    uint32_t channel;
    int id;
    IRQn_Type IRQn;                         // 中断
    uint32_t PreemptPriority;               // 抢占优先级
    uint32_t SubPriority;                   // 子优先级
    uint8_t STA;
    uint32_t VAL;

} tim_capture_cfg_type;

int tim_capture_register(tim_capture_cfg_type *p_cfg, int num);

int get_capture_state(TIM_CAPTURE_CHx_Type timx);
int get_capture_val(TIM_CAPTURE_CHx_Type timx);
void set_capture_state(TIM_CAPTURE_CHx_Type timx, int data);
void set_capture_val(TIM_CAPTURE_CHx_Type timx, int data);

#ifdef __cplusplus
}
#endif
#endif
