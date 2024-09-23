#ifndef __CLOCK_H
#define __CLOCK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"


void stm32_clock_init(void);
void delay_us(uint32_t cnt);
void delay_ms(uint32_t cnt);


#ifdef __cplusplus
}
#endif
#endif
