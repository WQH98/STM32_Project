#ifndef __UART_H
#define __UART_H
#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "stdio.h"

#define USART1_TX_PORT    GPIOA
#define USART1_TX_PIN     GPIO_PIN_2
#define USART1_RX_PORT    GPIOA
#define USART1_RX_PIN     GPIO_PIN_3

void usart1_init(uint32_t baudrate);

#ifdef __cplusplus
}
#endif
#endif
