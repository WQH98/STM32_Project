#include "tim.h"

TIM_HandleTypeDef tim3_handler;

void tim3_init(uint16_t arr, uint16_t psc) {
    tim3_handler.Instance = TIM3;
    // 时钟分频因子
    tim3_handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    // 向上计数
    tim3_handler.Init.CounterMode = TIM_COUNTERMODE_UP;
    // 分频系数
    tim3_handler.Init.Period = arr;
    // 自动装载值
    tim3_handler.Init.Prescaler =psc;
    HAL_TIM_Base_Init(&tim3_handler);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
    if(htim->Instance == TIM3) {
        __HAL_RCC_TIM3_CLK_ENABLE();
        // 使能定时器3和定时器3更新中断
        __HAL_TIM_ENABLE_IT(&tim3_handler, TIM_IT_UPDATE);
        __HAL_TIM_ENABLE(&tim3_handler);
        // HAL_TIM_Base_Start_IT(&tim3_handler);
        HAL_NVIC_SetPriority(TIM3_IRQn, 1, 3);
        HAL_NVIC_EnableIRQ(TIM3_IRQn);
    }
}

void TIM3_IRQHandler(void) {
    if(__HAL_TIM_GET_FLAG(&tim3_handler, TIM_FLAG_UPDATE) == SET) {
        led0_reversal();
        __HAL_TIM_CLEAR_FLAG(&tim3_handler, TIM_FLAG_UPDATE);
    }
}


