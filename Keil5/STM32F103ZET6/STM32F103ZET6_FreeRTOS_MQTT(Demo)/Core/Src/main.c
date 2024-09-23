/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"
#include "led.h"
#include "usart.h"
#include "task.h"
#include "esp8266.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

void WIFI_Task(void* pvParameters)
{
	uint16_t cnt = 0;
    uint8_t *pData;
    char *data;
    int reclen = 0;
	uint8_t *message;
	uint16_t message_len;
    uint8_t message_i = 0;
	uint16_t revmessagelen = 0;
	uint8_t revmessage[50];
	uint8_t Heart[2] = {0xC0, 0x00};
	ESP8266_Clear();
	ESP8266_SendData(Bind_OneNet(), 42);
    pData = ESP8266_GetIPD(300);
    if(pData != NULL)
    {
        printf("pData[3] = %X\r\n", pData[3]);
    }
    else
    {
        printf("could not get IPD\r\n");
    }
	ESP8266_Clear();
	for(;;)
	{
		if(cnt == 10000)
		{	
			message = OneNet_SendMessage("Test", "123456");
			message_len = message[1] + 2;
			ESP8266_SendData(message, message_len);
		}
        if(cnt == 20000)
		{	
			message = OneNet_SendMessage("Test", "6789");
			message_len = message[1] + 2;
			ESP8266_SendData(message, message_len);
		}
		if(cnt == 30000)
		{
			ESP8266_SendData(Heart, 2);
			cnt = 0;
		}
		if(USART3_RX_STA_SIGN == 1)
		{
            reclen = USART3_RX_STA;
			if(reclen > 50)
			{
				message_i = 0;
                memset(revmessage, 0, 50);
				//HAL_UART_Transmit(&USART1_Handler, USART3_RX_BUF, USART3_RX_STA, 200);
				for(int i = 0; i < reclen; i++)
				{
					if(USART3_RX_BUF[i] == '/')
					{
						message_i = i + 1;
						break;
					}
				}
				//printf("reclen = %d\r\n", reclen);
				if(message_i == 0)
				{
					printf("not found\r\n");
				}
				
				revmessagelen = reclen - message_i - 36;
				//printf("revmessagelen = %d  reclen = %d  message_i = %d\r\n", revmessagelen, reclen, message_i);
				for(int i = message_i + 36, j = 0; i < reclen; i++, j++)
				{
					//printf("%c", USART3_RX_BUF[i]);
					revmessage[j] = USART3_RX_BUF[i];
				}
				//printf("\r\n");
				HAL_UART_Transmit(&USART1_Handler, revmessage, revmessagelen, 200);
				if(strcmp((const char*)revmessage, "greenledon") == 0)
				{
					GreenLED_ON();
				}
                else if(strcmp((const char*)revmessage, "greenledoff") == 0)
                {
                    GreenLED_OFF();
                }
				else if(strcmp((const char*)revmessage, "redledon") == 0)
                {
                    RedLED_ON();
                }
				else if(strcmp((const char*)revmessage, "redledoff") == 0)
                {
                    RedLED_OFF();
                }
			}
			ESP8266_Clear();
		}
		cnt++;
        vTaskDelay(pdMS_TO_TICKS(1));
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

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */
    LED_Init();
    USART1_Init(115200);
	USART3_Init(115200);
	HAL_Delay(500);
    
    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    
    ESP8266_Init();
    
    //HAL_UART_Transmit(&USART1_Handler, Bind_OneNet(), 42, 200);
    /* USER CODE BEGIN 2 */
    xTaskCreate(WIFI_Task, "WIFI_Task", 3000, NULL, 1, NULL);
    vTaskStartScheduler();

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */
        //printf("error\r\n");
		HAL_Delay(500);
        /* USER CODE BEGIN 3 */
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

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM1 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM1)
    {
        HAL_IncTick();
    }
    /* USER CODE BEGIN Callback 1 */

    /* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
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
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */


