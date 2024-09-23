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
#include "led.h"
#include "beep.h"
#include "uart.h"
#include "esp8266.h"
#include "mpu6050.h"
#include "oled.h"
#include "w25q64.h"
#include "tim.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
TaskHandle_t start_task_handler;
TaskHandle_t led_task_handler;
TaskHandle_t uart_task_handler;
TaskHandle_t beep_task_handler;
TaskHandle_t esp8266_task_handler;
TaskHandle_t mpu6050_task_handler;
TaskHandle_t oled_task_handler;
TaskHandle_t w25q64_task_handler;
TaskHandle_t pwm_task_handler;

void led_task(void *pvParameters) {
    led_init();
    greenled_on();
    whiteled_off();
    while(1) {
        greenled_toggle();
        whiteled_toggle();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void uart_task(void *pvParameters) {
    uart1_init(115200);
    while(1) {
        if(true == uart1_message.rx_finsh) {
            printf("%s\r\n", uart1_message.rx_buf);
            uart1_clear_message();
        }
    }
}

void beep_task(void *pvParameters) {
    beep_init();
    beep_on();
    vTaskDelay(pdMS_TO_TICKS(50));
    beep_off();
    while(1) {
        
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void esp8266_task(void *pvParameters) {
	uart3_init(115200);
    ESP8266_Init();
    while(1) {
        if(true == uart3_message.rx_finsh) {
			printf("%s\r\n", uart3_message.rx_buf);
			uart3_clear_message();
		}
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

void mpu6050_task(void *pvParameters) {
    float pitch,roll,yaw; 		//欧拉角
    short temp;					//温度	
    uart1_init(115200);
	iic_init(); 	            //初始化IIC总线
    while(mpu_dmp_init());
    while(1) {
		mpu_dmp_get_data(&pitch,&roll,&yaw);
        temp = mpu6050_get_temperature();	//得到温度值
        printf("pitch = %.2f\tpoll = %.2f\troll = %.2f\ttemp = %.2f\r\n", pitch, roll, yaw, temp/100.0);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void oled_task(void *pvParameters) {
    iic_init();
    oled_init();
    oled_clear();
	// oled_show_string(0, 0, "Hello World", 16);
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void w25q64_task(void *pvParameters) {
    uart1_init(115200);
	led_init();
    iic_init();
    oled_init();
    w25q64_init();
	
    printf("ID = %X\r\n", w25qxx_read_id());
	oled_show_one_chinese(0, 0, "王");
    oled_show_one_chinese(20, 0, "庆");
	oled_show_one_chinese(40, 0, "浩");
	
	oled_show_one_chinese(0, 11, "你");
    oled_show_one_chinese(20, 11, "好");
	oled_show_one_chinese(40, 11, "世");
	oled_show_one_chinese(60, 11, "界");
	
    oled_show_string(0, 14, "Hello World", 16);

    while(1) {
		whiteled_toggle();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void pwm_task(void *pvParameters) {
    int compare = 250;
	double proportion = 0;
	int flag = 0;
    tim4_ch2_init(9999, 143);
    uart1_init(115200);
    iic_init();
    oled_init();
	w25q64_init();
    led_init();
    oled_show_one_chinese(0, 0, "占");
    oled_show_one_chinese(20, 0, "空");
    oled_show_one_chinese(40, 0, "比");
    oled_show_one_chinese(60, 0, "：");
	oled_show_char(98, 0, '.', 16);
	oled_show_char(118, 0, '%', 16);
    while(1) {
        proportion = compare / 10000.0 * 100.0;
        printf("compare = %.1f%%\r\n", proportion);
        oled_show_num(80, 0, (int)proportion, 2, 16);
        oled_show_num(105, 0, (int)((int)(proportion * 10) % 10), 1, 16);
        set_tim_compare(TIM4, 2, compare);
		if(compare == 1250) {
			// compare = 250;
			flag = 1;
		}
		else if(compare == 250) {
			flag = 0;
		}
		if(flag == 0)
		{
			compare += 5;
		}
        else {
			compare -= 5;
		}
		greenled_toggle();
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
void start_task(void *pvParameters) {
    taskENTER_CRITICAL();
    xTaskCreate(led_task, "led_task", 256, NULL, 3, &led_task_handler);
    // xTaskCreate(uart_task, "uart_task", 256, NULL, 3, &uart_task_handler);
    // xTaskCreate(beep_task, "beep_task", 256, NULL, 4, &beep_task_handler);
    // xTaskCreate(esp8266_task, "esp8266_task", 256, NULL, 3, &esp8266_task_handler);
    // xTaskCreate(mpu6050_task, "mpu6050_task", 256, NULL, 3, &mpu6050_task_handler);
    // xTaskCreate(oled_task, "oled_task", 256, NULL, 3, &oled_task_handler);
    // xTaskCreate(w25q64_task, "w25q64_task", 1024, NULL, 2, &w25q64_task_handler);
    // xTaskCreate(pwm_task, "pwm_task", 1024, NULL, 2, &pwm_task_handler);
    vTaskDelete(start_task_handler);
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

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    // MX_GPIO_Init();
    /* USER CODE BEGIN 2 */
    xTaskCreate(start_task, "start_task", 128, NULL, 1, &start_task_handler);
    vTaskStartScheduler();
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

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
