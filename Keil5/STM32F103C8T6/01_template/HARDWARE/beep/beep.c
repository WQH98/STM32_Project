#include "beep.h"

void beep_init(void)
{
	GPIO_InitTypeDef gpio_handler;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	gpio_handler.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_handler.Pin = BEEP_PIN;
	gpio_handler.Pull = GPIO_PULLUP;
	gpio_handler.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &gpio_handler);
	
	HAL_GPIO_WritePin(BEEP_PORT, BEEP_PIN, GPIO_PIN_RESET);
}

void beep_on(void) {
    HAL_GPIO_WritePin(BEEP_PORT, BEEP_PIN, GPIO_PIN_SET);
}

void beep_off(void) {
    HAL_GPIO_WritePin(BEEP_PORT, BEEP_PIN, GPIO_PIN_RESET);
}

void beep_toggle(void) {
    HAL_GPIO_TogglePin(BEEP_PORT, BEEP_PIN);
}
