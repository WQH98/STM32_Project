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

#define RX_LEN_MAX        110 * 1024

typedef struct uart_rec_msg {
    uint8_t data[RX_LEN_MAX];
    uint16_t len;
    bool finish;
} uart_rec_msg_t;

extern uart_rec_msg_t uart1_msg;

void usart1_init(uint32_t baudrate);

#ifdef __cplusplus
}
#endif
#endif
