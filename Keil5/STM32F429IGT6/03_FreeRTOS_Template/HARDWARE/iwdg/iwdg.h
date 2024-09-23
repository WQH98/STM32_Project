#ifndef __IWDG_H
#define __IWDG_H

#include "stm32f4xx_hal.h"

void iwdg_init(uint8_t prer, uint16_t rlr);
void iwdg_feed(void);

#endif
