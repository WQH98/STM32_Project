#include "spi.h"

SPI_HandleTypeDef spi1_handler;

void spi1_init(void) {

    spi1_handler.Instance = SPI1;        // SPI1
    spi1_handler.Init.Mode = SPI_MODE_MASTER;      // 设置SPI工作模式为主机模式
    spi1_handler.Init.Direction = SPI_DIRECTION_2LINES;  // 设置SPI单向或双向的数据模式 设置为双线模式
    spi1_handler.Init.DataSize = SPI_DATASIZE_8BIT;    // 设置SPI的数据大小 设置发送接收都为8位帧结构
    spi1_handler.Init.CLKPolarity = SPI_POLARITY_HIGH;  // 串行同步时钟的空闲状态是高电平
    spi1_handler.Init.CLKPhase = SPI_PHASE_2EDGE;   // 串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    spi1_handler.Init.NSS = SPI_NSS_SOFT;  // NSS信号由软件控制 
    spi1_handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;  // 定义波特率预分频的值为2
    spi1_handler.Init.FirstBit = SPI_FIRSTBIT_MSB;   // 指定数据传输位从MSB开始
    spi1_handler.Init.TIMode = SPI_TIMODE_DISABLE;   // 关闭TI模式
    spi1_handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;  // 关闭硬件CRC计算
    spi1_handler.Init.CRCPolynomial = 7;   // CRC值计算的多项式
    HAL_SPI_Init(&spi1_handler);        // 初始化SPI1
    __HAL_SPI_ENABLE(&spi1_handler);   // 使能SPI1
    spi1_read_write_byte(0xFF);
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi) {
    GPIO_InitTypeDef gpio_handler;
    if (hspi->Instance == SPI1) { 

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_SPI1_CLK_ENABLE();

        gpio_handler.Mode = GPIO_MODE_AF_PP;
        gpio_handler.Pin = SPI1_SCK_PIN | SPI1_MOSI_PIN;
        gpio_handler.Pull = GPIO_PULLUP;
        gpio_handler.Speed = GPIO_SPEED_HIGH;
        HAL_GPIO_Init(GPIOA, &gpio_handler);
		
		gpio_handler.Mode = GPIO_MODE_AF_INPUT;
        gpio_handler.Pin = SPI1_MISO_PIN;
		HAL_GPIO_Init(GPIOA, &gpio_handler);

        gpio_handler.Mode = GPIO_MODE_OUTPUT_PP;
        gpio_handler.Pin = SPI1_CS_PIN;
        HAL_GPIO_Init(GPIOA, &gpio_handler);
    }
}

// SPI1读写一个字节
uint8_t spi1_read_write_byte(uint8_t tx_data) {
    uint8_t rx_data;
    HAL_SPI_TransmitReceive(&spi1_handler, &tx_data, &rx_data, 1, 1000);
    return rx_data;
}

// SPI CS 拉高
void spi1_cs_high(void) {
    HAL_GPIO_WritePin(SPI1_CS_PORT, SPI1_CS_PIN, GPIO_PIN_SET);
}

// SPI CS 拉低
void spi1_cs_low(void) {
    HAL_GPIO_WritePin(SPI1_CS_PORT, SPI1_CS_PIN, GPIO_PIN_RESET);
}
