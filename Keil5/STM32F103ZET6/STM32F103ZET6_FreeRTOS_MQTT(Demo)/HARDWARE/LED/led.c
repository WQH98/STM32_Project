#include "led.h"

GPIO_InitTypeDef GPIOB_Init_Struct;
GPIO_InitTypeDef GPIOE_Init_Struct;

void LED_Init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
	
	GPIOB_Init_Struct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOB_Init_Struct.Pin = GPIO_PIN_5;
	GPIOB_Init_Struct.Pull = GPIO_PULLUP;
	GPIOB_Init_Struct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIOB_Init_Struct);

    GPIOE_Init_Struct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOE_Init_Struct.Pin = GPIO_PIN_5;
	GPIOE_Init_Struct.Pull = GPIO_PULLUP;
	GPIOE_Init_Struct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOE, &GPIOE_Init_Struct);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);
}

void RedLED_ON(void) 
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
}
void RedLED_OFF(void) 
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
}
void GreenLED_ON(void)
{
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);
}
void GreenLED_OFF(void)
{
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);
}

void RedLED_Reversal(void)
{
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
}

void GreenLED_Reversal(void)
{
    HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_5);
}

