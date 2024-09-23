#ifndef __PCF8574_H
#define __PCF8574_H
#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "iic.h"

#define PCF8574_IIC_INT_PORT  GPIOB
#define PCF8574_IIC_INT_PIN   GPIO_PIN_12
#define PCF8574_ADDR 	0X40

#define BEEP_IO         0	
#define AP_INT_IO       1   
#define DCMI_PWDN_IO    2    
#define USB_PWR_IO      3    
#define EX_IO      		4    
#define MPU_INT_IO      5   
#define RS485_RE_IO     6    
#define ETH_RESET_IO    7    

uint8_t pcf8574_init(void);
uint8_t pcf8574_read_one_byte(void);
void pcf8574_write_one_byte(uint8_t data);
void pcf8574_write_bit(uint8_t bit, uint8_t sta);
uint8_t pcf8574_read_bit(uint8_t bit);

#ifdef __cplusplus
}
#endif
#endif
