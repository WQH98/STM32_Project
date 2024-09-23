#ifndef __WWDG_H
#define __WWDG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

void wwdg_init(uint8_t tr,uint8_t wr,uint32_t fprer);


#ifdef __cplusplus
}
#endif
#endif
