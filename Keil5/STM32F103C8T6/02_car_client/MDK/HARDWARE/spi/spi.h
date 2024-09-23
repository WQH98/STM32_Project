#ifndef __SPI_H
#define __SPI_H

#include "stm32f1xx_hal.h"

#define SPI1_CS_PORT   GPIOA
#define SPI1_CS_PIN    GPIO_PIN_4
#define SPI1_SCK_PORT  GPIOA
#define SPI1_SCK_PIN   GPIO_PIN_5
#define SPI1_MISO_PORT GPIOA
#define SPI1_MISO_PIN  GPIO_PIN_6
#define SPI1_MOSI_PORT GPIOA
#define SPI1_MOSI_PIN  GPIO_PIN_7

void spi1_init(void);
uint8_t spi1_read_write_byte(uint8_t tx_data);
void spi1_cs_high(void);
void spi1_cs_low(void);

#endif
