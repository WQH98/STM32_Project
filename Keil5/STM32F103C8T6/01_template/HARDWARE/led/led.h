#ifndef __LED_H
#define __LED_H

#include "stm32f1xx_hal.h"

#define GREEN_PORT  GPIOB
#define GREEN_PIN   GPIO_PIN_0
#define WHITE_PORT  GPIOB
#define WHITE_PIN   GPIO_PIN_1


void led_init(void);
void greenled_on(void);
void greenled_off(void);
void greenled_toggle(void);
void whiteled_on(void);
void whiteled_off(void);
void whiteled_toggle(void);


#endif
