#include "iic.h"

void iic_delay(uint16_t i) {
    while(i--);
}

void iic_init(void) {
    __HAL_RCC_GPIOH_CLK_ENABLE();
    iic_scl_init();
    iic_sda_out_init();
    HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET);
}

void iic_scl_init(void) {
    GPIO_InitTypeDef gpio_handler;
    gpio_handler.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_handler.Pin = SCL_PIN;
    gpio_handler.Pull = GPIO_PULLUP;
    gpio_handler.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(SCL_PORT, &gpio_handler);
}

void iic_sda_out_init(void) {
    GPIO_InitTypeDef gpio_handler;
    HAL_GPIO_DeInit(SDA_PORT, SDA_PIN);
    gpio_handler.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_handler.Pin = SDA_PIN;
    gpio_handler.Pull = GPIO_PULLUP;
    gpio_handler.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(SDA_PORT, &gpio_handler);
}

void iic_sda_in_init(void) {
    GPIO_InitTypeDef gpio_handler;
    HAL_GPIO_DeInit(SDA_PORT, SDA_PIN);
    gpio_handler.Mode = GPIO_MODE_INPUT;
    gpio_handler.Pin = SDA_PIN;
    // gpio_handler.Pull = GPIO_PULLUP;
    gpio_handler.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(SDA_PORT, &gpio_handler);
}

void iic_start(void) {
    iic_sda_out_init();
    HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET);
    iic_delay(50);
    HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_RESET);
    iic_delay(50);
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET);
}

void iic_stop(void) {
    iic_sda_out_init();
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_RESET);
    iic_delay(50);
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET);
    iic_delay(50);
    HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET);
}


// 等待应答信号到来
// 返回值： 1，接收应答失败
//         0，接收应答成功
uint8_t iic_wait_ack(void) {
    uint8_t count = 0;
    iic_sda_out_init();
    HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET);
    iic_delay(50);
    iic_sda_in_init();
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET);
    iic_delay(50);
    while(HAL_GPIO_ReadPin(SDA_PORT, SDA_PIN)) {
        count++;
        if(count > 250) {
            iic_stop();
            return 1;
        }
    }
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET);
    return 0;
}

// 产生ack应答
void iic_ack(void) {
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET);
    iic_sda_out_init();
    HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_RESET);
    iic_delay(50);
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET);
    iic_delay(50);
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET);
}

// 不产生ack应答
void iic_nack(void) {
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET);
    iic_sda_out_init();
    HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET);
    iic_delay(50);
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET);
    iic_delay(50);
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET);
}

// iic发送一个字节
void iic_send_byte(uint8_t byte) {
    uint8_t t;
    iic_sda_out_init();
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET);
    for(t = 0; t < 8; t++) {
        if((byte & 0x80) >> 7) {
            HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET);
        }
        else {
            HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_RESET);
        }
        byte <<= 1;
        iic_delay(50);
        HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET);
        iic_delay(50);
        HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET);
        iic_delay(50);
    }
}

// 读一个字节 ack=1 发送ack  ack=0 发送nack
uint8_t iic_read_byte(uint8_t ack) {
    uint8_t i = 0, res = 0;
    iic_sda_in_init();
    for(i = 0; i < 8; i++) {
        HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET);
        iic_delay(50);
        HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET);
        res <<= 1;
        if(HAL_GPIO_ReadPin(SDA_PORT, SDA_PIN)) {
            res++;
        }
        iic_delay(125);
    }
    if(ack) {
        iic_ack();
    }
    else {
        iic_nack();
    }
    return res;
}

