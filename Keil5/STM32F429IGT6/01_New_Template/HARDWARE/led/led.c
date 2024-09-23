#include "led_cfg.h"

// LED设备指针
static led_cfg_type *g_led_info = NULL;

// LED开
int led_on(int id) 
{
    // 输入参数检查
    if(id != g_led_cfg[id].id) {
        return -1;
    }
    // 设置LED电平
    HAL_GPIO_WritePin(g_led_info[id].led_pin->port, \
                      g_led_info[id].led_pin->gpio_cfg->Pin,\
                      (GPIO_PinState)g_led_info[id].polar);
    return 0;
}

// LED关
int led_off(int id)
{
    // 输入参数检查
    if(id != g_led_cfg[id].id) {
        return -1;
    }
    // 设置LED电平
    HAL_GPIO_WritePin(g_led_info[id].led_pin->port, \
                      g_led_info[id].led_pin->gpio_cfg->Pin,\
                      (GPIO_PinState)!g_led_info[id].polar);
    return 0;
}

// LED翻转
int led_toggle(int id) 
{
    // 输入参数检查
    if(id != g_led_cfg[id].id) {
        return -1;
    }
    // 设置LED电平
    HAL_GPIO_TogglePin(g_led_info[id].led_pin->port, \
                      g_led_info[id].led_pin->gpio_cfg->Pin);
    return 0;
}

// LED设备注册
int led_register(led_cfg_type *p_cfg, int num) 
{
	int i = 0;
    // 拷贝配置参数
    g_led_info = p_cfg;
    // 判断初始化时 回调是否有指向
    if(g_led_info->rcc_enable == NULL) 
    {
        return -1;
    }
    
    for(i = 0; i < num; i++)
    {
        // 使能GPIO时钟
        g_led_info[i].rcc_enable();
        
        // LED初始化
        HAL_GPIO_Init(g_led_info[i].led_pin->port, \
                      g_led_info[i].led_pin->gpio_cfg);
        // LED默认为灭的状态
        HAL_GPIO_WritePin(g_led_info[i].led_pin->port, \
                          g_led_info[i].led_pin->gpio_cfg->Pin, \
                          (GPIO_PinState)!g_led_info[i].polar);
    }
    return 0;
}


