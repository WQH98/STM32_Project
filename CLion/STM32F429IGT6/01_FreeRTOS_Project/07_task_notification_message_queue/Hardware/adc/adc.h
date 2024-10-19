#ifndef __ADC_H
#define __ADC_H


#include "stm32f4xx_hal.h"

#define ADC1_CHANNEL5_PORT   GPIOA
#define ADC1_CHANNEL5_PIN    GPIO_PIN_5

void adc1_init();
uint16_t get_adc1_value(uint32_t ch);
uint16_t get_adc1_average_value(uint32_t ch, uint8_t nums);
float get_voltage(uint16_t adc_value);

#endif
