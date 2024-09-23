#include "iic.h"

void iic_sda_out()
{
	GPIO_InitTypeDef gpio_handler;
	HAL_GPIO_DeInit(IIC_SDA_PORT, IIC_SDA_PIN);
	gpio_handler.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_handler.Pin = IIC_SDA_PIN;
	gpio_handler.Pull = GPIO_PULLUP;
	gpio_handler.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(IIC_SDA_PORT, &gpio_handler);
}

void iic_sda_in()
{
    GPIO_InitTypeDef gpio_handler;
	HAL_GPIO_DeInit(IIC_SDA_PORT, IIC_SDA_PIN);
	gpio_handler.Mode = GPIO_MODE_INPUT;
	gpio_handler.Pin = IIC_SDA_PIN;
	gpio_handler.Pull = GPIO_PULLUP;
	gpio_handler.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(IIC_SDA_PORT, &gpio_handler);
}

void iic_scl_out()
{
    GPIO_InitTypeDef gpio_handler;
	
	gpio_handler.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_handler.Pin = IIC_SCL_PIN;
	gpio_handler.Pull = GPIO_PULLUP;
	gpio_handler.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(IIC_SCL_PORT, &gpio_handler);
}

void iic_set_sda_level(uint8_t level)
{
    if(level)
    {
        HAL_GPIO_WritePin(IIC_SDA_PORT, IIC_SDA_PIN, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(IIC_SDA_PORT, IIC_SDA_PIN, GPIO_PIN_RESET);
    }
}

uint8_t iic_get_sda_level(void)
{
    return HAL_GPIO_ReadPin(IIC_SDA_PORT, IIC_SDA_PIN);
}

void iic_set_scl_level(uint8_t level)
{
    if(level)
    {
        HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_RESET);
    }
}

void iic_init()
{
    __HAL_RCC_GPIOH_CLK_ENABLE();

    iic_sda_out();
    iic_scl_out();
    
    iic_set_sda_level(1);
    iic_set_scl_level(1);
}

void iic_start()
{
    iic_sda_out();
    iic_set_sda_level(1);
    iic_set_scl_level(1);
    delay_us(4);
    iic_set_sda_level(0);
    delay_us(4);
    iic_set_scl_level(0);
}

void iic_stop()
{
    iic_sda_out();
    iic_set_scl_level(0);
    iic_set_sda_level(0);
    delay_us(4);
    iic_set_sda_level(1);
    iic_set_scl_level(1);
    delay_us(4);
}

// 等待应答信号
// 1 未收到 0 收到了
uint8_t iic_wait_ack()
{
    uint8_t cnt = 0;
    iic_set_sda_level(1);
    delay_us(1);
    iic_sda_in();
    iic_set_scl_level(1);
    delay_us(1);
    while(iic_get_sda_level())
    {
        cnt++;
        if(cnt > 250)
        {
            iic_stop();
            return 1;
        }
    }
    iic_set_scl_level(0);
    return 0;
}

void iic_ack(void)
{
    iic_set_scl_level(0);
    iic_sda_out();
    iic_set_sda_level(0);
    delay_us(2);
    iic_set_scl_level(1);
    delay_us(2);
    iic_set_scl_level(0);
}

void iic_nack(void)
{
    iic_set_scl_level(0);
    iic_sda_out();
    iic_set_sda_level(1);
    delay_us(2);
    iic_set_scl_level(1);
    delay_us(2);
    iic_set_scl_level(0);
}

void iic_send_byte(uint8_t data)
{
    int8_t i = 0;
    iic_sda_out();
    iic_set_scl_level(0);
    for(i = 0; i < 8; i++)
    {
        if((data & 0x80) >> 7)
        {
            iic_set_sda_level(1);
        }
        else
        {
            iic_set_sda_level(0);
        }
        data <<= 1;
        delay_us(2);
        iic_set_scl_level(1);
        delay_us(2);
        iic_set_scl_level(0);
        delay_us(2);
    }
}

uint8_t iic_recv_byte(uint8_t ack)
{
    uint8_t i = 0, rec_data = 0;
    iic_sda_in();
    for(i = 0; i < 8; i++)
    {
        iic_set_scl_level(0);
        delay_us(2);
        iic_set_scl_level(1);
        rec_data <<= 1;
        if(iic_get_sda_level())
        {
            rec_data++;
        }
        delay_us(1);
    }
    if(!ack)
    {
        iic_nack();
    }
    else
    {
        iic_ack();
    }
    return rec_data;
}
