/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include "stdio.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "led.h"
#include "uart.h"
#include "adc.h"
#include "inner_temperature.h"
#include "tim.h"
#include "key.h"
#include "at24cxx.h"
#include "pcf8574t.h"
#include "w25qxx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//任务句柄
TaskHandle_t start_task_handler;
TaskHandle_t led_task_handler;
TaskHandle_t send_task_handler;
TaskHandle_t receive_task_handler;

EventGroupHandle_t event_handler;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
void led_task(void *pvParameters) {
    led0_on();
    led1_off();
    while(1) {
        led0_reversal();
        led1_reversal();
        vTaskDelay(500);
    }
}

void send_task(void *pvParameters) {

    while(1) {
        if(HAL_GPIO_ReadPin(KEY0_PORT, KEY0_PIN) == GPIO_PIN_RESET) {
            xEventGroupSetBits(event_handler, 0x01);
        }
        if(HAL_GPIO_ReadPin(KEY1_PORT, KEY1_PIN) == GPIO_PIN_RESET) {
            xEventGroupSetBits(event_handler, 0x02);
        }
        if(HAL_GPIO_ReadPin(KEY0_PORT, KEY0_PIN) == GPIO_PIN_RESET && HAL_GPIO_ReadPin(KEY1_PORT, KEY1_PIN) == GPIO_PIN_RESET) {
            xEventGroupSetBits(event_handler, 0x03);
        }

        vTaskDelay(200);
    }
}

void receive_task(void *pvParameters) {
    EventBits_t event_bits;
    while(1) {
        event_bits = xEventGroupWaitBits(event_handler, 0x03, pdTRUE, pdFALSE, portMAX_DELAY);
        if(event_bits == 0x01) {
            my_printf(&uart1_handler, "KEY0 pressed\r\n");
            // xEventGroupClearBits(event_handler, 0x01);
        }
        else if(event_bits == 0x02) {
            my_printf(&uart1_handler, "KEY1 pressed\r\n");
            // xEventGroupClearBits(event_handler, 0x02);
        }
        else if(event_bits == 0x03) {
            my_printf(&uart1_handler, "KEY0 and KEY1 both pressed\r\n");
            // xEventGroupClearBits(event_handler, 0x03);
        }
        vTaskDelay(1);
    }
}



void start_task(void *pvParameters)
{
    BaseType_t x_return = pdPASS;
    taskENTER_CRITICAL();
    x_return = xTaskCreate(led_task, "led_task", 256, NULL, 2, &led_task_handler);
    my_printf(&uart1_handler, "creat %s %s\r\n", "led_task", (x_return == pdPASS)? "success" : "failed");
    x_return = xTaskCreate(send_task, "send_task", 256, NULL, 3, &send_task_handler);
    my_printf(&uart1_handler, "creat %s %s\r\n", "send_task", (x_return == pdPASS)? "success" : "failed");
    x_return = xTaskCreate(receive_task, "receive_task", 256, NULL, 4, &receive_task_handler);
    my_printf(&uart1_handler, "creat %s %s\r\n", "receive_task", (x_return == pdPASS)? "success" : "failed");
    // 创建一个事件组
    event_handler = xEventGroupCreate();
    my_printf(&uart1_handler, "creat %s %s\r\n", "event_group", (event_handler != NULL)? "success" : "failed");
    vTaskDelete(NULL);
    taskEXIT_CRITICAL();
}


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
int main(void) {
    /* USER CODE BEGIN 1 */

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
    led_init();
    key_init();
    uart1_init(115200);
    xTaskCreate(start_task, "start_task", 128, NULL, 1, &start_task_handler);
    vTaskStartScheduler();          //开启任务调度
    /* USER CODE BEGIN 2 */
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */

    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
    */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 15;
    RCC_OscInitStruct.PLL.PLLN = 216;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Activate the Over-Drive mode
    */
    if (HAL_PWREx_EnableOverDrive() != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
