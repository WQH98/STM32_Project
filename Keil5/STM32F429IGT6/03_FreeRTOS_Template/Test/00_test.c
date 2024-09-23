
#define    LED_TEST       0
#define    EXTI_TEST      0
#define    IWDG_TEST      0
#define    WWDG_TEST      0
#define    DMA_TEST       1

void test_demo(void) {
// 外部中断测试
#if EXTI_TEST
    extern void exti_test(void);
    exti_test();
#endif
// LED测试
#if LED_TEST
    extern void led_test(void);
    led_test();
#endif
// 独立看门狗测试
#if IWDG_TEST
    extern void iwdg_test(void);
    iwdg_test();
#endif
// 窗口看门狗测试
#if WWDG_TEST
    extern void wwdg_test(void);
    wwdg_test();
#endif
// DMA测试
#if DMA_TEST
    extern void dma_test(void);
    dma_test();
#endif


}


