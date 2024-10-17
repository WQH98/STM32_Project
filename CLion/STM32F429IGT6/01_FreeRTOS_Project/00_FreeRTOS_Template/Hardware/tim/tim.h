#ifndef __TIM_H
#define __TIM_H

#include "stm32f4xx_hal.h"
#include "led.h"

void tim3_init(uint16_t arr, uint16_t psc);

#endif
