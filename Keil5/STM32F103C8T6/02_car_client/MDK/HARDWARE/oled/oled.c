#include "oled.h"
#include "oledfont.h"
#include "w25q64.h"
#include "stdlib.h"




uint8_t OLED_GRAM[128][8];
void oled_send_command(uint8_t command) {
    iic_start();
    iic_send_byte(0x78);
    iic_wait_ack();
    iic_send_byte(0x00);
    iic_wait_ack();
    iic_send_byte(command);
    iic_wait_ack();
    iic_stop();
}

void oled_send_data(uint8_t data) {
    iic_start();
    iic_send_byte(0x78);
    iic_wait_ack();
    iic_send_byte(0x40);
    iic_wait_ack();
    iic_send_byte(data);
    iic_wait_ack();
    iic_stop();
}

void oled_wr_byte(uint8_t data, uint8_t cmd) {
    if(cmd) {
        oled_send_data(data);
    }
    else {
        oled_send_command(data);
    }
}

// 以下为正点原子OLED库
// 更新显存到OLED
void oled_refresh_gram(void) {
    uint8_t i, n;
    for(i = 0; i < 8; i++) {
        oled_wr_byte(0xb0+i, OLED_CMD);
        oled_wr_byte(0x00, OLED_CMD);
        oled_wr_byte(0x10, OLED_CMD);
        for(n = 0; n < 128; n++) {
            oled_wr_byte(OLED_GRAM[n][i], OLED_DATA);
        }
    }
}

#if USE_ZHENGDIAN
// OLED清屏函数
void oled_clear(void) {
    uint8_t i, n;  
	for(i = 0; i < 8; i++) {
        for(n = 0; n < 128; n++) {
            OLED_GRAM[n][i] = 0X00;  
        }
    }
	oled_refresh_gram();//更新显示
}

// OLED画点函数
void oled_draw_point(uint8_t x, uint8_t y, uint8_t t) {
    uint8_t pos, bx, temp=0;
	if(x > 127 || y > 63) {
        return;//超出范围了.
    }
	pos = 7 - y / 8;
	bx = y % 8;
	temp = 1 << (7 - bx);
	if(t) {
        OLED_GRAM[x][pos] |= temp;
    }
	else {
        OLED_GRAM[x][pos] &= ~temp;
    }
}


//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,清空;1,填充
void oled_fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot) {  
	uint8_t x, y;
	for(x = x1; x <= x2; x++) {
		for(y = y1; y <= y2; y++) {
            oled_draw_point(x, y, dot);
        }
	}													    
	oled_refresh_gram();//更新显示
}

//开启OLED显示
void oled_display_on(void) {
	oled_wr_byte(0X8D, OLED_CMD);  //SET DCDC命令
	oled_wr_byte(0X14, OLED_CMD);  //DCDC ON
	oled_wr_byte(0XAF, OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void oled_display_off(void) {
	oled_wr_byte(0X8D, OLED_CMD);  //SET DCDC命令
	oled_wr_byte(0X10, OLED_CMD);  //DCDC OFF
	oled_wr_byte(0XAE, OLED_CMD);  //DISPLAY OFF
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 12/16/24
void oled_show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t mode) {
    uint8_t temp, t, t1;
	uint8_t y0 = y;
	uint8_t csize = (16/8+((16%8)?1:0))*(16/2);		//得到字体一个字符对应点阵集所占的字节数
	chr = chr - ' ';//得到偏移后的值		 
    for(t = 0; t < csize; t++) {   
        temp = asc2_1608[chr][t];
        for(t1 = 0; t1 < 8; t1++) {
			if(temp & 0x80) {
                oled_draw_point(x, y, mode);
            }
			else {
                oled_draw_point(x, y, !mode);
            }
			temp <<= 1;
			y++;
			if((y - y0) == 16) {
				y = y0;
				x++;
				break;
			}
		}  	 
    }          
}

#endif


#if !USE_ZHENGDIAN 
// 以下为OLED库
void oled_fill_picture(uint8_t fill_data) {
	uint8_t m, n;
	for(m = 0; m < 8; m++)
	{
		oled_wr_byte(0xb0+m, 0);		//page0-page1
		oled_wr_byte(0x00, 0);		    //low column start address
		oled_wr_byte(0x10, 0);		    //high column start address
		for(n = 0; n < 128; n++)
        {
            oled_wr_byte(fill_data, 1);
        }
	}
}

//坐标设置
void oled_set_pos(uint8_t x, uint8_t y) { 	
    oled_wr_byte(0xb0+y, OLED_CMD);
	oled_wr_byte(((x&0xf0)>>4)|0x10, OLED_CMD);
	oled_wr_byte((x&0x0f), OLED_CMD); 
}

//开启OLED显示
void oled_display_on(void) {
	oled_wr_byte(0X8D, OLED_CMD);  //SET DCDC命令
	oled_wr_byte(0X14, OLED_CMD);  //DCDC ON
	oled_wr_byte(0XAF, OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void oled_display_off(void) {
	oled_wr_byte(0X8D, OLED_CMD);  //SET DCDC命令
	oled_wr_byte(0X10, OLED_CMD);  //DCDC OFF
	oled_wr_byte(0XAE, OLED_CMD);  //DISPLAY OFF
}

//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  

void oled_clear(void) {  
	uint8_t i, n;		    
	for(i = 0; i < 8; i++) {  
		oled_wr_byte(0xb0+i, OLED_CMD);    //设置页地址（0~7）
		oled_wr_byte(0x00, OLED_CMD);      //设置显示位置—列低地址
		oled_wr_byte(0x10, OLED_CMD);      //设置显示位置—列高地址   
		for(n = 0; n < 128; n++) {
            oled_wr_byte(0, OLED_DATA); 
        }
	} //更新显示
}


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void oled_show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t char_size) {      	
	uint8_t c = 0, i = 0;	
    c = chr - ' ';//得到偏移后的值
    if(x > Max_Column-1) {
        x = 0;
        y = y + 2;
    }
    if(char_size == 16) {
        oled_set_pos(x, y);	
        for(i = 0; i < 8; i++) {
            oled_wr_byte(F8X16[c*16+i], OLED_DATA);
        }
        oled_set_pos(x, y+1);
        for(i = 0; i < 8; i++) {
            oled_wr_byte(F8X16[c*16+i+8], OLED_DATA);
        }
    }
    else {
        oled_set_pos(x, y);
        for(i = 0; i < 6; i++) {
            oled_wr_byte(F6x8[c][i], OLED_DATA);
        }
    }
}

//m^n函数
uint32_t oled_pow(uint8_t m, uint8_t n) {
	uint32_t result=1;	 
	while(n--) {
        result *= m;
    }
	return result;
}

//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void oled_show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size2) {         	
	uint8_t t, temp;
	uint8_t enshow = 0;						   
	for(t = 0; t < len; t++) {
		temp = (num / oled_pow(10, len-t-1)) % 10;
		if(enshow == 0 && t < (len-1)) {
			if(temp == 0) {
				oled_show_char(x+(size2/2)*t, y, ' ', size2);
				continue;
			}
            else {
                enshow=1;    
            }
		}
	 	oled_show_char(x+(size2/2)*t, y, temp+'0', size2); 
	}
}

//显示一个字符号串
void oled_show_string(uint8_t x, uint8_t y, uint8_t *chr, uint8_t char_size) {
	unsigned char j=0;
	while (chr[j] != '\0') {		
        oled_show_char(x, y, chr[j], char_size);
		x += 8;
		if(x > 120) {
            x = 0;
            y += 2;
        }
		j++;
	}
}
// 显示汉字
void oled_show_chinese(uint8_t x, uint8_t y, uint8_t no) {
	uint8_t t, adder = 0;
	oled_set_pos(x, y);	
   for(t = 0; t < 16; t++)
   {
       oled_wr_byte(Hzk[2*no][t], OLED_DATA);
       adder += 1;
   }	
	oled_set_pos(x, y+1);	
   for(t = 0; t < 16; t++)
	{	
       oled_wr_byte(Hzk[2*no+1][t], OLED_DATA);
       adder += 1;
   }					
}

// 仅支持单个汉字的显示
void oled_show_one_chinese(uint8_t x, uint8_t y, char *Font) {
	char *pFont = Font;
	uint8_t GBKL,GBKH;// tmp;
	uint16_t SIZE = 16*16/8;        //计算字节数
//	uint16_t y0 = y;        //记录第一行显示的初始位置
//    uint16_t x1 = x;        //记录显示的初始位置
//	uint16_t y1 = y;
//	tmp = x;
//	x = y;
//	y = tmp;
	uint16_t i; // j;
	uint32_t Addr_offset;       //汉字的偏移地址
	uint8_t pBuff[32];//  = malloc(SIZE);      //动态分配空间
	while(*pFont != '\0')
	{
		//计算汉字的偏移地址
		GBKH = *pFont++;        //高字节
		GBKL = *pFont++;        //低字节
		if(GBKL < 0x7F) 
		{
			Addr_offset = ((GBKH-0x81)*190 + GBKL - 0x40)*(16 * 2);
		}
		else
		{
			Addr_offset = ((GBKH-0x81)*190 + GBKL - 0x41)*(16 * 2);
		}
		//从flash中取出一个汉字
		// switch(16)
		// {
		//	case 16:W25Q64_ReadData(Addr_offset,pBuff,SIZE);break;
		//	default :W25Q64_ReadData(Addr_offset,pBuff,SIZE);break;
		//}
        w25qxx_read(pBuff, Addr_offset, SIZE);
		// for(i = 0; i < 32; i++)
		// {
		// 	printf("0x%02X ", pBuff[i]);
		// }
		// printf("\r\n");
        // 显示一个汉字
        oled_set_pos(x, y);
        for(i = 0; i < 16; i++) {
            oled_send_data(pBuff[i]);
        }
        oled_set_pos(x, y+1);
        for(i = 0; i < 16; i++) {
            oled_send_data(pBuff[16+i]);
        }

		// //显示一个汉字
		// for(i=0;i<SIZE;i++)
		// {
		// 	tmp = *(pBuff+i);
		// 	for(j=0;j<8;j++)
		// 	{
		// 		if(tmp & 0x80)	//高位先发
		// 		{
        //             oled_set_pos(x, y);
                    
		// 			// LCD_DrawDot(x,y,colour);
		// 		}
		// 		tmp <<= 1;
		// 		y++;
		// 	}
		// 	x++;
		// 	if(x-x1 == 16)
		// 	{
		// 		x = x1;
		// 		y1 += 8;
		// 	}
		// 	y = y1;
		// }
		//一个汉字显示完成，为下一个汉字显示做准备
		// x += 16;
		// if(LCD_WIDTH - x < size)//考虑是否需要换行
		// {
		// 	y0 += size;
		// 	x = 0;
		// }
		// y = y0;
	}
	// free(pBuff);//释放空间
}


/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void oled_draw_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[]) { 	
    uint16_t j = 0;
    uint8_t x, y;
  
    if(y1 % 8 == 0) {
        y = y1 / 8;
    }
    else {
        y = y1 / 8 + 1;    
    }
	for(y = y0; y < y1; y++) {
		oled_set_pos(x0, y);
        for(x = x0; x < x1; x++)
	    {      
	    	oled_wr_byte(BMP[j++], OLED_DATA);	    	
	    }
	}
} 

#endif

//初始化SSD1306					    
void oled_init(void) { 	
    oled_wr_byte(0xAE, OLED_CMD);//--display off
	oled_wr_byte(0x00, OLED_CMD);//---set low column address
	oled_wr_byte(0x10, OLED_CMD);//---set high column address
	oled_wr_byte(0x40, OLED_CMD);//--set start line address  
	oled_wr_byte(0xB0, OLED_CMD);//--set page address
	oled_wr_byte(0x81, OLED_CMD); // contract control
	oled_wr_byte(0xFF, OLED_CMD);//--128   
	oled_wr_byte(0xA1, OLED_CMD);//set segment remap 
	oled_wr_byte(0xA6, OLED_CMD);//--normal / reverse
	oled_wr_byte(0xA8, OLED_CMD);//--set multiplex ratio(1 to 64)
	oled_wr_byte(0x3F, OLED_CMD);//--1/32 duty
	oled_wr_byte(0xC8, OLED_CMD);//Com scan direction
	oled_wr_byte(0xD3, OLED_CMD);//-set display offset
	oled_wr_byte(0x00, OLED_CMD);//
 
	oled_wr_byte(0xD5, OLED_CMD);//set osc division
	oled_wr_byte(0x80, OLED_CMD);//
 
	oled_wr_byte(0xD8, OLED_CMD);//set area color mode off
	oled_wr_byte(0x05, OLED_CMD);//
 
	oled_wr_byte(0xD9, OLED_CMD);//Set Pre-Charge Period
	oled_wr_byte(0xF1, OLED_CMD);//
 
	oled_wr_byte(0xDA, OLED_CMD);//set com pin configuartion
	oled_wr_byte(0x12, OLED_CMD);//
 
	oled_wr_byte(0xDB, OLED_CMD);//set Vcomh
	oled_wr_byte(0x30, OLED_CMD);//
 
	oled_wr_byte(0x8D, OLED_CMD);//set charge pump enable
	oled_wr_byte(0x14, OLED_CMD);//
 
	oled_wr_byte(0xAF, OLED_CMD);//--turn on oled panel

	oled_clear();

}  



