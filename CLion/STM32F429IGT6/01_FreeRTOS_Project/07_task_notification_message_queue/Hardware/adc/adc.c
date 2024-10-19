#include "adc.h"

ADC_HandleTypeDef adc1_handler;

void adc1_init() {
    adc1_handler.Instance = ADC1;
    // 4分频 adc_clk = pclk2 / 4 = 90 / 4 = 22.5MHz
    adc1_handler.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
    // 12位模式
    adc1_handler.Init.Resolution = ADC_RESOLUTION_12B;
    // 右对齐
    adc1_handler.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    // 非扫描模式
    adc1_handler.Init.ScanConvMode = DISABLE;
    // 关闭EOC中断
    adc1_handler.Init.EOCSelection = DISABLE;
    // 关闭连续转换
    adc1_handler.Init.ContinuousConvMode = DISABLE;
    // 1个转换在连续序列中
    adc1_handler.Init.NbrOfConversion = 1;
    // 禁止不连续采样模式
    adc1_handler.Init.DiscontinuousConvMode = DISABLE;
    // 不连续采样通道为0
    adc1_handler.Init.NbrOfDiscConversion = 0;
    // 软件触发
    adc1_handler.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    // 关闭DMA请求
    adc1_handler.Init.DMAContinuousRequests = DISABLE;
    HAL_ADC_Init(&adc1_handler);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc) {
    GPIO_InitTypeDef gpio_handler;
    if(hadc->Instance == ADC1) {
        __HAL_RCC_ADC1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        gpio_handler.Pin = ADC1_CHANNEL5_PIN;
        gpio_handler.Mode = GPIO_MODE_ANALOG;
        gpio_handler.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(ADC1_CHANNEL5_PORT, &gpio_handler);
    }
}

// 获得ADC值
// ch: 通道值0—16 取值范围ADC_CHANNEL_0~ADC_CHANNEL_16
// 返回值 转换结果
uint16_t get_adc1_value(uint32_t ch) {

    ADC_ChannelConfTypeDef adc1_channel_handler;

    // 通道
    adc1_channel_handler.Channel = ch;
    // 偏移
    adc1_channel_handler.Offset = 0;
    // 第一个序列 序列1
    adc1_channel_handler.Rank = 1;
    // 采样时间
    adc1_channel_handler.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    HAL_ADC_ConfigChannel(&adc1_handler, &adc1_channel_handler);

    // 开启ADC
    HAL_ADC_Start(&adc1_handler);

    // 轮询转换
    HAL_ADC_PollForConversion(&adc1_handler, 10);

    // 返回最近转换结果
    return (uint16_t)HAL_ADC_GetValue(&adc1_handler);
}

// 获取指定通道的转换值 取nums次 然后求平均值
// nums: 获取次数
// 返回值: 通道ch的nums次转换结果平均值
uint16_t get_adc1_average_value(uint32_t ch, uint8_t nums) {
    uint32_t temp_val = 0;
    uint8_t t = 0;
    for(t = 0; t < nums; t++) {
        temp_val += get_adc1_value(ch);
        HAL_Delay(4);
    }
    return temp_val / nums;
}

// 获取电压函数
// adc_value: adc值
// 返回值: 电压值
float get_voltage(uint16_t adc_value) {
    return (float)((float)adc_value * (3.3 / 4096));
}
