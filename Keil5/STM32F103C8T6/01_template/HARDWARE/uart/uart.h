#ifndef __UART_H
#define __UART_H


#include "stm32f1xx_hal.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

/**************UART1*****************/
#define USART1_TX_PORT  GPIOA
#define USART1_TX_PIN   GPIO_PIN_9
#define USART1_RX_PORT  GPIOA
#define USART1_RX_PIN   GPIO_PIN_10

/**************UART2*****************/
#define USART2_TX_PORT  GPIOA
#define USART2_TX_PIN   GPIO_PIN_2
#define USART2_RX_PORT  GPIOA
#define USART2_RX_PIN   GPIO_PIN_3

/**************UART3*****************/
#define USART3_TX_PORT  GPIOB
#define USART3_TX_PIN   GPIO_PIN_10
#define USART3_RX_PORT  GPIOB
#define USART3_RX_PIN   GPIO_PIN_11

extern UART_HandleTypeDef usart1_hander;
extern UART_HandleTypeDef usart2_hander;
extern UART_HandleTypeDef usart3_hander;

typedef struct uart_rx_data {
    uint8_t rx_buf[256];
    uint16_t rx_len;
    bool rx_finsh;
} uart_rx_data_type;

extern uart_rx_data_type uart1_message;
extern uart_rx_data_type uart2_message;
extern uart_rx_data_type uart3_message;

void uart1_init(uint32_t baud);
void uart2_init(uint32_t baud);
void uart3_init(uint32_t baud);

void u2_printf(char* fmt,...);
void u3_printf(char* fmt,...);

void uart1_clear_message(void);
void uart2_clear_message(void);
void uart3_clear_message(void);

#endif
