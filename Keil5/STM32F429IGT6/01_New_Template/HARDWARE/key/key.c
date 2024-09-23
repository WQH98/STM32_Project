#include "key_cfg.h"

static key_cfg_type *g_key_info = NULL;

int key_get_state(int id) 
{
    // 输入参数检查
    if(id != g_key_info[id].id) 
    {
        return -1;
    }
    // 读 KEY 电平
    return HAL_GPIO_ReadPin(g_key_info[id].key_pin->port, g_key_info[id].key_pin->gpio_cfg->Pin);
}

int key_register(key_cfg_type *p_cfg, int num)
{
    int i = 0;
    // 拷贝配置参数
    g_key_info = p_cfg;
    for(i = 0; i < num; i++) 
    {
        // 判断初始化时 时钟回调函数是否有指向
        if(NULL == g_key_info[i].rcc_enable) 
        {
            continue;
        }
        // 使能GPIO时钟
        g_key_info[i].rcc_enable();
        // KEY 初始化
        HAL_GPIO_Init(g_key_info[i].key_pin->port, g_key_info[i].key_pin->gpio_cfg);
    }
    
    return 0;
}
