#ifndef __USART_H
#define __USART_H

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include "stm32f1xx_hal.h"

extern UART_HandleTypeDef USART1_Handler;
extern UART_HandleTypeDef USART3_Handler;

extern uint16_t USART1_RX_STA;
extern uint8_t USART1_RX_BUF[200];

typedef struct
{
    uint8_t rx_buf[200];
    uint16_t rx_len;
    bool rx_finsh;
} USART3_Data;

extern USART3_Data usart3_message;

void USART1_Init(uint32_t baud_rate);
void USART3_Init(uint32_t baud_rate);
void u3_printf(char* fmt,...);

#endif
