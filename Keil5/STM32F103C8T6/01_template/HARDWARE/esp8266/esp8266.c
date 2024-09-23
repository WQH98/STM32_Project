#include "esp8266.h"

/**
 * 函数作用：清空ESP8266接收数据的缓冲区
 * 无传递参数
 * 无返回值
*/
void ESP8266_Clear(void)
{
    memset(uart3_message.rx_buf, 0, sizeof(uart3_message.rx_buf));
    uart3_message.rx_finsh = false;
    uart3_message.rx_len = 0;
}

/**
 * 函数作用：给ESP8266传AT指令
 * 参数：cmd  需要发送的指令
 *      res  期待的返回值
 * 返回值：1 失败
 *        0 成功
*/
uint8_t ESP8266_SendCMD(uint8_t *cmd, uint8_t *res)
{
    uint16_t waitTime = 200;
    HAL_UART_Transmit(&usart3_hander, cmd, strlen((const char*)cmd), 200);
    while(waitTime--)
    {
        if(uart3_message.rx_finsh == true)
        {
            if(strstr((const char*)uart3_message.rx_buf, (const char*)res) != NULL)
            {
                ESP8266_Clear();
                return 0;
            }
        }
        HAL_Delay(10);
    }
    return 1;
}

/**
 * 函数作用：使用ESP8266发送数据
 * 参数： data 需要发送的数据
 *       len 发送数据的长度
 * 无返回值
*/
void ESP8266_SendData(uint8_t *data, uint16_t len)
{
    uint8_t txbuff[50];
    ESP8266_Clear();
    sprintf((char*)txbuff, "AT+CIPSEND=%d\r\n", len);

    if(!ESP8266_SendCMD(txbuff, (uint8_t*)"OK"))
    {
        HAL_UART_Transmit(&usart3_hander, data, len, 200);
    }
}


/**
 * 函数作用：初始化ESP8266并绑定OneNet
 * 无传递参数
 * 无返回值
*/
void ESP8266_Init(void)
{
    uint8_t p[50];

    ESP8266_Clear();
    // printf("AT start\r\n");
    while(ESP8266_SendCMD((uint8_t*)"AT\r\n", (uint8_t*)"OK"));
    // printf("AT end\r\n");
    HAL_Delay(2000);
    
    ESP8266_Clear();
    // printf("CWMODE start\r\n");
    while(ESP8266_SendCMD((uint8_t*)"AT+CWMODE=1\r\n", (uint8_t*)"OK"));
    // printf("CWMODE end\r\n");
    HAL_Delay(2000);

    ESP8266_Clear();
    // printf("CWDHCP start\r\n");
    while(ESP8266_SendCMD((uint8_t*)"AT+CWDHCP=1,1\r\n", (uint8_t*)"OK"));
    // printf("CWDHCP end\r\n");
    HAL_Delay(2000);

    ESP8266_Clear();
    // printf("CWJAP start\r\n");
    sprintf((char*)p, "AT+CWJAP=\"%s\",\"%s\"\r\n", WIFISTA_SSID, WIFISTA_PASSWORD);
    while(ESP8266_SendCMD((uint8_t*)p, (uint8_t*)"WIFI GOT IP"));
    // printf("CWJAP end\r\n");
    HAL_Delay(2000);
    
    ESP8266_Clear();
    // printf("CIPSTART start\r\n");
    sprintf((char*)p, "AT+CIPSTART=\"TCP\",\"%s\",%s\r\n", SERVER_IP, SERVER_PORT);
    while(ESP8266_SendCMD((uint8_t*)p, (uint8_t*)"CONNECT"));
    // printf("CIPSTART end\r\n");
    HAL_Delay(2000);

	ESP8266_Clear();
    while(ESP8266_SendCMD((uint8_t*)"ATE0\r\n", (uint8_t*)"OK"));
    HAL_Delay(2000);

    ESP8266_Clear();
    while(ESP8266_SendCMD((uint8_t*)"AT+CIPMODE=0\r\n", (uint8_t*)"OK"));
    HAL_Delay(2000);

	

    printf("ESP8266 Init OK\r\n");
}

