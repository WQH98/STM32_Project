#include "key.h"

void key_delay(uint16_t i) {
    while(i--);
}


void key_init(void) {
    
    GPIO_InitTypeDef gpio_handle;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    gpio_handle.Mode = GPIO_MODE_INPUT;
    gpio_handle.Pin = WK_UP_PIN;
    gpio_handle.Pull = GPIO_PULLDOWN;
    gpio_handle.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(WK_UP_PORT, &gpio_handle);

    gpio_handle.Mode = GPIO_MODE_INPUT;
    gpio_handle.Pin = KEY0_PIN;
    gpio_handle.Pull = GPIO_PULLUP;
    gpio_handle.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(KEY0_PORT, &gpio_handle);

    gpio_handle.Mode = GPIO_MODE_INPUT;
    gpio_handle.Pin = KEY1_PIN;
    gpio_handle.Pull = GPIO_PULLUP;
    gpio_handle.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(KEY1_PORT, &gpio_handle);

    gpio_handle.Mode = GPIO_MODE_INPUT;
    gpio_handle.Pin = KEY2_PIN;
    gpio_handle.Pull = GPIO_PULLUP;
    gpio_handle.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(KEY2_PORT, &gpio_handle);
}

// TODO: 这里的按键扫描做的不好 要做一个通用的按键扫描函数 且在FreeRTOS中可以使用的
// TODO: 做的也不是很好
uint8_t key_scan(GPIO_TypeDef *gpio_port, uint16_t gpio_pin, GPIO_PinState state) {
    if(HAL_GPIO_ReadPin(gpio_port, gpio_pin) == state) {
        if(state) {
            while(HAL_GPIO_ReadPin(gpio_port, gpio_pin));
        }
        else {
            while(!HAL_GPIO_ReadPin(gpio_port, gpio_pin));
        }
        return 1;
    }
    return 0;
}
