#include "dma.h"
#include "uart.h"

DMA_HandleTypeDef dma2_stream7_handle;
DMA_HandleTypeDef dma2_stream5_handle;
DMA_HandleTypeDef dma1_stream1_handle;

extern UART_HandleTypeDef usart1_handler;
extern UART_HandleTypeDef usart3_handler;

void uart1_tx_dma_init(void) {
    __HAL_RCC_DMA2_CLK_ENABLE();
	
    dma2_stream7_handle.Instance = DMA2_Stream7;
    dma2_stream7_handle.Init.Channel = DMA_CHANNEL_4;
    dma2_stream7_handle.Init.Direction = DMA_MEMORY_TO_PERIPH;
    dma2_stream7_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    dma2_stream7_handle.Init.MemBurst = DMA_MBURST_SINGLE;
    dma2_stream7_handle.Init.MemInc = DMA_MINC_ENABLE;
    dma2_stream7_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    dma2_stream7_handle.Init.Mode = DMA_NORMAL;
    dma2_stream7_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;
    dma2_stream7_handle.Init.PeriphInc = DMA_PINC_DISABLE;
    dma2_stream7_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    dma2_stream7_handle.Init.Priority = DMA_PRIORITY_VERY_HIGH;
    HAL_DMA_DeInit(&dma2_stream7_handle);
	HAL_DMA_Init(&dma2_stream7_handle);

    HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

	__HAL_LINKDMA(&usart1_handler, hdmatx, dma2_stream7_handle);
}

void uart1_rx_dma_init(void) {
    __HAL_RCC_DMA2_CLK_ENABLE();

    dma2_stream5_handle.Instance = DMA2_Stream5;
    dma2_stream5_handle.Init.Channel = DMA_CHANNEL_4;
    dma2_stream5_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
    dma2_stream5_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    dma2_stream5_handle.Init.MemBurst = DMA_MBURST_SINGLE;
    dma2_stream5_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    dma2_stream5_handle.Init.MemInc = DMA_MINC_ENABLE;
    dma2_stream5_handle.Init.Mode = DMA_NORMAL;
    dma2_stream5_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;
    dma2_stream5_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    dma2_stream5_handle.Init.PeriphInc = DMA_PINC_DISABLE;
    dma2_stream5_handle.Init.Priority = DMA_PRIORITY_MEDIUM;
	HAL_DMA_DeInit(&dma2_stream5_handle);
    HAL_DMA_Init(&dma2_stream5_handle);
    
    HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);
	
	__HAL_LINKDMA(&usart1_handler, hdmarx, dma2_stream5_handle);
}

void uart3_rx_dma_init(void) {
    __HAL_RCC_DMA1_CLK_ENABLE();

    dma1_stream1_handle.Instance = DMA1_Stream1;
    dma1_stream1_handle.Init.Channel = DMA_CHANNEL_4;
    dma1_stream1_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
    dma1_stream1_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    dma1_stream1_handle.Init.MemBurst = DMA_MBURST_SINGLE;
    dma1_stream1_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    dma1_stream1_handle.Init.MemInc = DMA_MINC_ENABLE;
    dma1_stream1_handle.Init.Mode = DMA_NORMAL;
    dma1_stream1_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;
    dma1_stream1_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    dma1_stream1_handle.Init.PeriphInc = DMA_PINC_DISABLE;
    dma1_stream1_handle.Init.Priority = DMA_PRIORITY_MEDIUM;
	HAL_DMA_DeInit(&dma1_stream1_handle);
    HAL_DMA_Init(&dma1_stream1_handle);
    
    HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
	
	__HAL_LINKDMA(&usart3_handler, hdmarx, dma1_stream1_handle);
}

void DMA2_Stream7_IRQHandler(void) {
	HAL_DMA_IRQHandler(&dma2_stream7_handle);
}

void DMA2_Stream5_IRQHandler(void) {
	HAL_DMA_IRQHandler(&dma2_stream5_handle);
}

void DMA1_Stream1_IRQHandler(void) {
	HAL_DMA_IRQHandler(&dma1_stream1_handle);
}


