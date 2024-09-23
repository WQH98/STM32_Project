#ifndef __LED_H
#define __LED_H

#include "stm32f1xx_hal.h"
//LED初始化
void LED_Init(void);
//红色LED亮
void RedLED_ON(void);
//红色LED灭
void RedLED_OFF(void);
//绿色LED亮
void GreenLED_ON(void);
//绿色LED灭
void GreenLED_OFF(void);
//红色LED反转
void RedLED_Reversal(void);
//绿色LED反转
void GreenLED_Reversal(void);
#endif
