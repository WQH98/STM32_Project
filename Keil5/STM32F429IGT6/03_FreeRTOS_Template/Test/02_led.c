#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "uart.h"

TaskHandle_t led_task_handler;

void led_task(void *pvParameters) {
    led_init();
    led0_on();
    led1_off();
    while(1) {
        printf("this is led task\r\n");
        led0_toggle();
        led1_toggle();
        vTaskDelay(1000);
    }
}
void led_test(void) {
    BaseType_t x_return = pdPASS;
    x_return = xTaskCreate(led_task, "led_task", 512, NULL, 2, &led_task_handler);
    if(pdPASS == x_return) {
        printf("create led task success\r\n");
    }
}


