#ifndef __TIM_PWM_H
#define __TIM_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"


typedef enum TIM_PWM_CHx{
    TIM3_CH4 = 0,
} TIM_PWM_CHx_type;


typedef struct tim_pwm_gpio_cfg {
    GPIO_TypeDef *port;
    GPIO_InitTypeDef *gpio_cfg;
} tim_pwm_gpio_cfg_type;


typedef struct tim_pwm_cfg {
    void (*rcc_enable)(void);
    TIM_HandleTypeDef *tim_handle;
    tim_pwm_gpio_cfg_type *tim_pwm_pin;
    TIM_OC_InitTypeDef *tim_pwm_ch_handle;
    uint32_t Channel;
    int id;
} tim_pwm_cfg_type;

int tim_pwm_register(tim_pwm_cfg_type *p_cfg, int num);
int set_pwm_compompare(TIM_PWM_CHx_type timx, uint32_t compare);

#ifdef __cplusplus
}
#endif
#endif
