#include "esp8266.h"

/**
 * 函数作用：清空ESP8266接收数据的缓冲区
 * 无传递参数
 * 无返回值
*/
void ESP8266_Clear(void)
{
    memset(usart3_message.rx_buf, 0, sizeof(usart3_message.rx_buf));
    usart3_message.rx_finsh = false;
    usart3_message.rx_len = 0;
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
    HAL_UART_Transmit(&USART3_Handler, cmd, strlen((const char*)cmd), 200);
    while(waitTime--)
    {
        if(usart3_message.rx_finsh == true)
        {
            if(strstr((const char*)usart3_message.rx_buf, (const char*)res) != NULL)
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
 * 函数作用：使用ESP8266进入透传发送数据
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
        HAL_UART_Transmit(&USART3_Handler, data, len, 200);
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
    printf("AT start\r\n");
    while(ESP8266_SendCMD((uint8_t*)"AT\r\n", (uint8_t*)"OK"));
    printf("AT end\r\n");
    HAL_Delay(2000);
    
    ESP8266_Clear();
    printf("CWMODE start\r\n");
    while(ESP8266_SendCMD((uint8_t*)"AT+CWMODE=1\r\n", (uint8_t*)"OK"));
    printf("CWMODE end\r\n");
    HAL_Delay(2000);

    ESP8266_Clear();
    printf("CWDHCP start\r\n");
    while(ESP8266_SendCMD((uint8_t*)"AT+CWDHCP=1,1\r\n", (uint8_t*)"OK"));
    printf("CWDHCP end\r\n");
    HAL_Delay(2000);

    ESP8266_Clear();
    printf("CWJAP start\r\n");
    sprintf((char*)p, "AT+CWJAP=\"%s\",\"%s\"\r\n", WIFISTA_SSID, WIFISTA_PASSWORD);
    while(ESP8266_SendCMD((uint8_t*)p, (uint8_t*)"WIFI GOT IP"));
    printf("CWJAP end\r\n");
    HAL_Delay(2000);
    
    ESP8266_Clear();
    printf("CIPSTART start\r\n");
    sprintf((char*)p, "AT+CIPSTART=\"TCP\",\"%s\",%s\r\n", SERVER_IP, SERVER_PORT);
    while(ESP8266_SendCMD((uint8_t*)p, (uint8_t*)"CONNECT"));
    printf("CIPSTART end\r\n");
    HAL_Delay(2000);
    
    printf("ESP8266 Init OK\r\n");
}

/**
 * 函数作用：解析传入数据IPD后面的数据
 * 参数：waittime 等待超时时间 单位10ms
 * 返回值： IPD后面的指针
			NULL 未找到
*/
uint8_t *ESP8266_GetIPD(uint16_t waittime)
{
    uint8_t *p = NULL;
    do
    {
        if(usart3_message.rx_finsh == true)
        {
            // strstr函数 返回字符"IPD"在usart3_message.rx_buf中的位置 未找到则返回NULL
            p = (uint8_t*)strstr((const char*)usart3_message.rx_buf, (const char*)"IPD");
            if(p != NULL)
            {
                // strchr函数 返回字符':'在p中的位置 并返回指针 未找到就返回NULL
				p = (uint8_t*)strchr((const char*)p, ':');
                if(p != NULL)
                {
                    // 将P指针后移一个位置 也就是字符':'的下一个位置
                    p++;
                    return p;
                }
                else
                {
                    // 没有找到字符':'
                    return NULL;
                }
            }
            ESP8266_Clear();
        }
        HAL_Delay(10);
    }while(waittime--);
    return NULL;
}

/**
 * 函数作用：得到连接状态
 * 参数：waittime 等待数据穿回来的时间 单位10ms
 * 返回值： 1，255  失败
 *          其他   平台的返回码
*/
uint8_t Connect_Sta(uint16_t waittime)
{
    uint8_t *sta = ESP8266_GetIPD(waittime);
    if(sta[1] != 2)
        return 1;
    if(sta[2] == 0 || sta[2] == 1)
    {
        return sta[3];
    }
    else 
        return 255;
}

/**
 * 函数作用：得到OneNet的连接结果 并且打印
 * 参数：调用Connect_Sta函数的返回码
 * 无返回值
*/
void Get_Connect_Sta(uint8_t sta)
{
    switch(sta)
    {
        case 0: printf("Tips：连接成功\r\n"); break;
        case 1: printf("WARN：连接失败：协议错误\r\n"); break;
        case 2: printf("WARN：连接失败：非法的clientid\r\n"); break;
        case 3: printf("WARN：连接失败：服务器失败\r\n"); break;
        case 4: printf("WARN：连接失败：用户名或密码错误\r\n"); break;
        case 5: printf("WARN：连接失败：非法链接(比如token非法)\r\n"); break;
        default: printf("WARN：连接失败：未知错误\r\n"); break;
    }
}

/**
 * 函数作用：根据MQTT协议规定组包
 * 参数：deviceid 设备ID
 *       prodectid 产品ID
 *      passwordid 鉴权ID
 *      retention_time 连接而不被断开的时间
 *      mqtt_Packet MQTT组包结构体
 * 返回值：0 正常
 *        其他值 组包失败
*/
uint8_t Bind_OneNet(const uint8_t *deviceid, const uint8_t *prodectid, const uint8_t* passwordid,
                    uint16_t retention_time, MQTT_Data *mqtt_Packet)
{
    //设备ID长度 产品ID长度 鉴权ID长度 可变头部的标志位
    uint8_t flags = 0;
    uint16_t deviceid_len = strlen((const char*)deviceid);
    uint16_t prodectid_len = strlen((const char*)prodectid);
    uint16_t passwordid_len = strlen((const char*)passwordid);
    flags |= 1 << 7;        //username_flag = 1
    flags |= 1 << 6;        //password_flag = 1
    flags |= 1 << 1;        //cleansession_flag = 1
    memset(mqtt_Packet->data_buf, 0, mqtt_Packet->data_len);
    mqtt_Packet->data_len = 0;
    // ------------------------固定头部----------------------------------------
    mqtt_Packet->data_buf[mqtt_Packet->data_len++] = 0x10; //客户端到服务器的connect消息
    mqtt_Packet->data_buf[mqtt_Packet->data_len++] = 0;    //剩余长度 最后再写
    // ------------------------可变头部----------------------------------------
    mqtt_Packet->data_buf[mqtt_Packet->data_len++] = 0;    //协议名长度的高8位
    mqtt_Packet->data_buf[mqtt_Packet->data_len++] = 4;    //协议名长度的低8位
    mqtt_Packet->data_buf[mqtt_Packet->data_len++] = 'M';  //协议名
    mqtt_Packet->data_buf[mqtt_Packet->data_len++] = 'Q';
    mqtt_Packet->data_buf[mqtt_Packet->data_len++] = 'T';
    mqtt_Packet->data_buf[mqtt_Packet->data_len++] = 'T';
    mqtt_Packet->data_buf[mqtt_Packet->data_len++] = 0x04;  //协议级别字段
    mqtt_Packet->data_buf[mqtt_Packet->data_len++] = flags; //一些连接标志
    mqtt_Packet->data_buf[mqtt_Packet->data_len++] = (retention_time&0xFF00) >> 8;  //保持连接时间的高八位
    mqtt_Packet->data_buf[mqtt_Packet->data_len++] = (retention_time&0x00FF);       //保持连接时间的低八位
    // ------------------------有效荷载----------------------------------------
    mqtt_Packet->data_buf[mqtt_Packet->data_len++] = deviceid_len >> 8;   //设备ID长度高8位
    mqtt_Packet->data_buf[mqtt_Packet->data_len++] = deviceid_len;        //设备ID长度低8位
    for(int i = 0; i < deviceid_len; i++)                                 //把设备ID追加到发送数组里
    {
        mqtt_Packet->data_buf[mqtt_Packet->data_len++] = deviceid[i];     //总长度加上设备ID的长度
    } 
    
	  mqtt_Packet->data_buf[mqtt_Packet->data_len++] = prodectid_len >> 8;    //产品ID长度高8位
    mqtt_Packet->data_buf[mqtt_Packet->data_len++] = prodectid_len;         //产品ID长度低8位
    for(int i = 0; i < prodectid_len; i++)                                  //把产品ID追加到发送数组里
    {
        mqtt_Packet->data_buf[mqtt_Packet->data_len++] = prodectid[i];      //总长度加上产品ID的长度
    } 

    mqtt_Packet->data_buf[mqtt_Packet->data_len++] = passwordid_len >> 8;     //鉴权ID长度高8位
    mqtt_Packet->data_buf[mqtt_Packet->data_len++] = passwordid_len;          //鉴权ID长度低8位
    for(int i = 0; i < passwordid_len; i++)                                   //把产品ID追加到发送数组里
    {
        mqtt_Packet->data_buf[mqtt_Packet->data_len++] = passwordid[i];       //总长度加上产品ID的长度
    } 
    mqtt_Packet->data_buf[1] = mqtt_Packet->data_len - 2;

    return 0;
}

