#include "led.h"

void led_init(void) {
    GPIO_InitTypeDef led_gpio_handler;

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    led_gpio_handler.Mode = GPIO_MODE_OUTPUT_PP;
    led_gpio_handler.Pin = LED2_PIN;
    led_gpio_handler.Pull = GPIO_PULLUP;
    led_gpio_handler.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(LED2_PORT, &led_gpio_handler);

    led_gpio_handler.Pin = LED3_PIN;
    HAL_GPIO_Init(LED3_PORT, &led_gpio_handler);

    led_gpio_handler.Pin = LED4_PIN;
    HAL_GPIO_Init(LED4_PORT, &led_gpio_handler);

    led_gpio_handler.Pin = LED5_PIN;
    HAL_GPIO_Init(LED5_PORT, &led_gpio_handler);

    led_gpio_handler.Pin = LED6_PIN;
    HAL_GPIO_Init(LED6_PORT, &led_gpio_handler);

    led_gpio_handler.Pin = LED7_PIN;
    HAL_GPIO_Init(LED7_PORT, &led_gpio_handler);

    led_gpio_handler.Pin = LED8_PIN;
    HAL_GPIO_Init(LED8_PORT, &led_gpio_handler);
    
    led_switch(LED2, LED_OFF);
    led_switch(LED3, LED_OFF);
    led_switch(LED4, LED_OFF);
    led_switch(LED5, LED_OFF);
    led_switch(LED6, LED_OFF);
    led_switch(LED7, LED_OFF);
    led_switch(LED8, LED_OFF);
}   

void led_switch(GPIO_TypeDef *GPIOx, uint32_t Pin, int status) {
    if(status == LED_ON) {
        HAL_GPIO_WritePin(GPIOx, Pin, GPIO_PIN_SET);
    }
    else if(status == LED_OFF) {
        HAL_GPIO_WritePin(GPIOx, Pin, GPIO_PIN_RESET);
    }
    else if(status == LED_TORRLE) {
        HAL_GPIO_TogglePin(GPIOx, Pin);
    }
}

void led4_on(void) {
	led_switch(LED4, LED_ON);
}
void led4_off(void) {
	led_switch(LED4, LED_OFF);
}
