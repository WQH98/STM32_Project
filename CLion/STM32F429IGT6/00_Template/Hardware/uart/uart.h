#ifndef __UART_H
#define __UART_H

#include "stm32f4xx_hal.h"
#include "stdbool.h"

#define UART1_TX_PORT GPIOA
#define UART1_TX_PIN  GPIO_PIN_9
#define UART1_RX_PORT GPIOA
#define UART1_RX_PIN  GPIO_PIN_10

extern UART_HandleTypeDef uart1_handler;

typedef struct RX_DATA_Type {
    uint8_t buf[256];
    uint16_t len;
    bool finish;
}RX_DATA_Type;

extern RX_DATA_Type uart1_data;

void uart1_init(uint32_t baud);

#endif
