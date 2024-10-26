/**
 * @file shell_port.c
 * @author Letter (NevermindZZT@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-22
 * 
 * @copyright (c) 2019 Letter
 * 
 */

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "shell.h"
#include "uart.h"
#include "stm32f1xx_hal.h"



Shell shell;
char shellBuffer[512];

static SemaphoreHandle_t shellMutex;

/**
 * @brief 用户shell写
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际写入的数据长度
 */
short userShellWrite(char *data, unsigned short len)
{
    HAL_UART_Transmit(&uart2_handler, (uint8_t *)data, len, 0x1FF);
    return len;
}


/**
 * @brief 用户shell读
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际读取到
 */
short userShellRead(char *data, unsigned short len) {
    if(HAL_UART_Receive(&uart2_handler, (uint8_t *)data, len, 1) != HAL_OK)
    {
        vTaskDelay(1);
		// my_printf(&uart1_handler, "%d", data);
        return 0;
    }
    else
    {
        vTaskDelay(1);
		// my_printf(&uart1_handler, "%d", data);
        return 1;
    }
// 	return 0;
}
/**
 * @brief 用户shell上锁
 * 
 * @param shell shell
 * 
 * @return int 0
 */
int userShellLock(Shell *shell)
{
    xSemaphoreTake(shellMutex, portMAX_DELAY);
    return 0;
}

/**
 * @brief 用户shell解锁
 * 
 * @param shell shell
 * 
 * @return int 0
 */
int userShellUnlock(Shell *shell)
{
    xSemaphoreGive(shellMutex);
    return 0;
}

/**
 * @brief 用户shell初始化
 * 
 */
void userShellInit(void)
{
    shellMutex = xSemaphoreCreateMutex();

    shell.write = userShellWrite;
	// shell.read = userShellRead;
    // shell.lock = userShellLock;
    // shell.unlock = userShellUnlock;
    shellInit(&shell, shellBuffer, 512);
    xTaskCreate(shellTask, "shell", 1024, &shell, 3, NULL);
}

