#include "iap.h"

iapfun jump2app;
uint32_t app_buf[512] = {0};

// 从指定地址 写入bin文件
// addr 应用程序起始地址
// buf  应用程序code
// size 应用程序大小
void write_iap_app(uint32_t addr, uint8_t* buf, uint32_t size) {
	uint32_t temp = 0;
	uint32_t t = 0;
	uint32_t start_addr = addr;
	uint8_t *add_bin = buf;
	uint16_t buf_count = 0;
	for(t = 0; t < size; t+=4) {
		temp = (uint32_t)add_bin[3] << 24;
		temp |= (uint32_t)add_bin[2] << 16;
		temp |= (uint32_t)add_bin[1] << 8;
		temp |= (uint32_t)add_bin[0];
        add_bin += 4;
        app_buf[buf_count++] = temp;
        if(buf_count == 512) {
            flash_write(start_addr, app_buf, buf_count);
            buf_count = 0;
            start_addr += 2048;
        }
	}
    if(buf_count != 0) {
        flash_write(start_addr, app_buf, buf_count);
        buf_count = 0;
    }
}



// 跳转到应用程序  addr 应用程序起始地址
void jump_app(uint32_t addr) {
    //检查栈顶地址是否合法.
    if(((*(__IO uint32_t*)addr)&0x2FF00000)==0x20000000)	{
		jump2app=(iapfun)*(__IO uint32_t*)(addr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP(*(__IO uint32_t*)addr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		__disable_irq();
		jump2app();									//跳转到APP.
    }
}


//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(uint32_t addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}
