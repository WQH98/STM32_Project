#include "esp8266.h"

int ESP8266_RecCntPre = 0;

/* 函数作用：清空ESP8266接收数据的缓冲区
 * 无传参
 * 无返回值
*/
void ESP8266_Clear(void)
{
	memset(USART3_RX_BUF, 0 , sizeof(USART3_RX_BUF));
	USART3_RX_STA = 0;
	USART3_RX_STA_SIGN = 0;
}


/* 函数作用：给ESP8266传AT指令
 * 传参： cmd 需要给模块传的指令   
 *        res 期待的返回值
 * 返回值：1 失败
 *        0 成功
*/
uint8_t ESP8266_SendCMD(uint8_t *cmd, uint8_t *res)
{
	uint8_t timeout = 200;
	u3_printf("%s", cmd);
	while(timeout--)
	{
        if(USART3_RX_STA_SIGN == 1)
		{
			if(strstr((const char*)USART3_RX_BUF, (const char*)res) != NULL)
			{
				ESP8266_Clear();
				return 0;
			}
		}
		HAL_Delay(10);
	}
	return 1;
}

/* 函数作用：进入透传发送数据
 * 传参：data  需要发送的数据
 *       len   发送数据的长度
 * 无返回值
*/
void ESP8266_SendData(uint8_t *data, uint16_t len)
{
	char txbuff[50];
	ESP8266_Clear();
	sprintf(txbuff, "AT+CIPSEND=%d\r\n", len);
	if(!ESP8266_SendCMD((uint8_t*)txbuff, (uint8_t*)"OK"))
	{
		//HAL_UART_Transmit(&USART1_Handler, data, len, 200);
		HAL_UART_Transmit(&USART3_Handler, data, len, 200);
	}
}

/* 函数名称：初始化ESP8266绑定oneNET
 * 无传参
 * 无返回值
*/
void ESP8266_Init(void)
{
	ESP8266_Clear();
	printf("AT start\r\n");
	while(ESP8266_SendCMD((uint8_t*)"AT\r\n", (uint8_t*)"OK"));
	printf("AT END\r\n");
	HAL_Delay(2000);
	
	printf("CWMODE start\r\n");
	while(ESP8266_SendCMD((uint8_t*)"AT+CWMODE=1\r\n", (uint8_t*)"OK"));
	printf("CWMODE END\r\n");
	HAL_Delay(2000);
	
	printf("CWDHCP start\r\n");
	while(ESP8266_SendCMD((uint8_t*)"AT+CWDHCP=1,1\r\n", (uint8_t*)"OK"));
	printf("CWDHCP END\r\n");
	HAL_Delay(2000);
	
	printf("CWJAP start\r\n");
	while(ESP8266_SendCMD((uint8_t*)"AT+CWJAP=\"WQH\",\"12345678\"\r\n", (uint8_t*)"WIFI GOT IP"));
	printf("CWJAP END\r\n");
	HAL_Delay(2000);
	
	printf("CIPSTART start\r\n");
	while(ESP8266_SendCMD((uint8_t*)"AT+CIPSTART=\"TCP\",\"183.230.40.39\",6002\r\n", (uint8_t*)"CONNECT"));
	printf("CIPSTART END\r\n");
	HAL_Delay(2000);
	
	printf("ESP8266 Init OK\r\n");
}

/* 函数作用：返回传入数组中IPD:后面的内容
 * 传参：timeout 超时时间
 * 返回值：IPD:后面的指针
*/
uint8_t *ESP8266_GetIPD(uint16_t timeout)
{
    uint8_t *pIPD = NULL;
    do
    {
        if(USART3_RX_STA_SIGN == 1)
        {
            // strstr返回字符“IPD”在USART3_RX_BUF中的位置 未找到的话返回NULL
            pIPD = strstr((char *)USART3_RX_BUF, "IPD");
            if(pIPD != NULL)
            {
                // strchr函数返回字符‘:’在pIPD中的位置 并返回指针 未找到该字符就返回NULL
                pIPD = strchr(pIPD, ':');
                if(pIPD != NULL)
                {
                    // pIPD++ 表示将指针的位置加1 也就是字符‘:’的下一个位置
                    pIPD++;
                    return (uint8_t*)pIPD;
                }
                else
                    // 没找到字符':'
                    return NULL;
            }
            USART3_RX_STA_SIGN = 0;
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    } while(timeout--);
    //时间耗尽 返回空指针
    return NULL;
}

/* 函数作用：解析绑定数据
 * 无传参
 * 返回值：返回应该上传的绑定信息
*/
uint8_t *Bind_OneNet(void)
{
    static uint8_t connect_buff[200];
	memset(connect_buff, 0, 200);
    uint8_t cnt = 2;
    uint8_t *MQTT = "MQTT";
    uint16_t cTime = 120;
    uint8_t *devid = DEVICE_ID;
    uint8_t *username = PRODECT_ID;
    uint8_t *password = PASSWORD_ID;
    connect_buff[0] = 0x10;
    connect_buff[cnt++] = strlen(MQTT) >> 8;
    connect_buff[cnt++] = strlen(MQTT);
    connect_buff[cnt++] = 'M';
    connect_buff[cnt++] = 'Q';
    connect_buff[cnt++] = 'T';
    connect_buff[cnt++] = 'T';
    connect_buff[cnt++] = 0x04;
    connect_buff[cnt++] = 0xC2;
    connect_buff[cnt++] = (cTime & 0xFF00) >> 8;
    connect_buff[cnt++] = (cTime & 0x00FF);
    connect_buff[cnt++] = strlen((const char*)devid) >> 8;
    connect_buff[cnt++] = strlen((const char*)devid);
    for(int i = 0; i < strlen((const char*)devid); i++)
    {
        connect_buff[cnt++] = *(devid + i);
    }
    connect_buff[cnt++] = strlen((const char*)username) >> 8;
    connect_buff[cnt++] = strlen((const char*)username);
    for(int i = 0; i < strlen((const char*)username); i++)
    {
        connect_buff[cnt++] = *(username + i);
    }
    connect_buff[cnt++] = strlen((const char*)password) >> 8;
    connect_buff[cnt++] = strlen((const char*)password);
    for(int i = 0; i < strlen((const char*)password); i++)
    {
        connect_buff[cnt++] = *(password + i);
    }
    connect_buff[1] = (cnt-2);
	
    return connect_buff;
}

/* 函数作用：打包向OneNet平台上传的数据数组
 * 传参：topic 数据流名字
 *       message  需要发送的消息
 * 返回：打包好的数据信息数组
*/
uint8_t *OneNet_SendMessage(uint8_t *topic, uint8_t *message)
{	
    //static uint8_t senddata[100] = {0x30, 0x23, 0x00, 0x03, 0x24, 0x64, 0x70, 0x02, 0x00, 0x12, 0x7B, 0x20, 0x22, 0x64, 0x73, 0x5F, 0x69, 0x64, 0x22, 0x3A, 0x20, 0x22, 0x54, 0x65, 0x73, 0x74, 0x22, 0x7D, 0x00, 0x00, 0x00, 0x05, 0x31, 0x32, 0x33, 0x34, 0x37};
    static uint8_t senddata[100];
    memset(senddata, 0, 100);
    uint16_t cnt = 2;
    uint8_t topic_len = strlen((const char*)topic);
    uint32_t message_len = strlen((const char*)message);
    uint8_t JosnStrlen = 14 + topic_len;
    senddata[0] = 0x30;
    senddata[cnt++] = 0x00;
    senddata[cnt++] = 0x03;
    senddata[cnt++] = 0x24;
    senddata[cnt++] = 0x64;
    senddata[cnt++] = 0x70;
    senddata[cnt++] = 0x02;
    senddata[cnt++] = (JosnStrlen&0xFF00) >> 8;
    senddata[cnt++] = (JosnStrlen&0x00FF);
    senddata[cnt++] = 0x7B;
    senddata[cnt++] = 0x20;
    senddata[cnt++] = 0x22;
    senddata[cnt++] = 0x64;
    senddata[cnt++] = 0x73;
    senddata[cnt++] = 0x5F;
    senddata[cnt++] = 0x69;
    senddata[cnt++] = 0x64;
    senddata[cnt++] = 0x22;
    senddata[cnt++] = 0x3A;
    senddata[cnt++] = 0x20;
    senddata[cnt++] = 0x22;
    for(int i = 0; i < topic_len; i++)
    {
        senddata[cnt++] = *(topic + i);
    }
    senddata[cnt++] = 0x22;
    senddata[cnt++] = 0x7D;
    senddata[cnt++] = (message_len&0xFF000000) >> 24;
    senddata[cnt++] = (message_len&0x00FF0000) >> 16;
    senddata[cnt++] = (message_len&0x0000FF00) >> 8;
    senddata[cnt++] = (message_len&0x000000FF);
    for(int i = 0; i < message_len; i++)
    {
        senddata[cnt++] = *(message + i);
    }
    senddata[1] = (cnt-2);

	return senddata;
}

/* 函数作用：解析收到的OneNet的指令
 * 参数：收到的指令
 * 返回：NULL 未接收到正确指令
 *      
 *       其他：解析好的指令指针
*/
char *OneNetDataAnalysis(uint8_t *recdata)
{
    char *data;
    //recdata = "+IPD,55:05*$creq/af506f6f-85ad-57e4-af8b-69e7f61768b3123123123";
    //printf("%s\r\n", recdata);
    uint32_t len;
    data = strchr((const char *)recdata, 0x2F);
    if(data == NULL)
    {
		printf("not found\r\n");
        return NULL;
    }
    data+=36;
    printf("data = %s\r\n", data);
    return data;
}