/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 ** This notice applies to any and all portions of this file
 * that are not between comment pairs USER CODE BEGIN and
 * USER CODE END. Other portions of this file, whether
 * inserted by the user or by software development tools
 * are owned by their respective copyright owners.
 *
 * COPYRIGHT(c) 2023 STMicroelectronics
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include "stdbool.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "clock.h"
#include "led.h"
#include "uart.h"
#include "pcf8574.h"
#include "key.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum key_state {
	state_idle = 0,
	state_press_and_vibration,
	state_short_press,
	state_long_press,
	state_release_and_vibration
} key_state_type;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
bool is_key_level_low(void) {
	if(HAL_GPIO_ReadPin(KEY0_PORT, KEY0_PIN) == GPIO_PIN_RESET) {
		return true;
	}
	else {
		return false;
	}
}

bool is_key_level_high(void) {
	if(HAL_GPIO_ReadPin(KEY0_PORT, KEY0_PIN) == GPIO_PIN_SET) {
		return true;
	}
	else {
		return false;
	}
}

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* USER CODE BEGIN 1 */
	uint8_t counter, last_state, state = state_idle;
	uint32_t check_time, old_time, value = 0;
    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */

    /* USER CODE BEGIN 2 */
    led_init();
    usart1_init(115200);
	key_init();
	old_time = HAL_GetTick();      // 为state_idle初始化old_time
	printf("hello world\r\n");
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */

        switch(state) {
			// 空闲状态下检测按键电平 如果检测到低电平 表示可能按键被按下
			case state_idle:
				// 空闲时 每间隔1秒输出等待按键提示信息
				if(HAL_GetTick() - old_time > 1000) {
					printf("state idle, waiting for key press ...\r\n");
					old_time = HAL_GetTick();
				}
				// 检测到低电平 按键可能被按下 需要进行消抖来进一步判断
				if(is_key_level_low()) {
					// 为state_press_and_vibration状态设置初始化check_time
					// 设置首次开始消抖检测的时间
					check_time = HAL_GetTick();
					// 设置消抖检测次数为5
					counter = 5;
					// 更改当前状态为state_press_and_vibration
					// 跳转到state_press_and_vibration状态
					state = state_press_and_vibration;
				}
				break;
            // 对按下动作进行消抖 每间隔10ms检测一次按键状态
            // 如果连续counter(5)次检测到低电平 表示按键稳定按下 否则表示仍处于抖动中
			case state_press_and_vibration:
				// 还没检测完成 需要继续检测按键电平
                if(counter) {
                    // 每隔10ms检测一次按键电平
                    if(HAL_GetTick() - check_time > 10) {
                        // 如果检测到1次高电平 表明有抖动 则返回空闲状态
                        if(is_key_level_high()) {
                            // 为空闲状态初始化时间
                            old_time = HAL_GetTick();
                            state = state_idle;
                        }
                    }    
                    // 如果未检测到抖动 则更新本次检测的时间 将计数减一
                    else {
                        // 记录本次消抖时间
                        check_time = HAL_GetTick();
                        // 计数值减一
                        counter--;
                    }            
                }
                else {
                    // 将value的计数值加一并打印输出
                    printf(">>>>  short press, value = %u\r\n", ++value);
                    // 初始化old_time
                    old_time = HAL_GetTick();
                    // 跳转到state_short_press状态
                    state = state_short_press;
                }
				break;
            // 按键状态稳定以后 需要继续检测按键电平的状态 如果按下的时间超过2秒 则判断为长按按键
			case state_short_press:
                // 短按已经保持了2秒 进入长按状态
                if(HAL_GetTick() - old_time > 2000) {
                    // 初始化当前时间
                    old_time = HAL_GetTick();
                    // 进入长按状态
                    state = state_long_press;
                }
                // 短按的过程中 检测到了按键的高电平
                // 表示有可能放开了按键 需要消抖来进一步判断
                else if(is_key_level_high()) {
                    // 初始化检测时间
                    check_time = HAL_GetTick();
                    // 检测次数为5
                    counter = 5;
                    // 备份当前的状态 方便从消抖状态中返回
                    last_state = state;   
                    // 切换状态到消抖
                    state = state_release_and_vibration;
                }
				break;
            // 长按状态下 需要继续检测按键电平的状态 并且每间隔200ms 将value值加1 并打印输出
            // 如果检测到有高电平 表示发生抖动
			case state_long_press:
                // 每隔200ms value值加一
                if(HAL_GetTick() - old_time > 200) {
                    printf(">>>> long press, value = %u\r\n", ++value);
                    // 记录本次时间
                    old_time = HAL_GetTick();
                }
                // 如果长按的过程中 检测到了高电平 表示可能放开了按键 需要进一步判断
                if(is_key_level_high()) {
                    // 初始化检测时间
                    check_time = HAL_GetTick();
                    // 检测次数为5
                    counter = 5;
                    // 备份当前的状态 方便从消抖状态中返回
                    last_state = state;   
                    // 切换状态到消抖
                    state = state_release_and_vibration;
                }
				break;
            // 对松开的按键进行消抖 每间隔10ms检测按键状态
            // 如果连续5次检测到高电平 说明按键稳定松开 否则表示仍处于按下中
			case state_release_and_vibration:
				// 检测次数没用完 仍需接着检测按键电平
                if(counter) {
                    // 每隔10ms检测一次按键电平
                    if(HAL_GetTick() - check_time > 10) {
                        // 如果检测到一次低电平 表明有抖动 则返回之前的状态
                        if(is_key_level_low()) {
                            state = last_state;
                        }
                        // 未检测到低电平 则更新本次检测的时间 将counter减一
                        else {
                            check_time = HAL_GetTick();
                            counter--;
                        }
                    }
                }
                // 已经检测到5次高电平 表示按键确实松开 并且已经处于稳定状态
                else {
                    old_time = HAL_GetTick();
                    // 跳转到state_idle
                    state = state_idle;
                }
				break;
			default:
				break;
		}
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /**Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    /**Initializes the CPU, AHB and APB busses clocks
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 15;
    RCC_OscInitStruct.PLL.PLLN = 216;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /**Activate the Over-Drive mode
     */
    if (HAL_PWREx_EnableOverDrive() != HAL_OK)
    {
        Error_Handler();
    }
    /**Initializes the CPU, AHB and APB busses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
