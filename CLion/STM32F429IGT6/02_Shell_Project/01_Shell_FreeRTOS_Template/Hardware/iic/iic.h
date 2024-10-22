#ifndef __IIC_H
#define __IIC_H

#include "stm32f4xx_hal.h"

#define SCL_PORT GPIOH
#define SCL_PIN  GPIO_PIN_4
#define SDA_PORT GPIOH
#define SDA_PIN  GPIO_PIN_5

void iic_init(void);
void iic_scl_init(void);
void iic_sda_out_init(void);
void iic_sda_in_init(void);
void iic_start(void);
void iic_stop(void);
uint8_t iic_wait_ack(void);
void iic_ack(void);
void iic_nack(void);
void iic_send_byte(uint8_t byte);
uint8_t iic_read_byte(uint8_t ack);

#endif
