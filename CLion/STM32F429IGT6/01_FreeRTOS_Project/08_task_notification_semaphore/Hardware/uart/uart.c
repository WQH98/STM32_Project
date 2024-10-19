#include "uart.h"

UART_HandleTypeDef uart1_handler;

RX_DATA_Type uart1_data = {{0}, 0, false};

void uart1_init(uint32_t baud) {
    uart1_handler.Instance = USART1;
    uart1_handler.Init.Mode = UART_MODE_TX_RX;
    uart1_handler.Init.BaudRate = baud;
    uart1_handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    uart1_handler.Init.Parity = UART_PARITY_NONE;
    uart1_handler.Init.StopBits = UART_STOPBITS_1;
    uart1_handler.Init.WordLength = UART_WORDLENGTH_8B;
    HAL_UART_Init(&uart1_handler);
}


void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    GPIO_InitTypeDef gpio_handler;

    if(huart->Instance == USART1) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_USART1_CLK_ENABLE();

        gpio_handler.Mode = GPIO_MODE_AF_PP;
        gpio_handler.Speed = GPIO_SPEED_HIGH;
        gpio_handler.Pin = UART1_TX_PIN;
        gpio_handler.Alternate = GPIO_AF7_USART1;
        HAL_GPIO_Init(UART1_TX_PORT, &gpio_handler);

        gpio_handler.Pin = UART1_RX_PIN;
        HAL_GPIO_Init(UART1_RX_PORT, &gpio_handler);

        __HAL_UART_ENABLE_IT(&uart1_handler, UART_IT_RXNE);
        __HAL_UART_ENABLE_IT(&uart1_handler, UART_IT_IDLE);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
        HAL_NVIC_SetPriority(USART1_IRQn, 2, 2);
    }
}

void USART1_IRQHandler() {
    HAL_UART_IRQHandler(&uart1_handler);
    uint8_t rec;
    if(__HAL_UART_GET_FLAG(&uart1_handler, UART_FLAG_RXNE) == SET) {
        HAL_UART_Receive(&uart1_handler, &rec, 1, 1000);
        uart1_data.buf[uart1_data.len] = rec;
        uart1_data.len++;
        if(uart1_data.len >= 256) uart1_data.len = 0;
        __HAL_UART_CLEAR_FLAG(&uart1_handler, UART_FLAG_RXNE);
    }
    if(__HAL_UART_GET_FLAG(&uart1_handler, UART_FLAG_IDLE) == SET) {
        uart1_data.buf[uart1_data.len] = '\0';
        uart1_data.finish = true;
        __HAL_UART_CLEAR_IDLEFLAG(&uart1_handler);
    }
}

// 打印整型函数
// huart: 串口句柄 msg: 打印的信息
void my_printf_d(UART_HandleTypeDef *huart, char *msg, uint16_t num) {
    char log_msg[100];
    sprintf(log_msg, msg, num);
    HAL_UART_Transmit(huart, (uint8_t*)log_msg, strlen(log_msg), 1000);
}

// 打印浮点型函数
// huart: 串口句柄 msg: 打印的信息
void my_printf_f(UART_HandleTypeDef *huart, char *msg, float num) {
    char log_msg[100];
    sprintf(log_msg, msg, num);
    HAL_UART_Transmit(huart, (uint8_t*)log_msg, strlen(log_msg), 1000);
}

void my_printf(UART_HandleTypeDef *huart, const char* format, ...) {
    uint16_t len;
    va_list ap;
    va_start(ap, format);
    uint8_t buf[200];
    vsprintf((char*)buf, format, ap);
    va_end(ap);
    len = strlen((char*)buf);
    HAL_UART_Transmit(huart, buf, len, 1000);
}



