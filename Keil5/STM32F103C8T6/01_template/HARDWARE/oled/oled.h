#ifndef __OLED_H
#define __OLED_H


#include "stm32f1xx_hal.h"
#include "iic.h"

#define USE_ZHENGDIAN  0

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

#define Max_Column	128

#if !USE_ZHENGDIAN
void oled_send_command(uint8_t command);
void oled_send_data(uint8_t data);
void oled_fill_picture(uint8_t fill_data);
void oled_set_pos(uint8_t x, uint8_t y);
void oled_display_on(void);
void oled_display_off(void);
void oled_clear(void);
void oled_show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t char_size);
uint32_t oled_pow(uint8_t m, uint8_t n);
void oled_show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size2);
void oled_show_string(uint8_t x, uint8_t y, uint8_t *chr, uint8_t char_size);
void oled_show_chinese(uint8_t x, uint8_t y, uint8_t no);
void oled_draw_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[]);
void oled_init(void);
void oled_show_one_chinese(uint8_t x, uint8_t y, char *Font);
#else

void oled_init(void);
void oled_send_command(uint8_t command);
void oled_send_data(uint8_t data);
void oled_refresh_gram(void);
void oled_clear(void);
void oled_display_on(void);
void oled_display_off(void);
void oled_draw_point(uint8_t x, uint8_t y, uint8_t t);
void oled_fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot);
void oled_wr_byte(uint8_t data, uint8_t cmd);
void oled_show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t mode);
#endif

#endif
