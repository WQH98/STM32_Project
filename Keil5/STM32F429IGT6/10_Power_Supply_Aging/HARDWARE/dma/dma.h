#ifndef __DMA_H
#define __DMA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

void uart1_tx_dma_init(void);
void uart1_rx_dma_init(void);
void uart3_rx_dma_init(void);


#ifdef __cplusplus
}
#endif
#endif
