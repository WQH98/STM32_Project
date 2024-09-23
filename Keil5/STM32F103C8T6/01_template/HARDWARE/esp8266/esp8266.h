#ifndef __ESP8266_H
#define __ESP8266_H

#include "uart.h"

#define WIFISTA_SSID 	   "WQH"
#define WIFISTA_PASSWORD   "12345678"
#define SERVER_IP          "10.210.19.127"
#define SERVER_PORT		   "8000"

void ESP8266_Init(void);
void ESP8266_Clear(void);
uint8_t ESP8266_SendCMD(uint8_t *cmd, uint8_t *res);
void ESP8266_SendData(uint8_t *data, uint16_t len);


#endif
