#include "dma.h"
#include "uart.h"

DMA_HandleTypeDef dma2_stream0_handle;
DMA_HandleTypeDef dma2_stream7_handle;

/* 定义aSRC_Const_Buffer数组作为DMA传输数据源
const关键字将aSRC_Const_Buffer数组变量定义为常量类型 */
const uint32_t aSRC_Const_Buffer[BUFFER_SIZE] = {
    0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
    0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
    0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
    0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
    0x41424344, 0x44564748, 0x494A4B4C, 0x4D4E4F50,
    0x51525345, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
    0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,
    0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80,
};
/* 定义DMA传输目标存储器 */
uint32_t aDST_Buffer[BUFFER_SIZE];

// DMA2 Stream0 初始化
void dma2_stream0_init(void) {

    __HAL_RCC_DMA2_CLK_ENABLE();

    dma2_stream0_handle.Instance = DMA2_Stream0;
    // DMA数据流通道选择
    dma2_stream0_handle.Init.Channel = DMA_CHANNEL_0;
    // 存储器到存储器模式
    dma2_stream0_handle.Init.Direction = DMA_MEMORY_TO_MEMORY;
    // 使能自动递增功能
    dma2_stream0_handle.Init.PeriphInc = DMA_PINC_ENABLE;
    dma2_stream0_handle.Init.MemInc = DMA_MINC_ENABLE;
    // 源数据是字大小(32位)
    dma2_stream0_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    // 目标数据也是字大小
    dma2_stream0_handle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    // 一次传输模式 存储器到存储器之间不能使用循环传输
    dma2_stream0_handle.Init.Mode = DMA_NORMAL;
    // DMA数据流优先级为高
    dma2_stream0_handle.Init.Priority = DMA_PRIORITY_HIGH;
    // 禁用FIFO模式
    dma2_stream0_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    dma2_stream0_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    // 单次模式 突发传输 1个节拍
    dma2_stream0_handle.Init.MemBurst = DMA_MBURST_SINGLE;
    // 单次模式 突发传输 1个节拍
    dma2_stream0_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;
    HAL_DMA_Init(&dma2_stream0_handle);

    HAL_DMA_Start(&dma2_stream0_handle, (uint32_t)aSRC_Const_Buffer, (uint32_t)aDST_Buffer, BUFFER_SIZE);
}

// 源数据与目标地址数据对比
uint8_t buffer_cmp(const uint32_t *p_src_buffer, uint32_t *p_dst_buffer, uint16_t size) {
    // 数据长度递减
    while(size--) {
        // 判断两个数据源是否对应相等
        if(*p_src_buffer != *p_dst_buffer) {
            // 对应数据源不相等马上退出函数 返回-1
            return -1;
        }
        // 递增两个数据源的地址指针
        p_src_buffer++;
        p_dst_buffer++;
    }
    // 完成判断 没有问题 返回0
    return 0;
}

void dma2_stream7_init(void) {
    __HAL_RCC_DMA2_CLK_ENABLE();

    dma2_stream7_handle.Instance = DMA2_Stream7;
    // uart1_tx 对应dma2 通道4 数据流7
    dma2_stream7_handle.Init.Channel = DMA_CHANNEL_4;
    // 方向：从内存到外设
    dma2_stream7_handle.Init.Direction = DMA_MEMORY_TO_PERIPH;
    // 外设地址不增
    dma2_stream7_handle.Init.PeriphInc = DMA_PINC_DISABLE;
    // 内存地址自增
    dma2_stream7_handle.Init.MemInc = DMA_MINC_ENABLE;
    // 外设数据单位 8bit 1byte
    dma2_stream7_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    // 内存数据单位 8bit 1byte
    dma2_stream7_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    // DMA模式 单次模式
    dma2_stream7_handle.Init.Mode = DMA_NORMAL;
    // 优先级 中
    dma2_stream7_handle.Init.Priority = DMA_PRIORITY_MEDIUM;
    // 禁用FIFO
    dma2_stream7_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    dma2_stream7_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    // 存储器突发传输 1个节拍
    dma2_stream7_handle.Init.MemBurst = DMA_MBURST_SINGLE;
    // 外设突发传输 1个节拍
    dma2_stream7_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;
    HAL_DMA_DeInit(&dma2_stream7_handle);
    HAL_DMA_Init(&dma2_stream7_handle);

    __HAL_LINKDMA(&uart1_handler, hdmatx, dma2_stream7_handle);

    HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
}

void DMA2_Stream7_IRQHandler(void) {
    HAL_DMA_IRQHandler(&dma2_stream7_handle);
}
