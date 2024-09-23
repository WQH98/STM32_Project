#include "wwdg.h"
#include "led.h"

WWDG_HandleTypeDef wwdg_handle;

/*
    初始化窗口看门狗
    tr：T[6:0]计数器值
    wr：W[6:0]窗口值
    fprer：分频系数(WDGTB) 仅最低两位有效
    Fwwdg = PCLK1 / (4096 * 2^fprer)   一般PCLK1为45MHz
*/
void wwdg_init(uint8_t tr,uint8_t wr,uint32_t fprer) {

    __HAL_RCC_WWDG_CLK_ENABLE();

    wwdg_handle.Instance = WWDG;
    wwdg_handle.Init.Counter = tr;
    wwdg_handle.Init.Prescaler = fprer;
    wwdg_handle.Init.Window = wr;
    HAL_WWDG_Init(&wwdg_handle);
    HAL_WWDG_Start_IT(&wwdg_handle);

    HAL_NVIC_SetPriority(WWDG_IRQn, 2, 3);
    HAL_NVIC_EnableIRQ(WWDG_IRQn);
}

// 中断中喂狗
// 不知道为什么这样写会有问题 
//void WWDG_IRQHandler(void) {
//    if(__HAL_WWDG_GET_FLAG(&wwdg_handle, WWDG_FLAG_EWIF) != RESET) {
//        HAL_WWDG_Refresh(&wwdg_handle,0x7F);//更新窗口看门狗值
//        led1_toggle();
//        __HAL_WWDG_CLEAR_FLAG(&wwdg_handle, WWDG_FLAG_EWIF);
//    }
//}



 //窗口看门狗中断服务函数
 void WWDG_IRQHandler(void) {
     HAL_WWDG_IRQHandler(&wwdg_handle);
 }
 //中断服务函数处理过程,此函数会被 HAL_WWDG_IRQHandler()调用
 void HAL_WWDG_WakeupCallback(WWDG_HandleTypeDef* hwwdg) {
     HAL_WWDG_Refresh(&wwdg_handle,0x7F);//更新窗口看门狗值
     led1_toggle();
 }

