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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

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
uint8_t iic_text_buff[] = {"Apollo STM32F4 IIC TEST"};
uint8_t spi_text_buff[] = {"Apollo STM32F4 SPI TEST"};
#define IIC_SIZE sizeof(iic_text_buff)
#define SPI_SIZE sizeof(spi_text_buff)
uint8_t iic_datatemp[IIC_SIZE];
uint8_t spi_datatemp[SPI_SIZE];
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
    /* USER CODE BEGIN 1 */
    uint8_t count = 0;
    uint8_t beep_status = 0;
    uint32_t FLASH_SIZE = 32*1024*1024;
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
    uart1_init(115200);
    // adc1_init();
    // tim3_init(4999, 8999);
    key_init();
    if(pcf8574_init() == 0) {
        my_printf(&uart1_handler, "pcf8574 check ok\r\n");
    }
    else {
        my_printf(&uart1_handler, "pcf8574 check failed\r\n");
    }
    // iic_init();
    // HAL_Delay(10);
    if(at24cxx_check() == 0) {
        my_printf(&uart1_handler, "at24cxx check ok\r\n");
    }
    else {
        my_printf(&uart1_handler, "at24cxx check failed\r\n");
    }
    w25qxx_init();
    while(w25qxx_read_id() != W25Q256) {
        my_printf(&uart1_handler, "w25q256 check failed %d\r\n", w25qxx_read_id());
        HAL_Delay(500);
    }
    my_printf(&uart1_handler, "w25q256 check ok\r\n");


    /* USER CODE BEGIN 2 */
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        if(HAL_GPIO_ReadPin(KEY0_PORT, KEY0_PIN) == RESET) {
            HAL_Delay(49);
            if(HAL_GPIO_ReadPin(KEY0_PORT, KEY0_PIN) == RESET) {
                at24cxx_write(0, (uint8_t*)iic_text_buff, IIC_SIZE);
                w25qxx_write((uint8_t*)spi_text_buff, FLASH_SIZE-100, SPI_SIZE);
            }
            while(!HAL_GPIO_ReadPin(KEY0_PORT, KEY0_PIN));
        }

        if(HAL_GPIO_ReadPin(KEY1_PORT, KEY1_PIN) == RESET) {
            HAL_Delay(49);
            if(HAL_GPIO_ReadPin(KEY1_PORT, KEY1_PIN) == RESET) {
                at24cxx_read(0, iic_datatemp, IIC_SIZE);
                my_printf(&uart1_handler, "at24cxx read data: %s\t", iic_datatemp);
                w25qxx_read(spi_datatemp, FLASH_SIZE-100, SPI_SIZE);
                my_printf(&uart1_handler, "w25qxx read data: %s\r\n", spi_datatemp);
                memset(iic_datatemp, '0', IIC_SIZE);
                memset(spi_datatemp, '0', SPI_SIZE);
            }
            while(!HAL_GPIO_ReadPin(KEY1_PORT, KEY1_PIN));
        }

        if(HAL_GPIO_ReadPin(KEY2_PORT, KEY2_PIN) == RESET) {
            HAL_Delay(49);
            if(HAL_GPIO_ReadPin(KEY2_PORT, KEY2_PIN) == RESET) {
                beep_status = !beep_status;
                pcf8574_write_bit(BEEP_IO, beep_status);
            }
            while(!HAL_GPIO_ReadPin(KEY2_PORT, KEY2_PIN));
        }

        if(HAL_GPIO_ReadPin(IIC_INT_PORT, IIC_INT_PIN) == 0) {
            my_printf(&uart1_handler, "1 press\r\n");
            if(pcf8574_read_bit(EX_IO) == 0) {
                led1_reversal();
                my_printf(&uart1_handler, "key2 press\r\n");
            }
        }
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
