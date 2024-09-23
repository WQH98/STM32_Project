#include "clock.h"
#include "led_cfg.h"
#include "uart_cfg.h"
#include "key_cfg.h"
#include "tim_interrupt_cfg.h"
#include "tim_pwm_cfg.h"
#include "tim_capture_cfg.h"


int main(void)
{
    long long temp = 0;
//    uint8_t dir=1;
//    uint16_t led0pwmval=0;
    HAL_Init();
	stm32_clock_init();
    // led_init();
    uart_init();
    // key_init();
    // tim_interrupt_init();
    // tim_pwm_init();
    tim_capture_init();
    printf("Hello World\r\n");
    while (1)
    {
        // delay_ms(10);
        // if(dir)led0pwmval++; //dir==1 led0pwmval 递增
        // else led0pwmval--; //dir==0 led0pwmval 递减
        // if(led0pwmval>300)dir=0; //led0pwmval 到达 300 后，方向为递减
        // if(led0pwmval==0)dir=1; //led0pwmval 递减到 0 后，方向改为递增
        // // TIM_SetTIM3Compare4(led0pwmval);//修改比较值，修改占空比
        // // TIM3->CCR4 = led0pwmval;
        // set_pwm_compompare(TIM3_CH4, led0pwmval);

//        if(get_capture_state(TIM5_CH1) & 0x80)
//        {
//            temp = get_capture_state(TIM5_CH1) & 0x3F;
//            temp *= 0xFFFFFFFF;
//            temp += get_capture_val(TIM5_CH1);
//            printf("high: %lld us\r\n", temp);
//            set_capture_state(TIM5_CH1, 0);
//        }

    }
}
