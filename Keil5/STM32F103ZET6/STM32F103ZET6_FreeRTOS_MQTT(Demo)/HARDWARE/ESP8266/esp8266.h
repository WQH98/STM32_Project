#ifndef __ESP8266_H
#define __ESP8266_H

#include "stm32f1xx_hal.h"
#include "usart.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"

#define DEVICE_ID    "1059918589"  //设备ID
#define PRODECT_ID   "583480"      //产品ID
#define PASSWORD_ID  "11111111"    //鉴权ID


void ESP8266_Clear(void);
uint8_t ESP8266_SendCMD(uint8_t *cmd, uint8_t *res);
void ESP8266_SendData(uint8_t *data, uint16_t len);
void ESP8266_Init(void);
uint8_t *ESP8266_GetIPD(uint16_t timeout);
uint8_t *Bind_OneNet(void);
uint8_t *OneNet_SendMessage(uint8_t *topic, uint8_t *message);
char *OneNetDataAnalysis(uint8_t *recdata);

#endif
