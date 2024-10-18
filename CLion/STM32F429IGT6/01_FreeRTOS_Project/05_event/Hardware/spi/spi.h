#ifndef __SPI_H
#define __SPI_H

#include "stm32f4xx_hal.h"

#define SPI5_SCK_PORT  GPIOF
#define SPI5_SCK_PIN   GPIO_PIN_7
#define SPI5_MISO_PORT GPIOF
#define SPI5_MISO_PIN  GPIO_PIN_8
#define SPI5_MOSI_PORT GPIOF
#define SPI5_MOSI_PIN  GPIO_PIN_9

void spi5_init(void);
void spi5_set_speed(uint8_t speed);
uint8_t spi5_read_write_byte(uint8_t data);

#endif
