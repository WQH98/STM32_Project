#include "pcf8574t.h"

uint8_t pcf8574_init(void) {
    uint8_t temp = 0;
    GPIO_InitTypeDef gpio_handler;
    __HAL_RCC_GPIOB_CLK_ENABLE();

    gpio_handler.Mode = GPIO_MODE_INPUT;
    gpio_handler.Pin = IIC_INT_PIN;
    gpio_handler.Pull = GPIO_PULLUP;
    gpio_handler.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(IIC_INT_PORT, &gpio_handler);

    iic_init();
    // 检查pcf8574t是否连接正常
    iic_start();
    iic_send_byte(PCF8574_ADDR);
    temp = iic_wait_ack();
    iic_stop();
    pcf8574_write_byte(0xFF);
    return temp;
}

// 读取PCF8574T的8位数据
// 返回值：PCF8574T的8位数据
uint8_t pcf8574_read_byte(void) {
    uint8_t temp = 0;
    iic_start();
    iic_send_byte(PCF8574_ADDR | 0x01);
    iic_wait_ack();
    temp = iic_read_byte(0);
    iic_stop();
    return temp;
}

// 写入PCF8574T的8位数据
// data：要写入的数据
void pcf8574_write_byte(uint8_t data) {
    iic_start();
    iic_send_byte(PCF8574_ADDR | 0x00);
    iic_wait_ack();
    iic_send_byte(data);
    iic_wait_ack();
    iic_stop();
    HAL_Delay(10);
}

// 设置PCF8574T的某一位的高低电平
// bit: IO的状态 编号 0~7
// data: IO的状态 0 or 1
void pcf8574_write_bit(uint8_t bit, uint8_t data) {
    uint8_t temp;
    temp = pcf8574_read_byte();
    if(data == 0) {
        temp &= ~(1 << bit);
    }
    else {
        temp |= 1 << bit;
    }
    pcf8574_write_byte(temp);
}

// 读取PCF8574T的某一位IO的值
// bit: 要读取的IO编号 0~7
// 返回值: 此IO的值 0 or 1
uint8_t pcf8574_read_bit(uint8_t bit) {
    uint8_t data;
    data = pcf8574_read_byte();
    if(data & (1 << bit)) {
        return 1;
    }
    else {
        return 0;
    }
}
