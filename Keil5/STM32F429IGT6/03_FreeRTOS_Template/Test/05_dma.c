#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "uart.h"
#include "dma.h"
#include "string.h"

uint8_t data[] = "Hello World\r\n";

TaskHandle_t dma_task_handler;
extern UART_HandleTypeDef usart1_handler;
void dma_task(void *pvParameters) {
    int i = 0;
    uart1_tx_dma_init();
    uart1_rx_dma_init();
	usart1_init(115200);
	HAL_UART_Receive_DMA(&usart1_handler, uart1_msg.data, 256);
    while(1) {
        if(i == 500) {
            i = 0;
            led0_toggle();
        }
        if(uart1_msg.finish == true) {
            uart1_msg.finish = false;
			HAL_UART_Transmit_DMA(&usart1_handler, uart1_msg.data, uart1_msg.len);
            // HAL_UART_Transmit_DMA(&usart1_handler, uart1_msg.data, uart1_msg.len);
            // HAL_UART_Transmit_DMA(&usart1_handler, data, sizeof(data)); 
			// printf("%d\t", uart1_msg.len);
			// printf("%s", uart1_msg.data);
            memset(uart1_msg.data, 0, uart1_msg.len);
			uart1_msg.len = 0;
			led1_toggle();	
			HAL_UART_Receive_DMA(&usart1_handler, uart1_msg.data, 256);
        }

        i++;
        vTaskDelay(1);
    }
}

void dma_test(void) {
    
    BaseType_t x_return = pdPASS;
    x_return = xTaskCreate(dma_task, "dma_task", 512, NULL, 2, &dma_task_handler);
    if(pdPASS == x_return) {
        // printf("create dma task success\r\n");
    }
}


