#include "uart_cfg.h"
#include "string.h"

static uart_cfg_type *g_uart_cfg = NULL;
uart_rx_data_type uart1_message;
uart_rx_data_type uart3_message;

// printf重定向代码 修改其底层fputc
#if 1
#include "stdio.h"
// 告知连接器不从C库链接使用半主机的函数
#pragma import(__use_no_semihosting)

// 定义 _sys_exit() 以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}

// 标准库需要的支持类型
struct __FILE
{
    int handle;
};

FILE __stdout;

int fputc(int ch, FILE *stream)
{
    // 堵塞判断串口是否发送完成
    // 不同芯片的串口标志位不一定相同
    while((USART1->SR & 0x40) == 0);
    // 串口发送完成 将该字符发送
    USART1->DR = (uint8_t)ch;
    return ch;
}
#endif

void uart_send(int id, char *pData)
{
    if(id != g_uart_cfg[id].id) {
        return;
    }
    HAL_UART_Transmit(g_uart_cfg[id].uart_handle, (const uint8_t*)pData, strlen((const char*)pData), 2000);
}

int uart_register(uart_cfg_type *p_cfg, int num)
{
    int i = 0;
    g_uart_cfg = p_cfg;
	if(NULL == p_cfg) {
		return -1;
	}

    for(i = 0; i < num; i++) {
        g_uart_cfg[i].rcc_enable();
        HAL_UART_Init(g_uart_cfg[i].uart_handle);
        HAL_GPIO_Init(g_uart_cfg[i].uart_tx_pin->port, g_uart_cfg[i].uart_tx_pin->gpio_cfg);
        HAL_GPIO_Init(g_uart_cfg[i].uart_rx_pin->port, g_uart_cfg[i].uart_rx_pin->gpio_cfg);
        if(g_uart_cfg[i].rx_enable) 
        {
            __HAL_UART_ENABLE_IT(g_uart_cfg[i].uart_handle, UART_IT_RXNE);
        }
        if(g_uart_cfg[i].idle_enable) 
        {
            __HAL_UART_ENABLE_IT(g_uart_cfg[i].uart_handle, UART_IT_IDLE);
        }
        if(g_uart_cfg[i].rx_enable || g_uart_cfg[i].idle_enable) 
        {
            HAL_NVIC_EnableIRQ(g_uart_cfg[i].IRQn);
            HAL_NVIC_SetPriority(g_uart_cfg[i].IRQn, g_uart_cfg[i].PreemptPriority, g_uart_cfg[i].SubPriority);
        }
        if(g_uart_cfg[i].dma_tx_enable) 
        {
            g_uart_cfg[i].dma_rcc_enable();
            HAL_DMA_Init(g_uart_cfg[i].dma_tx_handle);
            __HAL_LINKDMA(g_uart_cfg[i].uart_handle, hdmatx, *g_uart_cfg[i].dma_tx_handle);
            HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, g_uart_cfg[i].PreemptPriority, g_uart_cfg[i].SubPriority);
            HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
        }
        if(g_uart_cfg[i].dma_rx_enable) 
        {
            g_uart_cfg[i].dma_rcc_enable();
            HAL_DMA_Init(g_uart_cfg[i].dma_rx_handle);
            __HAL_LINKDMA(g_uart_cfg[i].uart_handle, hdmarx, *g_uart_cfg[i].dma_rx_handle);
            HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, g_uart_cfg[i].PreemptPriority, g_uart_cfg[i].SubPriority);
            HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);
            HAL_UART_Receive_DMA(g_uart_cfg->uart_handle, g_uart_cfg[i].rx_buf, 256);
        }
    }

    return 0;
}

int uart_clear(uart_rx_data_type *data) 
{
    
    if(NULL == data) {
        return -1;
    }
    
    data->rx_finsh = 0;
    data->rx_len = 0;
    memset(data->rx_buf, 0, 256);

    return 0;
}


void USART1_IRQHandler(void) 
{
    uint8_t receive;
    if(g_uart_cfg[COM1].dma_rx_enable) 
    {
        if(__HAL_UART_GET_FLAG(g_uart_cfg[COM1].uart_handle, UART_FLAG_IDLE) == SET) 
        {
            __HAL_UART_CLEAR_IDLEFLAG(g_uart_cfg[COM1].uart_handle);
            HAL_UART_DMAStop(g_uart_cfg[COM1].uart_handle);
            g_uart_cfg[COM1].rx_len = 256 - __HAL_DMA_GET_COUNTER(g_uart_cfg[COM1].dma_rx_handle);
            g_uart_cfg[COM1].rx_finish = 1;
        }
    }
    else 
    {
        if(__HAL_UART_GET_FLAG(g_uart_cfg[COM1].uart_handle, UART_FLAG_RXNE) == SET)
        {
            HAL_UART_Receive(g_uart_cfg[COM1].uart_handle, &receive, 1, 1000);
            uart1_message.rx_buf[uart1_message.rx_len] = receive;
            uart1_message.rx_len++;
            if(uart1_message.rx_len >= 256) uart1_message.rx_len = 0;
            __HAL_UART_CLEAR_FLAG(g_uart_cfg[COM1].uart_handle, UART_FLAG_RXNE);
        }
        if(__HAL_UART_GET_FLAG(g_uart_cfg[COM1].uart_handle, UART_FLAG_IDLE) == SET) 
        {
            uart1_message.rx_buf[uart1_message.rx_len] = '\0';
            uart1_message.rx_finsh = 1;
            __HAL_UART_CLEAR_IDLEFLAG(g_uart_cfg[COM1].uart_handle);
        }
    }
    
    HAL_UART_IRQHandler(g_uart_cfg[COM1].uart_handle);
}


void USART3_IRQHandler(void) 
{
    uint8_t receive;
    if(__HAL_UART_GET_FLAG(g_uart_cfg[COM3].uart_handle, UART_FLAG_RXNE) == SET)
    {
        HAL_UART_Receive(g_uart_cfg[COM3].uart_handle, &receive, 1, 1000);
        uart3_message.rx_buf[uart3_message.rx_len] = receive;
		uart3_message.rx_len++;
		if(uart3_message.rx_len >= 256) uart3_message.rx_len = 0;
		__HAL_UART_CLEAR_FLAG(g_uart_cfg[COM3].uart_handle, UART_FLAG_RXNE);
    }
	if(__HAL_UART_GET_FLAG(g_uart_cfg[COM3].uart_handle, UART_FLAG_IDLE) == SET) 
	{
		uart3_message.rx_buf[uart3_message.rx_len] = '\0';
        uart3_message.rx_finsh = 1;
        __HAL_UART_CLEAR_IDLEFLAG(g_uart_cfg[COM3].uart_handle);
	}
}

void DMA2_Stream7_IRQHandler(void)
{
    HAL_DMA_IRQHandler(g_uart_cfg[COM1].dma_tx_handle);
}

void DMA2_Stream5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(g_uart_cfg[COM1].dma_rx_handle);
}


int uart_dma_send(int id, uint8_t *pData)
{
    return HAL_UART_Transmit_DMA(g_uart_cfg[id].uart_handle, (uint8_t *)pData, strlen((const char *)pData));
}

int uart_dma_receive(int id, uint8_t *len)
{
    *len = g_uart_cfg[id].rx_len;
    return HAL_UART_Receive_DMA(g_uart_cfg[id].uart_handle, \
                                g_uart_cfg[id].rx_buf,      \
                                *len);
}

uint8_t uart_dma_get_rec_state(int id)
{
    return g_uart_cfg[id].rx_finish;
}

void uart_dma_clear_rec_state(int id)
{
    g_uart_cfg[id].rx_finish = 0;
}

void uart_dma_clear_rx_len(int id)
{
    g_uart_cfg[id].rx_len = 0;
}

void uart_dma_clear_rx_buf(int id)
{
    memset(g_uart_cfg[id].rx_buf, 0, 256);
}

void uart_dma_get_rx_buf(int id, uint8_t *pData)
{
    strcpy((char *)pData, (const char *)g_uart_cfg[id].rx_buf);
}
