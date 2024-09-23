#ifndef __UART_H
#define __UART_H
#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "stdbool.h"

#define USART1_TX_PORT    GPIOA
#define USART1_TX_PIN     GPIO_PIN_9
#define USART1_RX_PORT    GPIOA
#define USART1_RX_PIN     GPIO_PIN_10

typedef struct uart_rec_msg {
    uint8_t data[256];
    uint8_t len;
    bool finish;
} uart_rec_msg_t;

extern uint8_t rec_data[256];

extern uart_rec_msg_t uart1_msg;

void usart1_init(uint32_t baudrate);

#ifdef __cplusplus
}
#endif
#endif
