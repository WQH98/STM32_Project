#include "FreeRTOS.h"
#include "task.h"
#include "exti.h"
#include "uart.h"

TaskHandle_t exti_task_handler;

void exti_task(void *pvParameters) {
    exti_init();
    while(1) {
        printf("this is exti task\r\n");
        vTaskDelay(1000);
    }
}
void exti_test(void) {
    BaseType_t x_return = pdPASS;
    x_return = xTaskCreate(exti_task, "exti_task", 512, NULL, 2, &exti_task_handler);
    if(pdPASS == x_return) {
        printf("create exti task success\r\n");
    }
}


