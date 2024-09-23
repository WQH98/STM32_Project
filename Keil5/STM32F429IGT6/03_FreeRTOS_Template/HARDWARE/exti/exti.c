#include "exti.h"
#include "key.h"
#include "led.h"

void exti_init(void) {
	
    GPIO_InitTypeDef gpio_handle;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    gpio_handle.Mode = GPIO_MODE_IT_RISING;
    gpio_handle.Pin = WK_UP_PIN;
    gpio_handle.Pull = GPIO_PULLDOWN;
    gpio_handle.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(WK_UP_PORT, &gpio_handle);

    gpio_handle.Mode = GPIO_MODE_IT_FALLING;
    gpio_handle.Pin = KEY0_PIN;
    gpio_handle.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY0_PORT, &gpio_handle);

    gpio_handle.Pin = KEY1_PIN;    
    HAL_GPIO_Init(KEY1_PORT, &gpio_handle);

    gpio_handle.Pin = KEY2_PIN;    
    HAL_GPIO_Init(KEY2_PORT, &gpio_handle);

    HAL_NVIC_SetPriority(EXTI0_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    HAL_NVIC_SetPriority(EXTI2_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);

    HAL_NVIC_SetPriority(EXTI3_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void EXTI0_IRQHandler(void) {
    if(__HAL_GPIO_EXTI_GET_FLAG(WK_UP_PIN) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_FLAG(WK_UP_PIN);
        led0_on();
    }
}

void EXTI2_IRQHandler(void) {
    if(__HAL_GPIO_EXTI_GET_FLAG(KEY1_PIN) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_FLAG(KEY1_PIN);
        led0_off();
    }
}

void EXTI3_IRQHandler(void) {
    if(__HAL_GPIO_EXTI_GET_FLAG(KEY0_PIN) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_FLAG(KEY0_PIN);
        led1_on();
    }
}

void EXTI15_10_IRQHandler(void) {
    if(__HAL_GPIO_EXTI_GET_FLAG(KEY2_PIN) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_FLAG(KEY2_PIN);
        led1_off();
    }
}
