#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "uart.h"
#include "iwdg.h"
#include "key.h"

TaskHandle_t iwdg_task_handler;

void iwdg_task(void *pvParameters) {
    vTaskDelay(500);
    led0_on();
    iwdg_init(IWDG_PRESCALER_64, 500);
    while(1) {
        if(1 == key_scan(KEY0_PORT, KEY0_PIN, GPIO_PIN_RESET)) {
            iwdg_feed();
        }
        vTaskDelay(10);
    }
}
void iwdg_test(void) {
    key_init();
    led0_off();
    BaseType_t x_return = pdPASS;
    x_return = xTaskCreate(iwdg_task, "iwdg_task", 512, NULL, 2, &iwdg_task_handler);
    if(pdPASS == x_return) {
        printf("create iwdg task success\r\n");
    }
}


