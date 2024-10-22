#include "spi.h"

SPI_HandleTypeDef spi5_handler;

void spi5_init(void) {
    spi5_handler.Instance = SPI5;
    // 设置SPI工作模式为主模式
    spi5_handler.Init.Mode = SPI_MODE_MASTER;
    // SPI设置为双线模式
    spi5_handler.Init.Direction = SPI_DIRECTION_2LINES;
    // SPI发送接收8位帧状态
    spi5_handler.Init.DataSize = SPI_DATASIZE_8BIT;
    // 同步时钟状态空闲状态为高电平
    spi5_handler.Init.CLKPolarity = SPI_POLARITY_HIGH;
    // 同步时钟第2个跳边沿数据被采样
    spi5_handler.Init.CLKPhase = SPI_PHASE_2EDGE;
    // NSS信号由硬件（NSS管脚）控制
    spi5_handler.Init.NSS = SPI_NSS_SOFT;
    // 定义波特率预分频的值 波特率预分频值为256
    spi5_handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    // 指定数据传输从MSB位开始
    spi5_handler.Init.FirstBit = SPI_FIRSTBIT_MSB;
    // 关闭TI模式
    spi5_handler.Init.TIMode = SPI_TIMODE_DISABLE;
    // 关闭CRC
    spi5_handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    // CRC值计算的多项式
    spi5_handler.Init.CRCPolynomial = 7;
    HAL_SPI_Init(&spi5_handler);
    // 使能SPI5
    __HAL_SPI_ENABLE(&spi5_handler);
    // 启动传输
    spi5_read_write_byte(0xFF);
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi) {
    if(hspi->Instance == SPI5) {
        __HAL_RCC_GPIOF_CLK_ENABLE();
        __HAL_RCC_SPI5_CLK_ENABLE();
        GPIO_InitTypeDef gpio_handler;

        gpio_handler.Alternate = GPIO_AF5_SPI5;
        gpio_handler.Mode = GPIO_MODE_AF_PP;
        gpio_handler.Pin = SPI5_SCK_PIN;
        gpio_handler.Pull = GPIO_PULLUP;
        gpio_handler.Speed = GPIO_SPEED_FAST;
        HAL_GPIO_Init(SPI5_SCK_PORT, &gpio_handler);

        gpio_handler.Pin = SPI5_MOSI_PIN;
        HAL_GPIO_Init(SPI5_MOSI_PORT, &gpio_handler);

        gpio_handler.Pin = SPI5_MISO_PIN;
        HAL_GPIO_Init(SPI5_MISO_PORT, &gpio_handler);
    }
}

// SPI速度设置函数  SPI速度 = Fapb1 / 分频系数
// Fapb1时钟一般为45MHz
void spi5_set_speed(uint8_t speed) {
    // 判断有效性
    assert_param(IS_SPI_BAUDRATE_PRESCALER(speed));
    // 关闭SPI
    __HAL_SPI_DISABLE(&spi5_handler);
    // 位3-5清零 用来设置波特率
    spi5_handler.Instance->CR1 &= 0xFFC7;
    // 设置SPI速度
    spi5_handler.Instance->CR1 |= speed;
    // 使能SPI
    __HAL_SPI_ENABLE(&spi5_handler);
}

// SPI5读写一个字节
// data: 要写入的字节 返回值: 读取到的字节
uint8_t spi5_read_write_byte(uint8_t data) {
    uint8_t res;
    HAL_SPI_TransmitReceive(&spi5_handler, &data, &res, 1, 1000);
    return res;
}
