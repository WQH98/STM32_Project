#ifndef __LAN8720_H
#define __LAN8720_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "pcf8574.h"
#include "clock.h"

extern ETH_HandleTypeDef ETH_Handler;

HAL_StatusTypeDef lan8720_init(void);

#ifdef __cplusplus
}
#endif

#endif
