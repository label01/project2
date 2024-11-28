#ifndef __OLED0561_H
#define __OLED0561_H

#include "sys.h"
#include "i2c.h"




#define OLED0561_ADD	0x78 //写在数据手册里的， 如果十五脚接VDD 从地址就变为 0111101b,也就是0x7A
#define COM		0x00
#define DAT		0x40



void OLED0561_Init(void);//初始化
void OLED_DISPLAY_ON (void);//OLED屏开显示
void OLED_DISPLAY_OFF (void);//OLED屏关显示
void OLED_DISPLAY_LIT (u8 x);//OLED屏亮度设置（0~255）
void OLED_DISPLAY_CLEAR(void);//清屏操作
void OLED_DISPLAY_8x16(u8 x,u8 y,u16 w);//显示8x16的单个字符 
void INVERSE_OLED_DISPLAY_8x16(u8 x, u8 y, u16 w);//反色显示英文字符
void OLED_DISPLAY_8x16_BUFFER(u8 row,u8 *str);//显示8x16的字符串

void OLED_DISPLAY_16x16(u8 x, u8 y, u16 w); //汉字显示
void INVERSE_OLED_DISPLAY_16x16(u8 x, u8 y, u16 w); //汉字 反色显示

void OLED_DISPLAY_PIC1(void);//显示图片






#endif


