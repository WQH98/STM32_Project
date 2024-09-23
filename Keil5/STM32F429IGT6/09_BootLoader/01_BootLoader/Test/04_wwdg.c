#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "uart.h"
#include "wwdg.h"

TaskHandle_t wwdg_task_handler;

void wwdg_task(void *pvParameters) {
    vTaskDelay(500);
    led0_on();
    wwdg_init(0x7F, 0x4F, WWDG_PRESCALER_8);
    while(1) {
		
        vTaskDelay(1);
    }
}

void wwdg_test(void) {
    led0_off();
    BaseType_t x_return = pdPASS;
    x_return = xTaskCreate(wwdg_task, "wwdg_task", 512, NULL, 2, &wwdg_task_handler);
    if(pdPASS == x_return) {
        printf("create wwdg task success\r\n");
    }
}


