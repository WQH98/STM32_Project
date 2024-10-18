#ifndef __LED_H
#define __LED_H

#include "stm32f4xx_hal.h"

#define LED0_PORT GPIOB
#define LED0_PIN  GPIO_PIN_0
#define LED1_PORT GPIOB
#define LED1_PIN  GPIO_PIN_1

void led_init(void);
void led0_on(void);
void led0_off(void);
void led1_on(void);
void led1_off(void);
void led0_reversal(void);
void led1_reversal(void);

#endif
