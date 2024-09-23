#ifndef __IAP_H
#define __IAP_H


#include "stm32f4xx_hal.h"
#include "flash.h"

#define FLASH_APP_ADDR 0x08010000

typedef  void (*iapfun)(void);

void write_iap_app(uint32_t addr, uint8_t* buf, uint32_t size);
void jump_app(uint32_t addr);
void MSR_MSP(uint32_t addr);	//设置堆栈地址 
#endif
