#ifndef __UART_H
#define __UART_H

#include "stm32f1xx_hal.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "stdbool.h"

#define UART1_TX_PORT  GPIOA
#define UART1_TX_PIN   GPIO_PIN_9
#define UART1_RX_PORT  GPIOA
#define UART1_RX_PIN   GPIO_PIN_10
#define UART2_TX_PORT  GPIOA
#define UART2_TX_PIN   GPIO_PIN_2
#define UART2_RX_PORT  GPIOA
#define UART2_RX_PIN   GPIO_PIN_3

extern UART_HandleTypeDef uart1_handler;
extern UART_HandleTypeDef uart2_handler;

typedef struct RX_DATA_Type {
    uint8_t buf[256];
    uint16_t len;
    bool finish;
}RX_DATA_Type;

extern RX_DATA_Type uart2_data;

void uart1_init(uint32_t baud);
void uart2_init(uint32_t baud);
void my_printf(UART_HandleTypeDef *huart, const char* format, ...);

#endif
