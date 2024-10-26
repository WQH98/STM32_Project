#include "tim.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "uart.h"
#include "shell.h"

TIM_HandleTypeDef tim4_pwm_handler;
TIM_HandleTypeDef tim3_handler;
TIM_OC_InitTypeDef tim4_ch2_handler;
TIM_HandleTypeDef tim2_handler;

int32_t OverflowCount = 0;//定时器溢出次数

void tim4_ch2_pwm_init(uint32_t arr, uint32_t psc) {
    TIM_MasterConfigTypeDef tim_master_handler;

    tim4_pwm_handler.Instance = TIM4;
    tim4_pwm_handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    tim4_pwm_handler.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim4_pwm_handler.Init.Period = arr;
	// tim4_pwm_handler.Init.Period = 100-1;
    tim4_pwm_handler.Init.Prescaler = psc;
    // tim4_pwm_handler.Init.Prescaler = 4-1;
	HAL_TIM_PWM_Init(&tim4_pwm_handler);

    tim_master_handler.MasterOutputTrigger = TIM_TRGO_UPDATE;
    tim_master_handler.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
    HAL_TIMEx_MasterConfigSynchronization(&tim4_pwm_handler, &tim_master_handler);

    tim4_ch2_handler.OCMode = TIM_OCMODE_PWM1;
    tim4_ch2_handler.Pulse = arr / 2;
    tim4_ch2_handler.OCPolarity = TIM_OCPOLARITY_LOW;
    HAL_TIM_PWM_ConfigChannel(&tim4_pwm_handler, &tim4_ch2_handler, TIM_CHANNEL_2);
    
    HAL_TIM_Base_Stop(&tim4_pwm_handler);
}



void tim3_init(uint16_t arr) {
    
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_SlaveConfigTypeDef tim_slave_handler;
	
    tim3_handler.Instance = TIM3;
    tim3_handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    tim3_handler.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim3_handler.Init.Period = arr;
	// tim3_handler.Init.Period = 0xFFFF;
    tim3_handler.Init.Prescaler = 0;
    HAL_TIM_Base_Init(&tim3_handler);
	
	
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    // sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_ITR3;
	HAL_TIM_ConfigClockSource(&tim3_handler, &sClockSourceConfig);

    
    tim_slave_handler.InputTrigger = TIM_TS_ITR3;
    tim_slave_handler.SlaveMode = TIM_SLAVEMODE_GATED;
    tim_slave_handler.TriggerFilter = 0;
    tim_slave_handler.TriggerPolarity = TIM_TRIGGERPOLARITY_RISING;
    tim_slave_handler.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1;
    HAL_TIM_SlaveConfigSynchronization(&tim3_handler, &tim_slave_handler);
	
    HAL_TIM_Base_Stop_IT(&tim3_handler);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
    if(htim->Instance == TIM3) {
        __HAL_RCC_TIM3_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM3_IRQn, 2, 3);
        HAL_NVIC_EnableIRQ(TIM3_IRQn);
    }
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim) {
    GPIO_InitTypeDef tim_gpio_handler;
    if(htim->Instance == TIM4) {
        __HAL_RCC_TIM4_CLK_ENABLE();
        __HAL_AFIO_REMAP_TIM4_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();

        tim_gpio_handler.Mode = GPIO_MODE_AF_PP;
        tim_gpio_handler.Pin = GPIO_PIN_13;
        tim_gpio_handler.Pull = GPIO_PULLUP;
        tim_gpio_handler.Speed = GPIO_SPEED_HIGH;
        HAL_GPIO_Init(GPIOD, &tim_gpio_handler);
    }
}


void set_tim4_ch2_compare(uint32_t value) {
    TIM4->CCR2 = value;
}


void TIM3_IRQHandler(void) {
    HAL_TIM_IRQHandler(&tim3_handler);
    
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if(htim->Instance == TIM3) {
        if(__HAL_TIM_GET_FLAG(&tim3_handler, TIM_FLAG_CC2) != RESET) {
            __HAL_TIM_CLEAR_FLAG(&tim3_handler, TIM_FLAG_CC2);
            HAL_TIM_PWM_Stop_IT(&tim4_pwm_handler, TIM_CHANNEL_2);
            HAL_TIM_Base_Stop_IT(&tim3_handler);
        }   
    }
	if(htim->Instance == TIM2) {
		if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&tim2_handler))
			OverflowCount--;       //向下计数溢出
		else
			OverflowCount++;       //向上计数溢出
	}
}

void out1_pwm(uint32_t count) {
	__HAL_TIM_SET_AUTORELOAD(&tim3_handler, count-1);
    HAL_TIM_Base_Start_IT(&tim3_handler); 
    HAL_TIM_PWM_Start_IT(&tim4_pwm_handler, TIM_CHANNEL_2);
}

void tim2_encode_init(void) {
    TIM_Encoder_InitTypeDef tim2_enacode_handler;

    tim2_handler.Instance = TIM2;
    tim2_handler.Init.Prescaler = 0;
    tim2_handler.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim2_handler.Init.Period = 0xFFFF;
    tim2_handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    tim2_enacode_handler.EncoderMode = TIM_ENCODERMODE_TI12;
    tim2_enacode_handler.IC1Polarity = TIM_ICPOLARITY_RISING;
    tim2_enacode_handler.IC1Selection = TIM_ICSELECTION_DIRECTTI;
    tim2_enacode_handler.IC1Prescaler = TIM_ICPSC_DIV1;
    tim2_enacode_handler.IC1Filter = 13;
    tim2_enacode_handler.IC2Polarity = TIM_ICPOLARITY_RISING;
    tim2_enacode_handler.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    tim2_enacode_handler.IC2Prescaler = TIM_ICPSC_DIV1;
    tim2_enacode_handler.IC2Filter = 13;

    __HAL_TIM_SET_COUNTER(&tim2_handler, 0);

    HAL_TIM_Encoder_Init(&tim2_handler, &tim2_enacode_handler);

    __HAL_TIM_CLEAR_IT(&tim2_handler, TIM_IT_UPDATE);
    __HAL_TIM_URS_ENABLE(&tim2_handler);
    __HAL_TIM_ENABLE_IT(&tim2_handler, TIM_IT_UPDATE);
    HAL_NVIC_SetPriority(TIM2_IRQn, 1, 2);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim) {
    GPIO_InitTypeDef gpio_handler;
    if(htim->Instance == TIM2) {
        __HAL_RCC_TIM2_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        gpio_handler.Mode = GPIO_MODE_INPUT;
        gpio_handler.Pin = GPIO_PIN_0 | GPIO_PIN_1;
        gpio_handler.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(GPIOA, &gpio_handler);
    }
}

void TIM2_IRQHandler(void) {
    HAL_TIM_IRQHandler(&tim2_handler);
}


