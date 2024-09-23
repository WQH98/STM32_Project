#include "flash.h"

void flash_delay(uint32_t i) {
	while(i--);
}

// 获取某个扇区所在的flash地址
// addr flash地址
// 返回值 0~11 addr所在的扇区
uint8_t flash_get_sector(uint32_t addr) {
	if(addr<ADDR_FLASH_SECTOR_1)return FLASH_SECTOR_0;
	else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_SECTOR_1;
	else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_SECTOR_2;
	else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_SECTOR_3;
	else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_SECTOR_4;
	else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_SECTOR_5;
	else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_SECTOR_6;
	else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_SECTOR_7;
	else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_SECTOR_8;
	else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_SECTOR_9;
	else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_SECTOR_10; 
	else if(addr<ADDR_FLASH_SECTOR_12)return FLASH_SECTOR_11; 
	else if(addr<ADDR_FLASH_SECTOR_13)return FLASH_SECTOR_12; 
	else if(addr<ADDR_FLASH_SECTOR_14)return FLASH_SECTOR_13; 
	else if(addr<ADDR_FLASH_SECTOR_15)return FLASH_SECTOR_14; 
	else if(addr<ADDR_FLASH_SECTOR_16)return FLASH_SECTOR_15; 
	else if(addr<ADDR_FLASH_SECTOR_17)return FLASH_SECTOR_16; 
	else if(addr<ADDR_FLASH_SECTOR_18)return FLASH_SECTOR_17; 
	else if(addr<ADDR_FLASH_SECTOR_19)return FLASH_SECTOR_18; 
	else if(addr<ADDR_FLASH_SECTOR_20)return FLASH_SECTOR_19; 
	else if(addr<ADDR_FLASH_SECTOR_21)return FLASH_SECTOR_20; 
	else if(addr<ADDR_FLASH_SECTOR_22)return FLASH_SECTOR_21; 
	else if(addr<ADDR_FLASH_SECTOR_23)return FLASH_SECTOR_22;  
	return FLASH_SECTOR_23;	
}


// 读取指定地址的值
// addr 读取的地址
// 返回值 对应数据
uint32_t flash_read_word(uint32_t addr) {
	return *(__IO uint32_t*)addr;
}


// 从指定的地址开始写入指定长度的数据
// 注意：因为STM32F4的扇区太大 没办法本地保存扇区数据 所以本函数写地址如果非0xFF
// 那么会先擦除整个扇区且不保存扇区数据 所以些非0xFF的地址 将会导致整个扇区数据丢失
// 建议写之前确保扇区里没有重要数据 最好是整个扇区先擦除了 再慢慢往里写
// 该函数可以往OPT区域内写值
// OTP 区域地址范围:0X1FFF7800~0X1FFF7A0F(最后 16 字节用于 OTP 数据块锁定)
// addr 写入的起始地址 此地址必须是4的倍数（因为要写入的数据是32位的 占用4个地址）
// buf  数据指针
// num  要写的字数（就是要写入32位数据的数量）
int flash_write(uint32_t addr, uint32_t* buf, uint32_t num) {
	
    FLASH_EraseInitTypeDef flash_handler;
    HAL_StatusTypeDef flash_status = HAL_OK;
    uint32_t sector_error = 0;
	uint32_t start_addr = 0, end_addr = 0;
	
	// 写入的地址比flash的起始地址小或者比结束地址大 是一个非法地址
	if(addr < STM32_FLASH_BASE) {
		return 1;
	}
	// 写入的地址不是4的倍数 是一个非法地址
	if(addr % 4 != 0) {
		return 2;
	}
	

	
    // 写入的起始地址和终止地址
    start_addr = addr;
    end_addr = addr + num * 4;

		// 解锁FLASH
	HAL_FLASH_Unlock();
	
    // 先擦除
	if(start_addr < 0X1FFF0000) {
		while(start_addr < end_addr) {
			// 有非0xFFFFFFFF的扇区 先把整个扇区擦了
			if(flash_read_word(start_addr) != 0xFFFFFFFF) {

				// printf("test\r\n");
				// flash_handler.Banks = FLASH_BANK_1;
				flash_handler.TypeErase = FLASH_TYPEERASE_SECTORS;    // 擦除类型选择扇区擦除
				flash_handler.Sector = flash_get_sector(start_addr);   // 擦除当前地址所在扇区
				flash_handler.NbSectors = 1;     // 一次只删除一个扇区
				flash_handler.VoltageRange = FLASH_VOLTAGE_RANGE_3;   // 电压范围在2.7~3.6V之间
				if(HAL_FLASHEx_Erase(&flash_handler, &sector_error) != HAL_OK) {
					break;
				}
			}
			else {
				// 地址加4
				start_addr += 4;
			}
			// 等待操作完成 50S内完成就可以
			FLASH_WaitForLastOperation(50000);
		}
	}
    // 一直等待 直到出结果
	flash_status = FLASH_WaitForLastOperation(HAL_MAX_DELAY);
    // 重新赋值开始地址
    start_addr = addr;
    if(HAL_OK == flash_status) {
        // 写数据
        while(start_addr < end_addr) {
            if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, start_addr, *buf) != HAL_OK) {
				break;
			}
            start_addr += 4;
            buf++;
        }
    }

	// 上锁FLASH
	HAL_FLASH_Lock();
	return 0;
}

// 从指定地址读出指定长度的数据
// addr 起始地址  buf 数据指针  num 要读出数据的个数
void flash_read(uint32_t addr, uint32_t* buf, uint32_t num) {
    uint32_t i = 0;
    for(i = 0; i < num; i++) {
        buf[i] = flash_read_word(addr);
        addr += 4;
    }
}


