#include "led.h"


void led_init() {
    GPIO_InitTypeDef gpio_handle;
    __HAL_RCC_GPIOB_CLK_ENABLE();

    gpio_handle.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_handle.Pin = GPIO_PIN_0;
    gpio_handle.Pull = GPIO_PULLDOWN;
    gpio_handle.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOB, &gpio_handle);
}

void led0_on() {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
}

void led0_off() {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
}

void led0_tag() {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
}

