#include "w25q64.h"

uint16_t W25QXX_TYPE = 0;

void w25q64_init() {
    uint8_t temp;
    spi1_cs_high();     // 默认不选中
    spi1_init();        // 初始化SPI
	W25QXX_TYPE = w25qxx_read_id();
    if(W25Q64 == W25QXX_TYPE) {     // 读ID
        temp = w25qxx_read_sr(3);        // 读取状态寄存器3 判断地址模式
        if((temp & 0x01) == 0) {         // 如果不是4字节地址模式 则进入4字节地址模式
            spi1_cs_low();               // 选中
            spi1_read_write_byte(W25X_Enable4ByteAddr);    // 发送进入4字节地址模式指令
            spi1_cs_high();              // 取消片选
        }
    }
}

//读取芯片ID
//返回值如下:				   
//0XEF13,表示芯片型号为W25Q80  
//0XEF14,表示芯片型号为W25Q16    
//0XEF15,表示芯片型号为W25Q32  
//0XEF16,表示芯片型号为W25Q64 
//0XEF17,表示芯片型号为W25Q128 	  
//0XEF18,表示芯片型号为W25Q256
uint16_t w25qxx_read_id()
{
	uint16_t temp = 0;
	spi1_cs_low();
	spi1_read_write_byte(0x90);         //发送读取ID命令
	spi1_read_write_byte(0x00);
	spi1_read_write_byte(0x00);
	spi1_read_write_byte(0x00);
	temp |= spi1_read_write_byte(0xFF) << 8;
	temp |= spi1_read_write_byte(0xFF);
	spi1_cs_high();
	return temp;
}



//读取W25QXX的状态寄存器，W25QXX一共有3个状态寄存器
//状态寄存器1：
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
//状态寄存器2：
//BIT7  6   5   4   3   2   1   0
//SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
//状态寄存器3：
//BIT7      6    5    4   3   2   1   0
//HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
//regno:状态寄存器号，范:1~3
//返回值:状态寄存器值
uint8_t w25qxx_read_sr(uint8_t regno) {  
	uint8_t byte=0, command=0;
    switch(regno) {
        case 1:
            command = W25X_ReadStatusReg1;    //读状态寄存器1指令
            break;
        case 2:
            command = W25X_ReadStatusReg2;    //读状态寄存器2指令
            break;
        case 3:
            command = W25X_ReadStatusReg3;    //读状态寄存器3指令
            break;
        default:
            command = W25X_ReadStatusReg1;
            break;
    }
	spi1_cs_low();                            //使能器件
	spi1_read_write_byte(command);            //发送读取状态寄存器命令
	byte = spi1_read_write_byte(0XFF);        //读取一个字节
	spi1_cs_high();                           //取消片选
	return byte;
} 

//写W25QXX状态寄存器
void w25qxx_write_sr(uint8_t regno, uint8_t sr) {
    uint8_t command = 0;
    switch(regno) {
        case 1:
            command = W25X_WriteStatusReg1;    //写状态寄存器1指令
            break;
        case 2:
            command = W25X_WriteStatusReg2;    //写状态寄存器2指令
            break;
        case 3:
            command = W25X_WriteStatusReg3;    //写状态寄存器3指令
            break;
        default:
            command = W25X_WriteStatusReg1;
            break;
    }
	spi1_cs_low();                            //使能器件
	spi1_read_write_byte(command);            //发送写取状态寄存器命令
	spi1_read_write_byte(sr);                 //写入一个字节
	spi1_cs_high();                           //取消片选
}

//W25QXX写使能	
//将WEL置位   
void w25qxx_write_enable(void) {
	spi1_cs_low();                             //使能器件   
    spi1_read_write_byte(W25X_WriteEnable);    //发送写使能  
	spi1_cs_high();                            //取消片选     	      
} 

//W25QXX写禁止	
//将WEL清零  
void w25qxx_write_disable(void) {  
	spi1_cs_low();                            //使能器件   
    spi1_read_write_byte(W25X_WriteDisable);  //发送写禁止指令    
	spi1_cs_high();                            //取消片选     	      
} 

//读取SPI FLASH  
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void w25qxx_read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead) {
 	uint16_t i;
	spi1_cs_low();                             //使能器件
    spi1_read_write_byte(W25X_ReadData);       //发送读取命令
    if(W25QXX_TYPE == W25Q256) {          //如果是W25Q256的话地址为4字节的，要发送最高8位
        spi1_read_write_byte((uint8_t)((ReadAddr)>>24));
    }
    spi1_read_write_byte((uint8_t)((ReadAddr)>>16));   //发送24bit地址
    spi1_read_write_byte((uint8_t)((ReadAddr)>>8));
    spi1_read_write_byte((uint8_t)ReadAddr);
    for(i = 0; i < NumByteToRead; i++) {
        pBuffer[i] = spi1_read_write_byte(0XFF);    //循环读数
    }
	spi1_cs_high();
}

//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!	 
void w25qxx_write_page(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite) {
 	uint16_t i;  
    w25qxx_write_enable();                    //SET WEL 
	spi1_cs_low();                            //使能器件   
    spi1_read_write_byte(W25X_PageProgram);   //发送写页命令   
    if(W25QXX_TYPE == W25Q256)                //如果是W25Q256的话地址为4字节的，要发送最高8位
    {
        spi1_read_write_byte((uint8_t)((WriteAddr)>>24)); 
    }
    spi1_read_write_byte((uint8_t)((WriteAddr)>>16)); //发送24bit地址    
    spi1_read_write_byte((uint8_t)((WriteAddr)>>8));   
    spi1_read_write_byte((uint8_t)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)spi1_read_write_byte(pBuffer[i]);//循环写数  
	spi1_cs_high();                            //取消片选 
	w25qxx_wait_busy();					       //等待写入结束
} 

//等待空闲
void w25qxx_wait_busy(void) {   
	while((w25qxx_read_sr(1) & 0x01) == 0x01);   // 等待BUSY位清空
} 

//无检验写SPI FLASH 
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void w25qxx_write_no_check(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite) {
	uint16_t pageremain;
	pageremain = 256 - WriteAddr % 256; //单页剩余的字节数
	if(NumByteToWrite <= pageremain) {
       pageremain = NumByteToWrite;//不大于256个字节
    }
	while(1) {
		w25qxx_write_page(pBuffer, WriteAddr, pageremain);
		if(NumByteToWrite == pageremain) {
            break;//写入结束了
        }
	 	else {                      //NumByteToWrite>pageremain
			pBuffer += pageremain;
			WriteAddr += pageremain;
			NumByteToWrite -= pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite > 256) {
                pageremain = 256;               //一次可以写入256个字节
            }
			else {
                pageremain=NumByteToWrite; 	  //不够256个字节了
            }
		}
	}	    
}

//写SPI FLASH  
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)						
//NumByteToWrite:要写入的字节数(最大65535)   
uint8_t W25QXX_BUFFER[4096];		 
void w25qxx_write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite) { 
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;
 	uint16_t i;
	uint8_t * W25QXX_BUF;
   	W25QXX_BUF = W25QXX_BUFFER;
 	secpos = WriteAddr / 4096;//扇区地址
	secoff = WriteAddr % 4096;//在扇区内的偏移
	secremain = 4096 - secoff;//扇区剩余空间大小
 	// printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
 	if(NumByteToWrite <= secremain) {
        secremain = NumByteToWrite;//不大于4096个字节
    }
	while(1) {	
		w25qxx_read(W25QXX_BUF, secpos * 4096, 4096);//读出整个扇区的内容
		for(i = 0; i < secremain; i++) {             //校验数据
			if(W25QXX_BUF[secoff + i] != 0XFF) {
                break;              //需要擦除  	  
            }
		}
		if(i < secremain) {            //需要擦除
			w25qxx_erase_sector(secpos);      //擦除这个扇区
			for(i = 0; i < secremain; i++) {    	   //复制
				W25QXX_BUF[i + secoff] = pBuffer[i];
			}
			w25qxx_write_no_check(W25QXX_BUF, secpos * 4096, 4096);//写入整个扇区  
		}
        else {
            w25qxx_write_no_check(pBuffer, WriteAddr, secremain);//写已经擦除了的,直接写入扇区剩余区间.
        }
		if(NumByteToWrite == secremain) {
            break;                  //写入结束了
        }
		else {       //写入未结束
			secpos++;//扇区地址增1
			secoff = 0;//偏移位置为0 	 

		   	pBuffer += secremain;  //指针偏移
			WriteAddr += secremain;//写地址偏移	   
		   	NumByteToWrite -= secremain;				//字节数递减
			if(NumByteToWrite > 4096) {
                secremain = 4096;	//下一个扇区还是写不完
            }
			else {
                secremain = NumByteToWrite;			//下一个扇区可以写完了
            }
		}	 
	}	 
}

//擦除整个芯片		  
//等待时间超长...
void w25qxx_erase_chip(void) {                                   
    w25qxx_write_enable();                       //SET WEL
    w25qxx_wait_busy();
  	spi1_cs_low();                               //使能器件
    spi1_read_write_byte(W25X_ChipErase);        //发送片擦除命令
	spi1_cs_high();                              //取消片选
	w25qxx_wait_busy();   				         //等待芯片擦除结束
}
//擦除一个扇区
//dst_addr:扇区地址 根据实际容量设置
//擦除一个扇区的最少时间:150ms
void w25qxx_erase_sector(uint32_t dst_addr) {  
	//监视falsh擦除情况,测试用   
 	//printf("fe:%x\r\n",Dst_Addr);
 	dst_addr*=4096;
    w25qxx_write_enable();                  //SET WEL 	 
    w25qxx_wait_busy();   
  	spi1_cs_low();                            //使能器件   
    spi1_read_write_byte(W25X_SectorErase);   //发送扇区擦除指令 
    if(W25QXX_TYPE == W25Q256)                //如果是W25Q256的话地址为4字节的，要发送最高8位
    {
        spi1_read_write_byte((uint8_t)((dst_addr)>>24)); 
    }
    spi1_read_write_byte((uint8_t)((dst_addr)>>16));  //发送24bit地址    
    spi1_read_write_byte((uint8_t)((dst_addr)>>8));   
    spi1_read_write_byte((uint8_t)dst_addr);  
	spi1_cs_high();                            //取消片选     	      
    w25qxx_wait_busy();   				    //等待擦除完成
}

//进入掉电模式
void w25qxx_power_down(void) { 
  	int i = 500;
    spi1_cs_low();                             //使能器件   
    spi1_read_write_byte(W25X_PowerDown);      //发送掉电命令  
	spi1_cs_high();                            //取消片选     	      
    while(i--);                                //等待TPD  
}   
//唤醒
void w25qxx_wakeup(void) {
    int i = 500;
  	spi1_cs_low();                                 //使能器件   
    spi1_read_write_byte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
	spi1_cs_high();                                //取消片选     	      
    while(i--);                                    //等待TRES1
}   
