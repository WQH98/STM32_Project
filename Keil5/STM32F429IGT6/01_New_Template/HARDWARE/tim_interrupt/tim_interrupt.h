#ifndef __TIM_H
#define __TIM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

typedef struct tim_interrupt_cfg {
    TIM_HandleTypeDef *tim_handle;
    void (*rcc_enable)(void);
    IRQn_Type IRQn;
    uint32_t arr;
    uint32_t PreemptPriority;
    uint32_t SubPriority;
} tim_interrupt_cfg_type;

int tim_interrupt_register(tim_interrupt_cfg_type *p_cfg, int num);

#ifdef __cplusplus
}
#endif
#endif
