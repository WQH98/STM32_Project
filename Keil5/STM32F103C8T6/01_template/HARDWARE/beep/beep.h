#ifndef __BEEP_H
#define __BEEP_H

#include "stm32f1xx_hal.h"

#define BEEP_PORT  GPIOB
#define BEEP_PIN   GPIO_PIN_3


void beep_init(void);
void beep_on(void);
void beep_off(void);
void beep_toggle(void);

#endif
