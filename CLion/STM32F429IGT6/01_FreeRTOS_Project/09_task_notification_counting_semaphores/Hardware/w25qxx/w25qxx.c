#include "w25qxx.h"

uint16_t W25QXX_TYPE = W25Q256;

void w25qxx_delay(uint16_t i) {
    while(i--);
}

// 4Kbytes为一个Sector 16个扇区为1个Block
// W25Q256 容量为32M字节,共有512个Block,8192个Sector
// 初始化SPI FLASH的IO口
void w25qxx_init(void) {
    uint8_t temp = 0;
    __HAL_RCC_GPIOF_CLK_ENABLE();
    GPIO_InitTypeDef gpio_handler;

    gpio_handler.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_handler.Pin = W25QXX_CS_PIN;
    gpio_handler.Pull = GPIO_PULLUP;
    gpio_handler.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(W25QXX_CS_PORT, &gpio_handler);

    // SPI flash不选中
    HAL_GPIO_WritePin(W25QXX_CS_PORT, W25QXX_CS_PIN, GPIO_PIN_SET);

    spi5_init();
    // 设置45M时钟 高速模式
    spi5_set_speed(SPI_BAUDRATEPRESCALER_2);
    W25QXX_TYPE = w25qxx_read_id();
    if(W25QXX_TYPE == W25Q256) {
        // 读取状态寄存器3 判断地址模式
        temp = w25qxx_read_sr(3);
        // 如果不是4字节地址模式 则进入4字节地址模式
        if((temp & 0x01) == 0) {
            // 选中FLASH
            HAL_GPIO_WritePin(W25QXX_CS_PORT, W25QXX_CS_PIN, GPIO_PIN_RESET);
            // 发送进入4字节地址模式指令
            spi5_read_write_byte(W25X_Enable4ByteAddr);
            // 不选中FLASH
            HAL_GPIO_WritePin(W25QXX_CS_PORT, W25QXX_CS_PIN, GPIO_PIN_SET);
        }
    }
}

// 读取芯片ID
// 返回值如下:
// 0XEF13,表示芯片型号为W25Q80
// 0XEF14,表示芯片型号为W25Q16
// 0XEF15,表示芯片型号为W25Q32
// 0XEF16,表示芯片型号为W25Q64
// 0XEF17,表示芯片型号为W25Q128
// 0XEF18,表示芯片型号为W25Q256
uint16_t w25qxx_read_id(void) {
    uint16_t temp = 0;
    // 选中FLASH
    HAL_GPIO_WritePin(W25QXX_CS_PORT, W25QXX_CS_PIN, GPIO_PIN_RESET);
    // 发送读取ID命令
    spi5_read_write_byte(0x90);
    spi5_read_write_byte(0x00);
    spi5_read_write_byte(0x00);
    spi5_read_write_byte(0x00);
    temp |= spi5_read_write_byte(0xFF) << 8;
    temp |= spi5_read_write_byte(0xFF);
    // 不选中FLASH
    HAL_GPIO_WritePin(W25QXX_CS_PORT, W25QXX_CS_PIN, GPIO_PIN_SET);
    return temp;
}

// 读取W25QXX的状态寄存器，W25QXX一共有3个状态寄存器
// 状态寄存器1：
// BIT7  6   5   4   3   2   1   0
// SPR   RV  TB BP2 BP1 BP0 WEL BUSY
// SPR:默认0,状态寄存器保护位,配合WP使用
// TB,BP2,BP1,BP0:FLASH区域写保护设置
// WEL:写使能锁定
// BUSY:忙标记位(1,忙;0,空闲)
// 默认:0x00
// 状态寄存器2：
// BIT7  6   5   4   3   2   1   0
// SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
// 状态寄存器3：
// BIT7      6    5    4   3   2   1   0
// HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
// regno:状态寄存器号 :1~3
// 返回值:状态寄存器值
uint8_t w25qxx_read_sr(uint8_t regno) {
    uint8_t byte = 0, command = 0;
    switch(regno) {
        case 1:
            // 读取状态寄存器1指令
            command = W25X_ReadStatusReg1;
        break;
        case 2:
            // 读取状态寄存器2指令
            command = W25X_ReadStatusReg2;
        break;
        case 3:
            // 读取状态寄存器3指令
            command = W25X_ReadStatusReg3;
        break;
        default:
            // 读取状态寄存器1指令
            command = W25X_ReadStatusReg1;
        break;
    }
    // 选中FLASH
    HAL_GPIO_WritePin(W25QXX_CS_PORT, W25QXX_CS_PIN, GPIO_PIN_RESET);
    // 发送读取状态寄存器命令
    spi5_read_write_byte(command);
    // 读取一个字节
    byte = spi5_read_write_byte(0xFF);
    // 不选中FLASH
    HAL_GPIO_WritePin(W25QXX_CS_PORT, W25QXX_CS_PIN, GPIO_PIN_SET);
    return byte;
}

// 写W25QXX状态寄存器
void w25qxx_write_sr(uint8_t regno, uint8_t sr) {
    uint8_t command = 0;
    switch(regno) {
        case 1:
            // 写状态寄存器1指令
            command = W25X_WriteStatusReg1;
        break;
        case 2:
            // 写状态寄存器2指令
            command = W25X_WriteStatusReg2;
        break;
        case 3:
            // 写状态寄存器3指令
            command = W25X_WriteStatusReg3;
        break;
        default:
            // 写状态寄存器1指令
            command = W25X_WriteStatusReg1;
        break;
    }
    // 选中FLASH
    HAL_GPIO_WritePin(W25QXX_CS_PORT, W25QXX_CS_PIN, GPIO_PIN_RESET);
    // 发送写取状态寄存器命令
    spi5_read_write_byte(command);
    // 写入一个字节
    spi5_read_write_byte(sr);
    // 不选中FLASH
    HAL_GPIO_WritePin(W25QXX_CS_PORT, W25QXX_CS_PIN, GPIO_PIN_SET);
}


// W25QXX写使能 将WEL置位
void w25qxx_write_enable(void) {
    // 选中FLASH
    HAL_GPIO_WritePin(W25QXX_CS_PORT, W25QXX_CS_PIN, GPIO_PIN_RESET);
    // 发送写使能
    spi5_read_write_byte(W25X_WriteEnable);
    // 不选中FLASH
    HAL_GPIO_WritePin(W25QXX_CS_PORT, W25QXX_CS_PIN, GPIO_PIN_SET);
}

// W25QXX写禁止 将WEL清零
void w25qxx_write_disable(void) {
    // 选中FLASH
    HAL_GPIO_WritePin(W25QXX_CS_PORT, W25QXX_CS_PIN, GPIO_PIN_RESET);
    // 发送写使能
    spi5_read_write_byte(W25X_WriteDisable);
    // 不选中FLASH
    HAL_GPIO_WritePin(W25QXX_CS_PORT, W25QXX_CS_PIN, GPIO_PIN_SET);
}

// SPI在一页(0~65535)内写入少于256个字节的数据
// 在指定地址开始写入最大256字节的数据
// buff:数据存储区
// addr:开始写入的地址(24bit)
// num:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
void w25qxx_write_page(uint8_t *buff, uint32_t addr, uint16_t num) {
    uint16_t i;
    // SET WEL
    w25qxx_write_enable();
    // 选中FLASH
    HAL_GPIO_WritePin(W25QXX_CS_PORT, W25QXX_CS_PIN, GPIO_PIN_RESET);
    // 发送写页命令
    spi5_read_write_byte(W25X_PageProgram);
    // 如果是W25Q256的话 要发送最高8位
    if(W25QXX_TYPE == W25Q256) {
        spi5_read_write_byte((uint8_t)(addr >> 24));
    }
    spi5_read_write_byte((uint8_t)(addr >> 16));
    spi5_read_write_byte((uint8_t)(addr >> 8));
    spi5_read_write_byte((uint8_t)addr);
    for(i = 0; i < num; i++) {
        spi5_read_write_byte(buff[i]);
    }
    // 不选中FLASH
    HAL_GPIO_WritePin(W25QXX_CS_PORT, W25QXX_CS_PIN, GPIO_PIN_SET);
    w25qxx_wait_busy();
}

// 无检验写SPI FLASH
// 必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
// 具有自动换页功能
// 在指定地址开始写入指定长度的数据,但是要确保地址不越界!
// buff:数据存储区
// addr:开始写入的地址(24bit)
// num:要写入的字节数(最大65535)
void w25qxx_write_no_check(uint8_t *buff, uint32_t addr, uint16_t num) {
    // 计算单页剩余的字节数
    uint16_t page_remain = 256 - addr % 256;
    // 不大于256个字节
    if(num <= page_remain) {
        page_remain = num;
    }
    while(1) {
        w25qxx_write_page(buff, addr, page_remain);
        // 写入结束了
        if(num == page_remain) {
            break;
        }
        // num > page_remain
        else {
            buff += page_remain;
            addr += page_remain;
            // 减去可以写入了的字节数
            num -= page_remain;
            if(num > 256) {
                // 一次可以写入256个字节
                page_remain = 256;
            }
            else {
                // 不够256个字节了
                page_remain = num;
            }
        }
    }
}

// 读取SPI FLASH
// 在指定地址开始读取指定长度的数据
// buff:数据存储区
// addr:开始读取的地址(24bit)
// num:要读取的字节数(最大65535)
void w25qxx_read(uint8_t *buff, uint32_t addr, uint16_t num) {
    uint16_t i = 0;
    // 选中FLASH
    HAL_GPIO_WritePin(W25QXX_CS_PORT, W25QXX_CS_PIN, GPIO_PIN_RESET);
    // 发送读取命令
    spi5_read_write_byte(W25X_ReadData);
    // 发送24bit地址
    if(W25QXX_TYPE == W25Q256) {
        spi5_read_write_byte((uint8_t)(addr >> 24));
    }
    spi5_read_write_byte((uint8_t)(addr >> 16));
    spi5_read_write_byte((uint8_t)(addr >> 8));
    spi5_read_write_byte((uint8_t)addr);
    //循环读数
    for(i = 0; i < num; i++) {
        buff[i] = spi5_read_write_byte(0xFF);
    }
    // 不选中FLASH
    HAL_GPIO_WritePin(W25QXX_CS_PORT, W25QXX_CS_PIN, GPIO_PIN_SET);
}

// 写SPI FLASH
// 在指定地址开始写入指定长度的数据
// 该函数带擦除操作!
// buff:数据存储区
// addr:开始写入的地址(24bit)
// num:要写入的字节数(最大65535)
uint8_t W25QXX_BUFFER[4096];
void w25qxx_write(uint8_t *buff, uint32_t addr, uint16_t num) {
    uint32_t sec_pos;
    uint16_t sec_off;
    uint16_t sec_re_main;
    uint16_t i;
    uint8_t *w25qxx_buf = W25QXX_BUFFER;
    // 扇区地址
    sec_pos = addr / 4096;
    // 在扇区内的偏移
    sec_off = addr % 4096;
    // 扇区剩余空间大小
    sec_re_main = 4096 - sec_off;
    // 不大于4096个字节
    if(num <= sec_re_main) {
        sec_re_main = num;
    }
    while(1) {
        // 读出整个扇区的内容
        w25qxx_read(w25qxx_buf, sec_pos * 4096, 4096);
        // 校验数据
        for(i = 0; i < sec_re_main; i++) {
            // 需要擦除
            if(w25qxx_buf[sec_off+i] != 0xFF) {
                break;
            }
        }
        // 需要擦除
        if(i < sec_re_main) {
            // 擦除这个扇区
            w25qxx_erase_sector(sec_pos);
            // 复制
            for(i = 0; i < sec_re_main; i++) {
                w25qxx_buf[i+sec_off] = buff[i];
            }
            // 写入整个扇区
            w25qxx_write_no_check(w25qxx_buf, sec_pos * 4096, 4096);
        }
        else {
            // 写已经擦除了的 直接写入扇区剩余区间
            w25qxx_write_no_check(w25qxx_buf, addr, sec_re_main);
        }
        // 写入结束了
        if(num == sec_re_main) {
            break;
        }
        // 写入未结束
        else {
            // 扇区地址增1
            sec_pos++;
            // 偏移地址为0
            sec_off = 0;
            // 指针偏移
            buff += sec_re_main;
            // 写地址偏移
            addr += sec_re_main;
            // 字节数递减
            num -= sec_re_main;
            // 下一个区间还是写不完
            if(num > 4096) {
                sec_re_main = 4096;
            }
            // 下一个区间可以写完了
            else {
                sec_re_main = num;
            }
        }
    }
}

// 擦除整个芯片
// 等待时间超长...
void w25qxx_erase_chip(void) {
    // SET WEL
    w25qxx_write_enable();
    w25qxx_wait_busy();
    // 选中FLASH
    HAL_GPIO_WritePin(W25QXX_CS_PORT, W25QXX_CS_PIN, GPIO_PIN_RESET);
    // 发送片擦除命令
    spi5_read_write_byte(W25X_ChipErase);
    // 不选中FLASH
    HAL_GPIO_WritePin(W25QXX_CS_PORT, W25QXX_CS_PIN, GPIO_PIN_SET);
    w25qxx_wait_busy();
}

// 擦除一个扇区
// addr:扇区地址 根据实际容量设置
// 擦除一个扇区的最少时间:150ms
void w25qxx_erase_sector(uint32_t addr) {
    addr *= 4096;
    // SET WEL
    w25qxx_write_enable();
    w25qxx_wait_busy();
    // 选中FLASH
    HAL_GPIO_WritePin(W25QXX_CS_PORT, W25QXX_CS_PIN, GPIO_PIN_RESET);
    // 发送扇区擦除指令
    spi5_read_write_byte(W25X_SectorErase);
    // 如果是W25Q256的话 地址为4字节的 要发送最高8位
    if(W25QXX_TYPE == W25Q256) {
        spi5_read_write_byte((uint8_t)(addr >> 24));
    }
    spi5_read_write_byte((uint8_t)(addr >> 16));
    spi5_read_write_byte((uint8_t)(addr >> 8));
    spi5_read_write_byte((uint8_t)addr);
    // 不选中FLASH
    HAL_GPIO_WritePin(W25QXX_CS_PORT, W25QXX_CS_PIN, GPIO_PIN_SET);
    // 等待擦写完成
    w25qxx_wait_busy();
}

//等待空闲
void w25qxx_wait_busy(void) {
    while((w25qxx_read_sr(1) & 0x01) == 0x01);   // 等待BUSY位清空
}

//进入掉电模式
void w25qxx_power_down(void) {
    // 选中FLASH
    HAL_GPIO_WritePin(W25QXX_CS_PORT, W25QXX_CS_PIN, GPIO_PIN_RESET);
    // 发送掉电命令
    spi5_read_write_byte(W25X_PowerDown);
    // 不选中FLASH
    HAL_GPIO_WritePin(W25QXX_CS_PORT, W25QXX_CS_PIN, GPIO_PIN_SET);
    w25qxx_delay(10);
}

//唤醒
void w25qxx_wakeup(void) {
    // 选中FLASH
    HAL_GPIO_WritePin(W25QXX_CS_PORT, W25QXX_CS_PIN, GPIO_PIN_RESET);
    // 发送唤醒命令
    spi5_read_write_byte(W25X_ReleasePowerDown);
    // 不选中FLASH
    HAL_GPIO_WritePin(W25QXX_CS_PORT, W25QXX_CS_PIN, GPIO_PIN_SET);
    w25qxx_delay(10);
}
