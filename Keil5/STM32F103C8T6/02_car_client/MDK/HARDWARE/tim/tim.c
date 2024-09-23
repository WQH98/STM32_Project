#include "tim.h"

TIM_HandleTypeDef  tim1_handler;
TIM_OC_InitTypeDef tim1_ch1_handler;
TIM_HandleTypeDef  tim4_handler;
TIM_OC_InitTypeDef tim4_ch1_handler;
TIM_OC_InitTypeDef tim4_ch2_handler;

void tim1_ch1_init(uint16_t arr, uint16_t psc) {
    
    GPIO_InitTypeDef gpio_handler;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_TIM1_CLK_ENABLE();
    __HAL_RCC_AFIO_CLK_ENABLE();

    tim1_handler.Instance = TIM1;               // 选择定时器1
    tim1_handler.Init.Prescaler = psc;          // 定时器分频
    tim1_handler.Init.CounterMode = TIM_COUNTERMODE_UP;            // 向上计数模式
    tim1_handler.Init.Period = arr;             // 自动重装载值
    tim1_handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;      // 不分频
    HAL_TIM_PWM_Init(&tim1_handler);            // 初始化PWM

    tim1_ch1_handler.OCMode = TIM_OCMODE_PWM1;      // 模式选择PWM1
    tim1_ch1_handler.Pulse = arr / 2;              // 设置比较值 此值用来确定占空比 
                                                   // 默认占空比为自动装载值的一半 即占空比为50%
    tim1_ch1_handler.OCPolarity = TIM_OCPOLARITY_HIGH;          // 输出比较极性为高
    HAL_TIM_PWM_ConfigChannel(&tim1_handler, &tim1_ch1_handler, TIM_CHANNEL_1);    // 配置TIM1通道1

    gpio_handler.Mode = GPIO_MODE_AF_PP;
    gpio_handler.Pin = TIM1_CH1_PIN;
    gpio_handler.Pull = GPIO_PULLUP;
    gpio_handler.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TIM1_CH1_PORT, &gpio_handler);

    HAL_TIM_PWM_Start(&tim1_handler, TIM_CHANNEL_1);     // 开启PWM通道1
}

void tim4_ch1_init(uint16_t arr, uint16_t psc) {

    GPIO_InitTypeDef gpio_handler;

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_TIM4_CLK_ENABLE();
    __HAL_RCC_AFIO_CLK_ENABLE();

    tim4_handler.Instance = TIM4;               // 选择定时器4
    tim4_handler.Init.Prescaler = psc;          // 定时器分频
    tim4_handler.Init.CounterMode = TIM_COUNTERMODE_UP;            // 向上计数模式
    tim4_handler.Init.Period = arr;             // 自动重装载值
    tim4_handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;      // 不分频
    HAL_TIM_PWM_Init(&tim4_handler);            // 初始化PWM

    tim4_ch1_handler.OCMode = TIM_OCMODE_PWM1;      // 模式选择PWM1
    tim4_ch1_handler.Pulse = arr / 2;              // 设置比较值 此值用来确定占空比 
                                                   // 默认占空比为自动装载值的一半 即占空比为50%
    tim4_ch1_handler.OCPolarity = TIM_OCPOLARITY_HIGH;          // 输出比较极性为高
    HAL_TIM_PWM_ConfigChannel(&tim4_handler, &tim4_ch1_handler, TIM_CHANNEL_1);    // 配置TIM4通道1

    gpio_handler.Mode = GPIO_MODE_AF_PP;
    gpio_handler.Pin = TIM4_CH1_PIN;
    gpio_handler.Pull = GPIO_PULLUP;
    gpio_handler.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TIM4_CH1_PORT, &gpio_handler);

    HAL_TIM_PWM_Start(&tim4_handler, TIM_CHANNEL_1);     // 开启PWM通道1
}


void tim4_ch2_init(uint16_t arr, uint16_t psc) {

    GPIO_InitTypeDef gpio_handler;

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_TIM4_CLK_ENABLE();
    __HAL_RCC_AFIO_CLK_ENABLE();

    tim4_handler.Instance = TIM4;               // 选择定时器4
    tim4_handler.Init.Prescaler = psc;          // 定时器分频
    tim4_handler.Init.CounterMode = TIM_COUNTERMODE_UP;            // 向上计数模式
    tim4_handler.Init.Period = arr;             // 自动重装载值
    tim4_handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;      // 不分频
    HAL_TIM_PWM_Init(&tim4_handler);            // 初始化PWM

    tim4_ch2_handler.OCMode = TIM_OCMODE_PWM1;      // 模式选择PWM1
    tim4_ch2_handler.Pulse = arr / 2;              // 设置比较值 此值用来确定占空比 
                                                   // 默认占空比为自动装载值的一半 即占空比为50%
    tim4_ch2_handler.OCPolarity = TIM_OCPOLARITY_HIGH;          // 输出比较极性为高
    HAL_TIM_PWM_ConfigChannel(&tim4_handler, &tim4_ch2_handler, TIM_CHANNEL_2);    // 配置TIM4通道2

    gpio_handler.Mode = GPIO_MODE_AF_PP;
    gpio_handler.Pin = TIM4_CH2_PIN;
    gpio_handler.Pull = GPIO_PULLUP;
    gpio_handler.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TIM4_CH2_PORT, &gpio_handler);

    HAL_TIM_PWM_Start(&tim4_handler, TIM_CHANNEL_2);     // 开启PWM通道2
}


// TODO: 增加其他通道参数
void set_tim_compare(TIM_TypeDef *timx, uint8_t channelx, uint32_t compare) {
    switch(channelx) {
        case 1:
            timx->CCR1 = compare;
            break;
        case 2:
            timx->CCR2 = compare;
            break;
        case 3:
            timx->CCR3 = compare;
            break;
        case 4:
            timx->CCR4 = compare;
            break;
        default:
            break;
    }
}
