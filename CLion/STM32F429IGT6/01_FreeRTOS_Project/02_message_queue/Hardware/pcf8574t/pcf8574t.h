#ifndef __PCF8574T_H
#define __PCF8574T_H

#include "iic.h"

#define IIC_INT_PORT GPIOB
#define IIC_INT_PIN  GPIO_PIN_12

#define PCF8574_ADDR 	0X40	//PCF8574地址(左移了一位)

//PCF8574各个IO的功能
#define BEEP_IO         0		//蜂鸣器控制引脚  	P0
#define AP_INT_IO       1   	//AP3216C中断引脚	P1
#define DCMI_PWDN_IO    2    	//DCMI的电源控制引脚	P2
#define USB_PWR_IO      3    	//USB电源控制引脚	    P3
#define EX_IO      		4    	//扩展IO,自定义使用 	P4
#define MPU_INT_IO      5   	//MPU9250中断引脚	P5
#define RS485_RE_IO     6    	//RS485_RE引脚		P6
#define ETH_RESET_IO    7    	//以太网复位引脚		P7


uint8_t pcf8574_init(void);
uint8_t pcf8574_read_byte(void);
void pcf8574_write_byte(uint8_t data);
void pcf8574_write_bit(uint8_t bit, uint8_t data);
uint8_t pcf8574_read_bit(uint8_t bit);

#endif
