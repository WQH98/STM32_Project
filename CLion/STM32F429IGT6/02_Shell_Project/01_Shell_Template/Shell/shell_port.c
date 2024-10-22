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


#include "shell.h"
#include "stm32f4xx_hal.h"
#include "uart.h"


Shell shell;
char shellBuffer[512];


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
    // serialTransmit(&debugSerial, (uint8_t *)data, len, 0x1FF);
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
short userShellRead(char *data, unsigned short len)
{
    if(HAL_UART_Receive(&uart2_handler, (uint8_t *)data, len, 0x1FF) != HAL_OK)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


/**
 * @brief 用户shell初始化
 * 
 */
void userShellInit(void)
{
    shell.write = userShellWrite;
    shell.read = userShellRead;

    shellInit(&shell, shellBuffer, 512);
}


