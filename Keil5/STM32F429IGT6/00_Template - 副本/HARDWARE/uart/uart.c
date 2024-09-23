#include "uart.h"

UART_HandleTypeDef usart1_handler;

void usart1_init(uint32_t baudrate)
{
    GPIO_InitTypeDef gpio_handler;

    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    usart1_handler.Instance = USART2;
    usart1_handler.Init.BaudRate = baudrate;
    usart1_handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    usart1_handler.Init.Mode = UART_MODE_TX_RX;
    usart1_handler.Init.OverSampling = UART_OVERSAMPLING_16;
    usart1_handler.Init.Parity = UART_PARITY_NONE;
    usart1_handler.Init.StopBits = UART_STOPBITS_1;
    usart1_handler.Init.WordLength = UART_WORDLENGTH_8B;
    HAL_UART_Init(&usart1_handler);

    gpio_handler.Alternate = GPIO_AF7_USART2;
    gpio_handler.Mode = GPIO_MODE_AF_PP;
    gpio_handler.Pin = USART1_TX_PIN;
    gpio_handler.Pull = GPIO_PULLUP;
    gpio_handler.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(USART1_TX_PORT, &gpio_handler);

    gpio_handler.Pin = USART1_RX_PIN;
    HAL_GPIO_Init(USART1_RX_PORT, &gpio_handler);
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
    while((USART2->SR & 0x40) == 0);
    // 串口发送完成 将该字符发送
    USART2->DR = (uint8_t)ch;
    return ch;
}
#endif
