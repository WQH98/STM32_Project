#ifndef __DMA_H
#define __DMA_H

#include "stm32f4xx_hal.h"

#define BUFFER_SIZE   32

extern const uint32_t aSRC_Const_Buffer[BUFFER_SIZE];
extern uint32_t aDST_Buffer[BUFFER_SIZE];

extern DMA_HandleTypeDef dma2_stream0_handle;
extern DMA_HandleTypeDef dma2_stream7_handle;

void dma2_stream0_init(void);
uint8_t buffer_cmp(const uint32_t *p_src_buffer, uint32_t *p_dst_buffer, uint16_t size);
void dma2_stream7_init(void);

#endif
