#ifndef __KEY_H
#define __KEY_H

#include "stm32f4xx_hal.h"
#include "stdbool.h"

#define  WK_UP_PORT  GPIOA
#define  WK_UP_PIN   GPIO_PIN_0
#define  KEY0_PORT   GPIOH
#define  KEY0_PIN    GPIO_PIN_3
#define  KEY1_PORT   GPIOH
#define  KEY1_PIN    GPIO_PIN_2
#define  KEY2_PORT   GPIOC
#define  KEY2_PIN    GPIO_PIN_13

void key_init(void);


#endif
