#ifndef __USART_H
#define __USART_H

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "stm32f1xx_hal.h"

extern UART_HandleTypeDef USART1_Handler;
extern UART_HandleTypeDef USART3_Handler;
extern uint16_t USART1_RX_STA;
extern uint16_t USART3_RX_STA;
extern char USART3_RX_STA_SIGN;

extern uint8_t USART1_RX_BUF[200];
extern uint8_t USART3_RX_BUF[200];

void USART1_Init(uint32_t baud_rate);
void USART3_Init(uint32_t baud_rate);
void u3_printf(char* fmt,...);

#endif
