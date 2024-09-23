#include "main.h"
#include "gpio.h"
#include "clock.h"
#include "led.h"
#include "uart.h"
#include "string.h"
#include "stdlib.h"

void SystemClock_Config(void);

uint8_t send_cmd(uint8_t *cmd, uint8_t *res)
{
    uint16_t waitTime = 200;
    HAL_UART_Transmit(&usart3_handler, cmd, strlen((const char*)cmd), 200);
    while(waitTime--)
    {
        if(uart3_msg.finish == true)
        {
            if(strstr((const char*)uart3_msg.data, (const char*)res) != NULL)
            {
                return 0;
            }
        }
        HAL_Delay(10);
    }
    return 1;
}

int main(void)
{
    HAL_Init();

    SystemClock_Config();

    led_init();
    usart1_init(115200);
	usart3_init(9600);
    uint8_t input_current1 = 7;
	uint8_t input_current2 = 18;
	uint8_t command[64];
	char *current_s;
	char *voltage_s;
	float current = 0;
	float voltage = 0;
	float voltage_old = 0;
	char sign = 1;
	float r = 0;
	float Us = 0;
	float x1, x2, x3, x4;
	int i = 0;
    while(1) {
        if(uart1_msg.finish == true) {
			if(strcmp((const char *)uart1_msg.data, (const char *)"start\r\n") == 0) {
				memset(uart1_msg.data, '0', uart1_msg.len);
				uart1_msg.len = 0;
				uart1_msg.finish = false;
				printf("开始测量 打开输入 \r\n");
				while(!send_cmd((uint8_t *)"INP 1\r\n", (uint8_t *)"1"));
				printf("监测负载电压 电流 \r\n");
				sprintf((char*)command, "设置输入电流为%dA \r\n", input_current1);
				printf("%s", command);
				sprintf((char*)command, "CURR %d\r\n", input_current1);
				send_cmd((uint8_t *)command, (uint8_t *)"1");
				printf("等待电压稳定 \r\n");
				while(sign) {
					send_cmd((uint8_t *)"MEAS:REAL?\r\n", (uint8_t *)"1");
					while(!uart3_msg.finish);
					voltage_s = strtok((char *)uart3_msg.data, ",");
					current_s = strtok(NULL, ",");
					voltage = atof(voltage_s);
					current = atof(current_s);
					memset(uart3_msg.data, '0', uart3_msg.len);
					uart3_msg.len = 0;
					uart3_msg.finish = false;
					printf("voltage_old: %f, voltage: %f\n", voltage_old, voltage);
					if(voltage_old == voltage) {
						sign = 0;
					}
					voltage_old = voltage;
					HAL_Delay(2000);
				}
				x2 = voltage;
				x1 = current;
				voltage_old = 0;
				sign = 1;
				sprintf((char*)command, "设置输入电流为%dA \r\n", input_current2);
				printf("%s", command);
				sprintf((char*)command, "CURR %d\r\n", input_current2);
				send_cmd((uint8_t *)command, (uint8_t *)"1");
				printf("等待电压稳定 \r\n");
				while(sign) {
					send_cmd((uint8_t *)"MEAS:REAL?\r\n", (uint8_t *)"1");						
					while(!uart3_msg.finish);
					voltage_s = strtok((char *)uart3_msg.data, ",");
					current_s = strtok(NULL, ",");
					voltage = atof(voltage_s);
					current = atof(current_s);
					memset(uart3_msg.data, '0', uart3_msg.len);
					uart3_msg.len = 0;
					uart3_msg.finish = false;
					printf("voltage_old: %f, voltage: %f\n", voltage_old, voltage);
					if(voltage_old == voltage) {
						sign = 0;
					}
					voltage_old = voltage;
					HAL_Delay(2000);
				}
				x4 = voltage;
				x3 = current;
				voltage_old = 0;
				sign = 1;
				printf("关闭输入 \r\n");
				send_cmd((uint8_t *)"INP 0\r\n", (uint8_t *)"1");	
				r = (x4 - x2) / (x1 - x3);
				Us = x1 * r + x2;
				printf("输入电压为 %fV\r\n", Us);
				r = 0;
				Us = 0;
			}
			else {
				HAL_UART_Transmit(&usart3_handler, uart1_msg.data, uart1_msg.len, 1000);
				memset(uart1_msg.data, '0', uart1_msg.len);
				uart1_msg.len = 0;
				uart1_msg.finish = false;
			}			
		}
		if(uart3_msg.finish == true) {
			HAL_UART_Transmit(&usart1_handler, uart3_msg.data, uart3_msg.len, 1000);
			memset(uart3_msg.data, '0', uart3_msg.len);
			uart3_msg.len = 0;
			uart3_msg.finish = false;
		}
		i++;
		if(i == 250) {
			led0_toggle();
			i = 0;
		}
        HAL_Delay(1);
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
