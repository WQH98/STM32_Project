#include "at24cxx.h"

#include "uart.h"

// 在at24cxx指定地址读出一个数据
// addr：开始读数的地址
// 返回值：读到的数据
uint8_t at24cxx_read_byte(uint16_t addr) {
    uint8_t res = 0;
    iic_start();
    if(EE_TYPE > AT24C16) {
        iic_send_byte(0xA0);
        iic_wait_ack();
        iic_send_byte(addr >> 8);
    }
    else {
        iic_send_byte(0xA0 + ((addr / 256) << 1));
    }
    iic_wait_ack();
    iic_send_byte(addr % 256);
    iic_wait_ack();
    iic_start();
    iic_send_byte(0xA1);
    iic_wait_ack();
    res = iic_read_byte(0);
    iic_stop();
    return res;
}

// 在at24cxx指定地址写入一个数据
// addr: 要写入的地址  data: 要写入的数据
void at24cxx_write_byte(uint16_t addr, uint8_t data) {
    iic_start();
    if(EE_TYPE > AT24C16) {
        iic_send_byte(0xA0);
        iic_wait_ack();
        iic_send_byte(addr >> 8);
    }
    else {
        iic_send_byte(0xA0 + ((addr / 256) << 1));
    }
    iic_wait_ack();
    iic_send_byte(addr % 256);
    iic_wait_ack();
    iic_send_byte(data);
    iic_wait_ack();
    iic_stop();
    HAL_Delay(10);
}

// 检查at24cxx是否正常 使用at24cxx的最后一个地址(255)来存储标志字
// 如果使用了其他的24c系列 这个地址要改
// 返回值 1，检测失败 0，检测成功
uint8_t at24cxx_check(void) {
    uint8_t temp;
    // 避免每次开机都写at24cxx
    temp = at24cxx_read_byte(255);
    if(temp == 0x55) {
       return 0;
    }
    else {
        // 排除第一次初始化的情况
        at24cxx_write_byte(255, 0x55);
        temp = at24cxx_read_byte(255);
        if(temp == 0x55) {
            return 0;
        }
    }
    return 1;
}

// 在at24cxx的指定地址开始写入长度为len的数据
// 该函数用于用于16bit或者32bit的数据
// addr: 开始写入的地址
// data: 数据数组首地址
// len:  要写入数据的长度2, 4
void at24cxx_write_len_byte(uint16_t addr, uint32_t data, uint8_t len) {
    uint8_t t;
    for(t = 0; t < len; t++) {
        at24cxx_write_byte(addr + t, (data >> (8 * t)) & 0xFF);
    }
}

// 在at24cxx的指定地址开始读出长度为len的数据
// 该函数用于读出16bit或者32bit的数据
// addr: 开始读出的数据
// 返回值: 数据
// len: 要读出数据的长度2, 4
uint32_t at24cxx_read_len_byte(uint16_t addr, uint8_t len) {
    uint8_t t;
    uint32_t temp = 0;
    for(t = 0; t < len; t++) {
        temp <<= 8;
        temp += at24cxx_read_byte(addr+len-t-1);
    }
    return temp;
}

// 在at24cxx里面的指定地址开始读出指定个数的数据
// addr: 开始读出的地址 对24c02为0~255
// buff: 数据数组首地址
// num:  要读出数据的个数
void at24cxx_read(uint16_t addr, uint8_t *buff, uint16_t num) {
    while(num) {
        *buff++ = at24cxx_read_byte(addr++);
        num--;
    }
}

// 在at24cxx里面的指定地址开始写入指定个数的数据
// buff: 开始写入的地址 对24c02为0~255
// buff: 数据数组首地址
// num: 要写入数据的个数
void at24cxx_write(uint16_t addr, uint8_t *buff, uint16_t num) {
    while(num--) {
        at24cxx_write_byte(addr, *buff);
        addr++;
        buff++;
    }
}
