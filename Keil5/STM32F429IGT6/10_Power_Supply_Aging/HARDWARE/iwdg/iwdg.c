#include "iwdg.h"

IWDG_HandleTypeDef iwdg_handle;

/*
    初始化独立看门狗
    prer：分频数 IWDG_PRESCALER_4~IWDG_PRESCALER_256
    rlr： 自动重装载值
    时间计算：Tout = (((4 * 2^prer) * rlr)/32)ms
*/
void iwdg_init(uint8_t prer, uint16_t rlr) { 
    
    iwdg_handle.Instance = IWDG;
    iwdg_handle.Init.Prescaler = prer;    // 设置IWDG的预分频系数
    iwdg_handle.Init.Reload = rlr;        // 设置IWDG的重装载值
    HAL_IWDG_Init(&iwdg_handle);

    HAL_IWDG_Start(&iwdg_handle);
}

/*
    喂狗
*/
void iwdg_feed(void) {
    HAL_IWDG_Refresh(&iwdg_handle);
}

