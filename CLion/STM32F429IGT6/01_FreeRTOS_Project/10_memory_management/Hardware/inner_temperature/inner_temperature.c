#include "inner_temperature.h"

// 使用此函数之前 需要初始化ADC1
// 获取内部温度值
// 返回值: 内部温度值
float get_inner_temperature(void) {
    float temp = 0;
    temp = get_voltage(get_adc1_average_value(ADC_CHANNEL_TEMPSENSOR, 10));
    temp = ((temp - 0.76) / 0.0025 + 25);
    return temp;
}
