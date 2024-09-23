#ifndef __UART_CFG_H
#define __UART_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "uart.h"

static void uart1_rcc_enable(void)
{
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
}

static void uart3_rcc_enable(void)
{
    __HAL_RCC_USART3_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
}

static void uart1_dma_rcc_enable(void) 
{
    __HAL_RCC_DMA2_CLK_ENABLE();
}


static GPIO_InitTypeDef uart1_tx_cfg = {
    .Alternate = GPIO_AF7_USART1,
    .Mode = GPIO_MODE_AF_PP,
    .Pin = GPIO_PIN_9,
    .Pull = GPIO_PULLUP,
    .Speed = GPIO_SPEED_FAST
};

static uart_gpioinit_cfg_type uart1_tx_gpioinit_cfg = {
    .gpio_cfg = &uart1_tx_cfg,
    .port = GPIOA
};

static GPIO_InitTypeDef uart1_rx_cfg = {
    .Alternate = GPIO_AF7_USART1,
    .Mode = GPIO_MODE_AF_PP,
    .Pin = GPIO_PIN_10,
    .Pull = GPIO_PULLUP,
	.Speed = GPIO_SPEED_FAST
};

static uart_gpioinit_cfg_type uart1_rx_gpioinit_cfg = {
    .gpio_cfg = &uart1_rx_cfg,
    .port = GPIOA
};


static GPIO_InitTypeDef uart3_tx_cfg = {
    .Alternate = GPIO_AF7_USART3,
    .Mode = GPIO_MODE_AF_PP,
    .Pin = GPIO_PIN_10,
    .Pull = GPIO_PULLUP,
    .Speed = GPIO_SPEED_FAST
};

static uart_gpioinit_cfg_type uart3_tx_gpioinit_cfg = {
    .gpio_cfg = &uart3_tx_cfg,
    .port = GPIOB
};

static GPIO_InitTypeDef uart3_rx_cfg = {
    .Alternate = GPIO_AF7_USART3,
    .Mode = GPIO_MODE_AF_PP,
    .Pin = GPIO_PIN_11,
    .Pull = GPIO_PULLUP,
	.Speed = GPIO_SPEED_FAST
};

static uart_gpioinit_cfg_type uart3_rx_gpioinit_cfg = {
    .gpio_cfg = &uart3_rx_cfg,
    .port = GPIOB
};

static UART_HandleTypeDef usart1_handle = {
    .Instance = USART1,
    .Init.BaudRate = 115200,
    .Init.HwFlowCtl = UART_HWCONTROL_NONE,
    .Init.Mode = UART_MODE_TX_RX,
    .Init.Parity = UART_PARITY_NONE,
    .Init.StopBits = UART_STOPBITS_1,
    .Init.WordLength = UART_WORDLENGTH_8B
};

static UART_HandleTypeDef usart3_handle = {
    .Instance = USART3,
    .Init.BaudRate = 115200,
    .Init.HwFlowCtl = UART_HWCONTROL_NONE,
    .Init.Mode = UART_MODE_TX_RX,
    .Init.Parity = UART_PARITY_NONE,
    .Init.StopBits = UART_STOPBITS_1,
    .Init.WordLength = UART_WORDLENGTH_8B
};

static DMA_HandleTypeDef uart1_tx_dma_handle = {
    .Instance = DMA2_Stream7,     // 数据流选择 DMA2数据流7
    .Init.Channel = DMA_CHANNEL_4,    // 通道选择 通道4
    .Init.Direction = DMA_MEMORY_TO_PERIPH,     // 方向选择内存到外设
    .Init.PeriphInc = DMA_PINC_DISABLE,        // 外设非增量模式
    .Init.MemInc = DMA_MINC_ENABLE,          // 内存增量模式
    .Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,   // 外设数据长度 8字节
    .Init.MemDataAlignment = DMA_MDATAALIGN_BYTE,    // 内存数据长度 8字节
    .Init.Mode = DMA_NORMAL,           // 外设普通模式
    .Init.Priority = DMA_PRIORITY_MEDIUM,      // 中等优先级
    .Init.FIFOMode = DMA_FIFOMODE_DISABLE,     // 不使用FIFO队列
    .Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL,   
    .Init.MemBurst = DMA_MBURST_SINGLE,      // 存储器突发单次传输
    .Init.PeriphBurst = DMA_PBURST_SINGLE      // 外设突发单次传输
};

static DMA_HandleTypeDef uart1_rx_dma_handle = {
    .Instance = DMA2_Stream5,     // 数据流选择 DMA2数据流5
    .Init.Channel = DMA_CHANNEL_4,    // 通道选择 通道4
    .Init.Direction = DMA_PERIPH_TO_MEMORY,     // 方向选择内存到外设
    .Init.PeriphInc = DMA_PINC_DISABLE,        // 外设非增量模式
    .Init.MemInc = DMA_MINC_ENABLE,          // 内存增量模式
    .Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,   // 外设数据长度 8字节
    .Init.MemDataAlignment = DMA_MDATAALIGN_BYTE,    // 内存数据长度 8字节
    .Init.Mode = DMA_NORMAL,           // 外设普通模式
    .Init.Priority = DMA_PRIORITY_MEDIUM,      // 中等优先级
    .Init.FIFOMode = DMA_FIFOMODE_DISABLE,     // 不使用FIFO队列
    .Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL,   
    .Init.MemBurst = DMA_MBURST_SINGLE,      // 存储器突发单次传输
    .Init.PeriphBurst = DMA_PBURST_SINGLE,      // 外设突发单次传输
};

static uart_cfg_type uart_cfg_info[] = {
    {
        .uart_handle = &usart1_handle,
        .rcc_enable = uart1_rcc_enable,
        .uart_rx_pin = &uart1_rx_gpioinit_cfg,
        .uart_tx_pin = &uart1_tx_gpioinit_cfg,
        .IRQn = USART1_IRQn,
        .PreemptPriority = 3,
        .SubPriority = 3,
        .id = 0,
        .rx_enable = 1,
        .idle_enable = 1,
        .dma_rcc_enable = uart1_dma_rcc_enable,
        .dma_tx_enable = 1,
        .dma_tx_handle = &uart1_tx_dma_handle,
        .dma_rx_enable = 0,
        .dma_rx_handle = &uart1_rx_dma_handle
    },
    {
        .uart_handle = &usart3_handle,
        .rcc_enable = uart3_rcc_enable,
        .uart_rx_pin = &uart3_rx_gpioinit_cfg,
        .uart_tx_pin = &uart3_tx_gpioinit_cfg,
        .IRQn = USART3_IRQn,
        .PreemptPriority = 2,
        .SubPriority = 3,
        .id = 1,
        .rx_enable = 1,
        .idle_enable = 1,
        .dma_tx_enable = 0,
        .dma_tx_handle = NULL,
        .dma_rcc_enable = NULL,
        .dma_rx_enable = 0,
        .dma_rx_handle = NULL
    }
};

static void uart_init(void) {
    uart_register(uart_cfg_info, sizeof(uart_cfg_info) / sizeof(uart_cfg_info[0]));
}

#ifdef __cplusplus
}
#endif
#endif
