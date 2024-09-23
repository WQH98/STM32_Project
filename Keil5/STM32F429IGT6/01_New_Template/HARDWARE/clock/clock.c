#include "clock.h"


void stm32_clock_init(void) {
	// 使能PWR时钟
	__HAL_RCC_PWR_CLK_ENABLE();
	
    RCC_OscInitTypeDef rcc_oscinitstruct;
	// 设置调压器输出电压级别 以便在没有以最大频率工作时实现性能与功耗的平衡
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    // 设置时钟源为HSE
    rcc_oscinitstruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    // 打开HSE
    rcc_oscinitstruct.HSEState = RCC_HSE_ON;
    // 打开PLL
    rcc_oscinitstruct.PLL.PLLState = RCC_PLL_ON;
    // PLL时钟源为HSE
    rcc_oscinitstruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    rcc_oscinitstruct.PLL.PLLM = 25;
    rcc_oscinitstruct.PLL.PLLN = 360;
    rcc_oscinitstruct.PLL.PLLP = 2;
    rcc_oscinitstruct.PLL.PLLQ = 8;
    // 初始化系统时钟
    while(HAL_RCC_OscConfig(&rcc_oscinitstruct) != HAL_OK);
	// 开启over_drive功能
	while(HAL_PWREx_EnableOverDrive() != HAL_OK);
	
    RCC_ClkInitTypeDef rcc_clkinitstruct;
    // 选中PLL作为系统时钟源并配置HCLK PCLK1和PCLK2
    rcc_clkinitstruct.ClockType = ( RCC_CLOCKTYPE_SYSCLK |
									RCC_CLOCKTYPE_HCLK   |
									RCC_CLOCKTYPE_PCLK1  |
									RCC_CLOCKTYPE_PCLK2);
    // 系统时钟源PLL
    rcc_clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    // 分频系数为1
    rcc_clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    // 分频系数为4
    rcc_clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV4;
    // 分频系数为2
    rcc_clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV2;
    // 同时设置FLASH延时周期 也就是6个CPU周期
    while(HAL_RCC_ClockConfig(&rcc_clkinitstruct, FLASH_LATENCY_5) != HAL_OK);
    // SysTick频率为HCLK
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}


void delay_us(uint32_t cnt)
{
    uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				//LOAD的值
	ticks=cnt*180; 						        //需要的节拍数
	told=SysTick->VAL;        				    //刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
		}  
	}
}


void delay_ms(uint32_t cnt)
{
    uint32_t i = 0; 
    for(i = 0; i < cnt; i++)
    {
        delay_us(1000);
    }
}
