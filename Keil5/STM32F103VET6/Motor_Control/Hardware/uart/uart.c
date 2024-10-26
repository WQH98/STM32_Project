#include "uart.h"
#include "shell.h"

UART_HandleTypeDef uart1_handler;
UART_HandleTypeDef uart2_handler;

RX_DATA_Type uart2_data = {{0}, 0, false};


void uart1_init(uint32_t baud) {
    uart1_handler.Instance = USART1;
    uart1_handler.Init.BaudRate = baud;
    uart1_handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    uart1_handler.Init.Mode = UART_MODE_TX_RX;
    uart1_handler.Init.OverSampling = UART_OVERSAMPLING_16;
    uart1_handler.Init.Parity = UART_PARITY_NONE;
    uart1_handler.Init.StopBits = UART_STOPBITS_1;
    uart1_handler.Init.WordLength = UART_WORDLENGTH_8B;
    HAL_UART_Init(&uart1_handler);   
}


void uart2_init(uint32_t baud) {
    uart2_handler.Instance = USART2;
    uart2_handler.Init.BaudRate = baud;
    uart2_handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    uart2_handler.Init.Mode = UART_MODE_TX_RX;
    uart2_handler.Init.OverSampling = UART_OVERSAMPLING_16;
    uart2_handler.Init.Parity = UART_PARITY_NONE;
    uart2_handler.Init.StopBits = UART_STOPBITS_1;
    uart2_handler.Init.WordLength = UART_WORDLENGTH_8B;
    HAL_UART_Init(&uart2_handler);  
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    GPIO_InitTypeDef gpio_handler;
    if(huart->Instance == USART1) {
        
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_USART1_CLK_ENABLE();

        gpio_handler.Mode = GPIO_MODE_AF_PP;
        gpio_handler.Pin = UART1_TX_PIN;
        gpio_handler.Pull = GPIO_NOPULL;
        gpio_handler.Speed = GPIO_SPEED_HIGH;
        HAL_GPIO_Init(UART1_TX_PORT, &gpio_handler);

        gpio_handler.Pin = UART1_RX_PIN;
        HAL_GPIO_Init(UART1_RX_PORT, &gpio_handler);
    }
	
	if(huart->Instance == USART2) {
        
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_USART2_CLK_ENABLE();

        gpio_handler.Mode = GPIO_MODE_AF_PP;
        gpio_handler.Pin = UART2_TX_PIN;
        gpio_handler.Pull = GPIO_NOPULL;
        gpio_handler.Speed = GPIO_SPEED_HIGH;
        HAL_GPIO_Init(UART2_TX_PORT, &gpio_handler);

        gpio_handler.Pin = UART2_RX_PIN;
        HAL_GPIO_Init(UART2_RX_PORT, &gpio_handler);
		
		__HAL_UART_ENABLE_IT(&uart2_handler, UART_IT_RXNE);
        __HAL_UART_ENABLE_IT(&uart2_handler, UART_IT_IDLE);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
        HAL_NVIC_SetPriority(USART2_IRQn, 1, 2);
    }
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

void USART2_IRQHandler() {
    HAL_UART_IRQHandler(&uart2_handler);
    uint8_t rec;
    if(__HAL_UART_GET_FLAG(&uart2_handler, UART_FLAG_RXNE) == SET) {
        HAL_UART_Receive(&uart2_handler, &rec, 1, 1000);
        uart2_data.buf[uart2_data.len] = rec;
        uart2_data.len++;
        if(uart2_data.len >= 256) uart2_data.len = 0;
        __HAL_UART_CLEAR_FLAG(&uart2_handler, UART_FLAG_RXNE);
    }
    if(__HAL_UART_GET_FLAG(&uart2_handler, UART_FLAG_IDLE) == SET) {
		uart2_data.buf[uart2_data.len] = '\0';
        uart2_data.finish = true;
		
		for(uint8_t i = 0; i < uart2_data.len; i++) {
			shellHandler(&shell, (char)uart2_data.buf[i]);
		}
		uart2_data.len = 0;
        __HAL_UART_CLEAR_IDLEFLAG(&uart2_handler);
    }
}


