#include "pcf8574.h"

uint8_t pcf8574_init(void)
{
    uint8_t temp = 0;
    GPIO_InitTypeDef gpio_handler;
    __HAL_RCC_GPIOB_CLK_ENABLE();

    gpio_handler.Mode = GPIO_MODE_INPUT;
    gpio_handler.Pin = PCF8574_IIC_INT_PIN;
    gpio_handler.Pull = GPIO_PULLUP;
    gpio_handler.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(PCF8574_IIC_INT_PORT, &gpio_handler);
    iic_init();
    iic_start();
    iic_send_byte(PCF8574_ADDR);
    temp = iic_wait_ack();
    iic_stop();
    pcf8574_write_one_byte(0xFF);
    return temp;
}

uint8_t pcf8574_read_one_byte()
{
    uint8_t temp = 0;
    iic_start();
    iic_send_byte(PCF8574_ADDR | 0x01);
    iic_wait_ack();
    temp = iic_recv_byte(0);
    iic_stop();
    return temp;
}

void pcf8574_write_one_byte(uint8_t data)
{
    iic_start();
    iic_send_byte(PCF8574_ADDR | 0x00);
    iic_wait_ack();
    iic_send_byte(data);
    iic_wait_ack();
    iic_stop();
    delay_ms(10);
}

void pcf8574_write_bit(uint8_t bit, uint8_t sta)
{
    uint8_t data = 0;
    data = pcf8574_read_one_byte();
    if(sta == 0)
    {
        data &= ~(1 << bit);
    }
    else
    {
        data |= (1 << bit);
    }
    pcf8574_write_one_byte(data);
}

uint8_t pcf8574_read_bit(uint8_t bit)
{
    uint8_t data = 0;
    data = pcf8574_read_one_byte();
    if(data & (1 << bit))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
