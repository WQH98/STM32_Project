#include "uart.h"
#include "stdlib.h"


UART_HandleTypeDef usart1_handler;
UART_HandleTypeDef usart3_handler;

uart_rec_msg_t uart1_msg;
uart_rec_msg_t uart3_msg;


void usart1_init(uint32_t baudrate)
{
    GPIO_InitTypeDef gpio_handler;

    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
	
    usart1_handler.Instance = USART1;
    usart1_handler.Init.BaudRate = baudrate;
    usart1_handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    usart1_handler.Init.Mode = UART_MODE_TX_RX;
    usart1_handler.Init.OverSampling = UART_OVERSAMPLING_16;
    usart1_handler.Init.Parity = UART_PARITY_NONE;
    usart1_handler.Init.StopBits = UART_STOPBITS_1;
    usart1_handler.Init.WordLength = UART_WORDLENGTH_8B;
    HAL_UART_Init(&usart1_handler);

    gpio_handler.Alternate = GPIO_AF7_USART1;
    gpio_handler.Mode = GPIO_MODE_AF_PP;
    gpio_handler.Pin = USART1_TX_PIN;
    gpio_handler.Pull = GPIO_PULLUP;
    gpio_handler.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(USART1_TX_PORT, &gpio_handler);

    gpio_handler.Pin = USART1_RX_PIN;
    HAL_GPIO_Init(USART1_RX_PORT, &gpio_handler);

    __HAL_UART_ENABLE_IT(&usart1_handler, UART_IT_IDLE);
	__HAL_UART_ENABLE_IT(&usart1_handler, UART_IT_RXNE);

	HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);   

}

void usart3_init(uint32_t baudrate) {
    GPIO_InitTypeDef gpio_handler;

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_USART3_CLK_ENABLE();

    usart3_handler.Instance = USART3;
    usart3_handler.Init.BaudRate = baudrate;
    usart3_handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    usart3_handler.Init.Mode = UART_MODE_TX_RX;
    usart3_handler.Init.OverSampling = UART_OVERSAMPLING_16;
    usart3_handler.Init.Parity = UART_PARITY_NONE;
    usart3_handler.Init.StopBits = UART_STOPBITS_1;
    usart3_handler.Init.WordLength = UART_WORDLENGTH_8B;
    HAL_UART_Init(&usart3_handler);

    gpio_handler.Alternate = GPIO_AF7_USART3;
    gpio_handler.Mode = GPIO_MODE_AF_PP;
    gpio_handler.Pin = USART3_TX_PIN;
    gpio_handler.Pull = GPIO_PULLUP;
    gpio_handler.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(USART3_TX_PORT, &gpio_handler);

    gpio_handler.Pin = USART3_RX_PIN;
    HAL_GPIO_Init(USART3_RX_PORT, &gpio_handler);

    __HAL_UART_ENABLE_IT(&usart3_handler, UART_IT_IDLE);
	__HAL_UART_ENABLE_IT(&usart3_handler, UART_IT_RXNE);

	HAL_NVIC_SetPriority(USART3_IRQn, 3, 1);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
}


void USART1_IRQHandler(void) {
	uint8_t receive;
	if(__HAL_UART_GET_FLAG(&usart1_handler, UART_FLAG_RXNE) == SET) {
		HAL_UART_Receive(&usart1_handler, &receive, 1, 1000);
		uart1_msg.data[uart1_msg.len] = receive;
		uart1_msg.len++;
		if(uart1_msg.len >= 255) uart1_msg.len = 0;
		__HAL_UART_CLEAR_FLAG(&usart1_handler, UART_FLAG_RXNE);
	}
	if(__HAL_UART_GET_FLAG(&usart1_handler, UART_FLAG_IDLE) == SET) {
		uart1_msg.data[uart1_msg.len] = '\0';
		uart1_msg.finish = 1;
		__HAL_UART_CLEAR_IDLEFLAG(&usart1_handler);
	}
}

void USART3_IRQHandler(void) {
    uint8_t receive;
	if(__HAL_UART_GET_FLAG(&usart3_handler, UART_FLAG_RXNE) == SET) {
		HAL_UART_Receive(&usart3_handler, &receive, 1, 1000);
		uart3_msg.data[uart3_msg.len] = receive;
		uart3_msg.len++;
		if(uart3_msg.len >= 255) uart3_msg.len = 0;
		__HAL_UART_CLEAR_FLAG(&usart3_handler, UART_FLAG_RXNE);
	}
	if(__HAL_UART_GET_FLAG(&usart3_handler, UART_FLAG_IDLE) == SET) {
		uart3_msg.data[uart3_msg.len] = '\0';
		uart3_msg.finish = 1;
		__HAL_UART_CLEAR_IDLEFLAG(&usart3_handler);
	}
}


// printf重定向代码 修改其底层fputc
#if 1

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
