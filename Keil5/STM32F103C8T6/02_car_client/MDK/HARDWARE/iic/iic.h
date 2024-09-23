#ifndef __IIC_H
#define __IIC_H

#include "stm32f1xx_hal.h"

#define IIC_SCL_PORT  GPIOB
#define IIC_SCL_PIN   GPIO_PIN_8
#define IIC_SDA_PORT  GPIOB
#define IIC_SDA_PIN   GPIO_PIN_9

void iic_init(void);
void iic_start(void);
void iic_stop(void);
uint8_t iic_wait_ack(void);
void iic_ack(void);
void iic_nack(void);
void iic_send_byte(uint8_t byte);
uint8_t iic_read_byte(uint8_t ack);

#endif
