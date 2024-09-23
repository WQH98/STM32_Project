#ifndef __UART_H
#define __UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "stdio.h"

// GPIO_Init 参数 结构体
typedef struct uart_gpioinit_cfg {
    GPIO_TypeDef *port;             // LED 端口
    GPIO_InitTypeDef *gpio_cfg;     // HAL库 GPIO初始化参数
} uart_gpioinit_cfg_type;

typedef struct uart_rx_data {
    uint8_t rx_buf[256];
    uint8_t dma_rx_buf[256];
    uint16_t rx_len;
    uint8_t rx_finsh;
} uart_rx_data_type;

// 串口参数结构体
typedef struct uart_cfg {
    UART_HandleTypeDef *uart_handle;        // 串口句柄
    DMA_HandleTypeDef *dma_tx_handle;          // dma发送句柄
    DMA_HandleTypeDef *dma_rx_handle;          // dma接收句柄
    uart_gpioinit_cfg_type *uart_tx_pin;    // 串口发送使用引脚
    uart_gpioinit_cfg_type *uart_rx_pin;    // 串口接收使用引脚
    void (*rcc_enable)(void);               // 时钟使能
    IRQn_Type IRQn;                         // 中断
    uint32_t PreemptPriority;               // 抢占优先级
    uint32_t SubPriority;                   // 子优先级
    int id;                                 // 串口ID  
    uint8_t rx_enable;                      // 接收中断使能
    uint8_t idle_enable;                    // 空闲中断使能
    uint8_t dma_tx_enable;                  // dma发送使能
    uint8_t dma_rx_enable;                  // dma接收使能    
    void (*dma_rcc_enable)(void);           // dma时钟使能
    uint8_t rx_buf[256];                    // 接收buf
    uint8_t rx_len;                         // 接收数据长度
    uint8_t rx_finish;                      // 接收完成标志位
} uart_cfg_type;


// 串口号枚举
typedef enum uart_com {
    COM1 = 0,
    COM3 = 1
} uart_com_type;


// 接收数据结构体
extern uart_rx_data_type uart1_message;
extern uart_rx_data_type uart3_message;

// 串口发送函数
// id：使用哪个串口发送
// pData：要发送的数据
void uart_send(int id, char *pData);
// 串口设备注册
// p_cfg：串口参数结构体指针
// num：串口设备数量
int uart_register(uart_cfg_type *p_cfg, int num);
// 清除串口接收数据以及完成标志位
// data：接收数据结构体指针
int uart_clear(uart_rx_data_type *data);
// 串口DMA发送函数
// id：使用哪个串口发送
// pData：要发送的数据
int uart_dma_send(int id, uint8_t *pData);
// 串口DMA接收函数
// id：接收哪个串口的数据
// len：把数据的长度传出来
int uart_dma_receive(int id, uint8_t *len);
// 得到是否完成一帧数据的接收
// id：串口号
uint8_t uart_dma_get_rec_state(int id);
// 清除串口接收完成标志位
// id：串口号
void uart_dma_clear_rec_state(int id);
// 清除接收的长度
// id：串口号
void uart_dma_clear_rx_len(int id);
// 清除接收缓冲
// id：串口号
void uart_dma_clear_rx_buf(int id);
// 获取接收缓冲区的内容
// id：串口号
// pData：要存放的缓冲区的内容
void uart_dma_get_rx_buf(int id, uint8_t *pData);

#ifdef __cplusplus
}
#endif
#endif
