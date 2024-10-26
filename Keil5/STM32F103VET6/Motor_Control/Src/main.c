/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "led.h"
#include "uart.h"
#include "tim.h"
#include "FreeRTOS.h"
#include "task.h"
#include "shell.h"
#include "shell_port.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
TaskHandle_t start_task_handler;
TaskHandle_t led_task_handler;
TaskHandle_t uart_task_handler;
TaskHandle_t linear_encoder_task_handler;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), out1_pwm, out1_pwm, out1_pwm);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), led4_on, led4_on, led4_on);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), led4_off, led4_off, led4_off);
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void led_task(void *pvParameters) {
	led_switch(LED3, LED_ON);
	led_switch(LED4, LED_OFF);
    while(1) {
        led_switch(LED6, LED_TORRLE);
        vTaskDelay(500);
    }
}

void linear_encoder_task(void *pvParameters) {
     __IO int32_t CaptureNumber=0;     // 输入捕获数
    while(1) {
        CaptureNumber = (OverflowCount * 65535) + __HAL_TIM_GET_COUNTER(&tim2_handler);
        my_printf(&uart1_handler, "input cap value: %d\t", CaptureNumber);
        my_printf(&uart1_handler, "route: %.3f mm\r\n", (float)(CaptureNumber*5)/1000.0f);
        vTaskDelay(50);
    }
}

void motor1_start() {
	HAL_TIM_Base_Stop(&tim4_pwm_handler);
	HAL_TIM_PWM_Start_IT(&tim4_pwm_handler, TIM_CHANNEL_2);
	shellPrint(&shell, "motor1 start\r\n");
}

void motor1_stop() {
	HAL_TIM_PWM_Stop_IT(&tim4_pwm_handler, TIM_CHANNEL_2);
	shellPrint(&shell, "motor1 stop\r\n");
}

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), motor1_start, motor1_start, motor1_start);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), motor1_stop, motor1_stop, motor1_stop);

void start_task(void *pvParameters) {
    BaseType_t x_return = pdPASS;
    taskENTER_CRITICAL();
    x_return = xTaskCreate(led_task, "led_task", 256, NULL, 4, &led_task_handler);
    my_printf(&uart1_handler, "create %s %s\r\n", "led_task", (x_return == pdPASS)?"success":"failed");

    x_return = xTaskCreate(linear_encoder_task, "linear_encoder_task", 256, NULL, 4, &linear_encoder_task_handler);
    my_printf(&uart1_handler, "create %s %s\r\n", "linear_encoder_task", (x_return == pdPASS)?"success":"failed");


	vTaskDelete(NULL);
    taskEXIT_CRITICAL();
}
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
	BaseType_t x_return = pdPASS;
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
    MX_GPIO_Init();
    /* USER CODE BEGIN 2 */
    led_init();
	uart1_init(115200);
	uart2_init(115200);
	tim4_ch2_pwm_init(99, 71);
    tim2_encode_init();
    HAL_TIM_Encoder_Start(&tim2_handler, TIM_CHANNEL_ALL);
	// tim4_ch2_pwm_init(2-1, 72-1);
	tim3_init(0);
    
    x_return = xTaskCreate(start_task, "start_task", 256, NULL, 1, &start_task_handler);
    my_printf(&uart1_handler, "create %s %s\r\n", "start_task", (x_return == pdPASS)?"success":"failed");
	
	userShellInit();
	
	vTaskStartScheduler(); 
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
		HAL_Delay(500);
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

    /** Initializes the CPU, AHB and APB busses clocks
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB busses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
