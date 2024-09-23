#ifndef __IIC_H
#define __IIC_H
#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "clock.h"

#define IIC_SCL_PORT    GPIOH
#define IIC_SCL_PIN     GPIO_PIN_4
#define IIC_SDA_PORT    GPIOH
#define IIC_SDA_PIN     GPIO_PIN_5

void iic_sda_out(void);
void iic_sda_in(void);
void iic_scl_out(void);
void iic_set_sda_level(uint8_t level);
void iic_set_scl_level(uint8_t level);
uint8_t iic_get_sda_level(void);
void iic_init(void);
void iic_start(void);
void iic_stop(void);
uint8_t iic_wait_ack(void);
void iic_ack(void);
void iic_nack(void);
void iic_send_byte(uint8_t data);
uint8_t iic_recv_byte(uint8_t ack);

#ifdef __cplusplus
}
#endif
#endif
