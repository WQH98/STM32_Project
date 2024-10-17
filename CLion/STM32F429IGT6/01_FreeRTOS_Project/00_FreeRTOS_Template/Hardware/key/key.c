#include "key.h"


void key_init(void) {
    GPIO_InitTypeDef gpio_handler;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    gpio_handler.Mode = GPIO_MODE_INPUT;
    gpio_handler.Pin = WK_UP_PIN;
    gpio_handler.Pull = GPIO_PULLDOWN;
    gpio_handler.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(WK_UP_PORT, &gpio_handler);

    gpio_handler.Pin = KEY0_PIN;
    gpio_handler.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY0_PORT, &gpio_handler);

    gpio_handler.Pin = KEY1_PIN;
    HAL_GPIO_Init(KEY1_PORT, &gpio_handler);

    gpio_handler.Pin = KEY2_PIN;
    HAL_GPIO_Init(KEY2_PORT, &gpio_handler);
}


