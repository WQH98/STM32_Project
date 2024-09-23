#include "iic.h"

void iic_delay(uint32_t i) {
    while(i--);
}

void sda_high() {
	HAL_GPIO_WritePin(IIC_SDA_PORT, IIC_SDA_PIN, GPIO_PIN_SET);
}

void sda_low() {
	HAL_GPIO_WritePin(IIC_SDA_PORT, IIC_SDA_PIN, GPIO_PIN_RESET);
}

void scl_high() {
	HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_SET);
}

void scl_low() {
	HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_RESET);
}

char read_sda() {
    return (char)HAL_GPIO_ReadPin(IIC_SDA_PORT, IIC_SDA_PIN);
}

void sda_out() {
	GPIO_InitTypeDef gpio_handler;
	gpio_handler.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_handler.Pin = IIC_SDA_PIN;
	gpio_handler.Pull = GPIO_PULLUP;
	gpio_handler.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOB, &gpio_handler);
}

void sda_in() {
	GPIO_InitTypeDef gpio_handler;
	gpio_handler.Mode = GPIO_MODE_INPUT;
	gpio_handler.Pin = IIC_SDA_PIN;
	gpio_handler.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOB, &gpio_handler);
}

void iic_init() {

    GPIO_InitTypeDef gpio_handler;

    __HAL_RCC_GPIOB_CLK_ENABLE();

    gpio_handler.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_handler.Pin = IIC_SCL_PIN | IIC_SDA_PIN;
	gpio_handler.Pull = GPIO_PULLUP;
	gpio_handler.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOB, &gpio_handler);
	
	sda_high();
	scl_high();
}

// IIC产生起始信号
void iic_start() {
	sda_out();
	sda_high();
	scl_high();
	iic_delay(10);
    sda_low();
    iic_delay(10);
    scl_low();
}

// IIC产生停止信号
void iic_stop() {
    sda_out();
    scl_low();
    sda_low();
    iic_delay(10);
    scl_high();
    sda_high();
    iic_delay(10);
}

// 等待应答信号到来
// 返回值 1 接收应答失败
//        0 接收应答成功
uint8_t iic_wait_ack() {
    uint8_t err_time = 0;
    sda_in();
    sda_high();
    iic_delay(5);
    scl_high();
    iic_delay(5);
    while(read_sda()) {
        err_time++;
        if(err_time > 250) {
            iic_stop();
            return 1;
        }
    }
    scl_low();
    return 0;
}

// 产生ACK应答
void iic_ack() {
    scl_low();
    sda_out();
    sda_low();
    iic_delay(10);
    scl_high();
    iic_delay(10);
    scl_low();
}

// 不产生ACK应答
void iic_nack() {
    scl_low();
    sda_out();
    sda_high();
    iic_delay(10);
    scl_high();
    iic_delay(10);
    scl_low();
}

// IIC发送一个数据
void iic_send_byte(uint8_t byte) {
    uint8_t temp;
    sda_out();
    scl_low();
    for(temp = 0; temp < 8; temp++) {
        if((byte & 0x80) >> 7)
        {
            sda_high();
        }
        else {
            sda_low();
        }
        byte <<= 1;
        iic_delay(10);
        scl_high();
        iic_delay(10);
        scl_low();
        iic_delay(10);
    }
}

// 读一个字节 ack=1时 发送ack  ack=0时 发送nack
uint8_t iic_read_byte(uint8_t ack) {
    uint8_t i, receive = 0;
    sda_in();
    for(i = 0; i < 8; i++) {
        scl_low();
        iic_delay(10);
        scl_high();
        receive <<= 1;
        if(read_sda()) {
            receive++;
        }
        iic_delay(10);
	}
	if(ack) {
		iic_ack();
	}
	else {
		iic_nack();
	}
    return receive;
}

