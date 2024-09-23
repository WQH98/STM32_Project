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

/* 全局变量和全局标志位 */
char *g_rec_data;	         // 全局接收服务端发来的数据 指针是全局的 所以不会失效
char g_send_data[256];       // 全局发送给服务端的数据
char g_data_rec_flag = 0;    // 全局标志位 当收到数据时并解析出数据来以后 此标志位置1 否则置0
float g_pitch, g_roll, g_yaw; 		//欧拉角
float g_temp;					    //温度


/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
// 全局任务句柄
TaskHandle_t start_task_handler;
TaskHandle_t wifi_client_task_handler;
TaskHandle_t data_analysis_task_handler;
TaskHandle_t run_task_handler;

// 获取真正的IPD数据
// msg: 接收到的IPD数据
// 返回值：IPD数据指针
//        NULL 收到的不是IPD数据
char *get_ipd(char *msg) {
	msg = strstr(msg, "IPD");
	if(msg != NULL) {
		msg = strstr(msg, ":");
		if(msg != NULL) {
			msg += 1;
			return msg;
		}
		else {
			return NULL;
		}
	}
	else {
		return NULL;
	}
}

// 数据校验函数 
// data: 需要检验的函数
// 返回值：0 数据是正常的
//        其他 错误码
int data_chack(char *data) {
	if(NULL == data) {
        return -1;    // 传入的数据为空
    }
	
	
    return 0;
}

// 运行任务
void run_task(void *pvParameters) {
    int i = 0;
	int compare = 0;
	char signal = 0;
    tim1_ch1_init(9999, 143);
    tim4_ch1_init(9999, 143);
    tim4_ch2_init(9999, 143);
	iic_init();
	while(mpu_dmp_init());
	
    while(1) {
		
		if(0 == (i % 100)) {
			mpu_dmp_get_data(&g_pitch, &g_roll, &g_yaw);
		}
		
		if(signal == 0) {
			compare++;
		}
		else {
			compare--;
		}
		
		if(compare >= 5000) {
			signal = 1;
		}
		if(compare <= 0) {
			signal = 0;
		}
		
		set_tim_compare(TIM1, 1, compare);
		
		if(i == 500) {
			whiteled_toggle();
			i = 0;
		}
		
        i++;
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}


// 数据解析线程  等收到了消息以后 解析出正确的消息
void data_analysis_task(void *pvParameters) {
	int i = 0;
	while(1) {
		if(1 == g_data_rec_flag) {
			for(i = 0; i < strlen(g_rec_data); i++) {
                // printf("0x%02X ", g_rec_data[i]);
            }
			// printf("\r\n");
			g_data_rec_flag = 0;
		}
		vTaskDelay(pdMS_TO_TICKS(1));
	}
}

// 客户端任务  与服务器互相收发数据 100ms上传一次
void wifi_client_task(void *pvParameters) {
	int i = 0;

    unsigned char *data;
	led_init();
	char rec_data[256];
	uart1_init(115200);
	uart3_init(115200);
	// ESP8266_Init();

	
	taskENTER_CRITICAL();
	// xTaskCreate(data_analysis_task, "data_analysis_task", 512, NULL, 4, &data_analysis_task_handler);
	xTaskCreate(run_task, "run_task", 1024, NULL, 4, &run_task_handler);
	taskEXIT_CRITICAL();
	
    while(1) {
		
		if(true == uart3_message.rx_finsh) {				    // 接收到了来自服务器的消息
			// 掉线重连函数 当检测到掉线以后 进入阻塞式的连接 其他任务全部暂停 只执行重新连接函数
			if(strcmp((const char *)uart3_message.rx_buf, "CLOSED\r\n") == 0) {         
				taskENTER_CRITICAL();
                ESP8266_Init();
                taskEXIT_CRITICAL();
				uart3_clear_message();
				goto con;
            }
			memset(rec_data, 0, sizeof(rec_data));                 // 先把缓存清空
            strcpy(rec_data, (const char *)uart3_message.rx_buf);     // 把收到的消息放到缓存中
			g_rec_data = get_ipd(rec_data);             // 去除协议头 获取出真正的IPD数据
			if(g_rec_data != NULL && !data_chack(g_rec_data)) {                    
				g_data_rec_flag = 1;                    // 收到的是IPD数据 把接收数据标志位置1
			}                                           
            uart3_clear_message();           // 将串口接收缓存清空
        }

        if(0 == (i % 100)) {            // TODO: 上传数据
            
            g_temp = mpu6050_get_temperature();	//得到温度值
            g_send_data[0] = 0x55;
            g_send_data[1] = HAL_GPIO_ReadPin(GREEN_PORT, GREEN_PIN);   // 绿灯状态
            g_send_data[2] = HAL_GPIO_ReadPin(WHITE_PORT, WHITE_PIN);   // 白灯状态
            data = (unsigned char*)(&g_pitch);     // 俯仰角
            g_send_data[3] = data[0];
            g_send_data[4] = data[1];
            g_send_data[5] = data[2];
            g_send_data[6] = data[3];
            data = (unsigned char*)(&g_roll);     // 翻滚角
            g_send_data[7] = data[0];
            g_send_data[8] = data[1];
            g_send_data[9] = data[2];
            g_send_data[10] = data[3];
            data = (unsigned char*)(&g_yaw);        // 偏航角
            g_send_data[11] = data[0];
            g_send_data[12] = data[1];
            g_send_data[13] = data[2];
            g_send_data[14] = data[3];
            data = (unsigned char*)(&g_temp);      // 温度
            g_send_data[15] = data[0];
            g_send_data[16] = data[1];
            g_send_data[17] = data[2];
            g_send_data[18] = data[3];
			
            g_send_data[19] = 0;                 // 校验位
            for(int j = 0; j < 19; j++) {
                g_send_data[19] += g_send_data[j];
            }
			// printf("%s", data);
			HAL_UART_Transmit(&usart1_hander, (uint8_t *)g_send_data, 20, 200);
			// ESP8266_SendData((uint8_t *)g_send_data, 20);
            
        }
con:
        if(i == 500) {
			greenled_toggle();                  // 系统运行指示灯
			// printf("g_pitch = %.2f\tg_roll = %.2f\tg_yaw = %.2f\tg_temp = %.2f\r\n", g_pitch, g_roll, g_yaw, g_temp);
            i = 0;
        }
        
		i++;
		vTaskDelay(pdMS_TO_TICKS(1));
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
    xTaskCreate(wifi_client_task, "wifi_client_task", 1024, NULL, 3, &wifi_client_task_handler);
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
    MX_GPIO_Init();
    /* USER CODE BEGIN 2 */
    xTaskCreate(start_task, "start_task", 64, NULL, 1, &start_task_handler);
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
