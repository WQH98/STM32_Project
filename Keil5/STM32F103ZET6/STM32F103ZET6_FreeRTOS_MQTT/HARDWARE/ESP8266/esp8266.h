#ifndef ESP8266_H
#define ESP8266_H

#include "stm32f1xx_hal.h"
#include "usart.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"

#define WIFISTA_SSID   "WQH"
#define WIFISTA_PASSWORD  "12345678"
#define SERVER_IP       "183.230.40.39"
//#define SERVER_IP        "192.168.137.1"
#define SERVER_PORT     "6002"
//#define SERVER_PORT       "8888"

#define DEVICE_ID    "1059918589"  //设备ID
#define PRODECT_ID   "583480"      //产品ID
#define PASSWORD_ID  "11111111"    //鉴权ID

// const uint8_t* wifista_ssid = "WQH";
// const uint8_t* wifista_password = "12345678";
// const uint8_t* server_ip = "183.230.40.39";
// const uint8_t* server_port = "6002";

typedef struct
{
    uint8_t data_buf[200];
    uint16_t data_len;
} MQTT_Data;


void ESP8266_Clear(void);
uint8_t ESP8266_SendCMD(uint8_t *cmd, uint8_t *res);
void ESP8266_SendData(uint8_t *data, uint16_t len);
void ESP8266_Init(void);
uint8_t *ESP8266_GetIPD(uint16_t waittime);
uint8_t Connect_Sta(uint16_t waittime);
void Get_Connect_Sta(uint8_t sta);
uint8_t Bind_OneNet(const uint8_t *deviceid, const uint8_t *prodectid, const uint8_t* passwordid,
                    uint16_t retention_time, MQTT_Data *mqtt_Packet);

uint8_t *OneNet_SendMessage(uint8_t *topic, uint8_t *message);
char *OneNetDataAnalysis(uint8_t *recdata);

#endif
