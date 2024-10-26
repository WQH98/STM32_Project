#ifndef __LED_H
#define __LED_H

#include "stm32f1xx_hal.h"

#define LED1_PORT GPIOD
#define LED1_PIN  GPIO_PIN_13
#define LED2_PORT GPIOD
#define LED2_PIN  GPIO_PIN_11
#define LED3_PORT GPIOD
#define LED3_PIN  GPIO_PIN_9
#define LED4_PORT GPIOB
#define LED4_PIN  GPIO_PIN_15
#define LED5_PORT GPIOB
#define LED5_PIN  GPIO_PIN_13
#define LED6_PORT GPIOB
#define LED6_PIN  GPIO_PIN_11
#define LED7_PORT GPIOE
#define LED7_PIN  GPIO_PIN_15
#define LED8_PORT GPIOE
#define LED8_PIN  GPIO_PIN_13


#define LED1   LED1_PORT, LED1_PIN
#define LED2   LED2_PORT, LED2_PIN
#define LED3   LED3_PORT, LED3_PIN
#define LED4   LED4_PORT, LED4_PIN
#define LED5   LED5_PORT, LED5_PIN
#define LED6   LED6_PORT, LED6_PIN
#define LED7   LED7_PORT, LED7_PIN
#define LED8   LED8_PORT, LED8_PIN

enum LED_STATUS {
    LED_ON,
    LED_OFF,
    LED_TORRLE
};


void led_init(void);
void led_switch(GPIO_TypeDef *GPIOx, uint32_t Pin, int status);
void led4_on(void);
void led4_off(void);


#endif
