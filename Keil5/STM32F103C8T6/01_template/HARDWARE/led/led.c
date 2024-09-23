#include "led.h"

void led_init(void) {
	GPIO_InitTypeDef gpio_handler;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	gpio_handler.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_handler.Pin = GREEN_PIN | WHITE_PIN;
	gpio_handler.Pull = GPIO_PULLUP;
	gpio_handler.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &gpio_handler);
	
	HAL_GPIO_WritePin(GREEN_PORT, GREEN_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(WHITE_PORT, WHITE_PIN, GPIO_PIN_RESET);
}

void greenled_on(void) {
    HAL_GPIO_WritePin(GREEN_PORT, GREEN_PIN, GPIO_PIN_SET);
}

void greenled_off(void) {
    HAL_GPIO_WritePin(GREEN_PORT, GREEN_PIN, GPIO_PIN_RESET);
}

void greenled_toggle(void) {
    HAL_GPIO_TogglePin(GREEN_PORT, GREEN_PIN);
}

void whiteled_on(void) {
    HAL_GPIO_WritePin(WHITE_PORT, WHITE_PIN, GPIO_PIN_SET);
}

void whiteled_off(void) {
    HAL_GPIO_WritePin(WHITE_PORT, WHITE_PIN, GPIO_PIN_RESET);
}

void whiteled_toggle(void) {
    HAL_GPIO_TogglePin(WHITE_PORT, WHITE_PIN);
}


