#include "led.h"

void led_init(void) {
    GPIO_InitTypeDef gpio_handler;

    __HAL_RCC_GPIOB_CLK_ENABLE();

    gpio_handler.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_handler.Pin = LED0_PIN;
    gpio_handler.Pull = GPIO_PULLUP;
    gpio_handler.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(LED0_PORT, &gpio_handler);

    gpio_handler.Pin = LED1_PIN;
    HAL_GPIO_Init(LED1_PORT, &gpio_handler);

    led0_off();
    led1_off();
}

void led0_on(void) {
    HAL_GPIO_WritePin(LED0_PORT, LED0_PIN, GPIO_PIN_RESET);
}

void led0_off(void) {
    HAL_GPIO_WritePin(LED0_PORT, LED0_PIN, GPIO_PIN_SET);
}

void led1_on(void) {
    HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, GPIO_PIN_RESET);
}

void led1_off(void) {
    HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, GPIO_PIN_SET);
}

void led0_reversal(void) {
    HAL_GPIO_TogglePin(LED0_PORT, LED0_PIN);
}

void led1_reversal(void) {
    HAL_GPIO_TogglePin(LED1_PORT, LED1_PIN);
}
