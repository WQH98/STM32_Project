#ifndef __TIM_H
#define __TIM_H

#include "stm32f1xx_hal.h"

#define TIM1_CH1_PORT  GPIOA
#define TIM1_CH1_PIN   GPIO_PIN_8
#define TIM4_CH1_PORT  GPIOB
#define TIM4_CH1_PIN   GPIO_PIN_6
#define TIM4_CH2_PORT  GPIOB
#define TIM4_CH2_PIN   GPIO_PIN_7


void tim1_ch1_init(uint16_t arr, uint16_t psc);
void tim4_ch1_init(uint16_t arr, uint16_t psc);
void tim4_ch2_init(uint16_t arr, uint16_t psc);
void set_tim_compare(TIM_TypeDef *timx, uint8_t channelx, uint32_t compare);


#endif
