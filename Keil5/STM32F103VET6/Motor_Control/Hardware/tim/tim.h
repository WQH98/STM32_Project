#ifndef __TIM_H
#define __TIM_H

#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef tim3_handler;
extern TIM_HandleTypeDef tim2_handler;
extern TIM_HandleTypeDef tim4_pwm_handler;
extern int32_t OverflowCount;

void tim4_ch2_pwm_init(uint32_t arr, uint32_t psc);
void tim3_init(uint16_t arr);
void set_tim4_ch2_compare(uint32_t value);
void out1_pwm(uint32_t count);
void tim2_encode_init(void);

#endif
