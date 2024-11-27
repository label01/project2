/*
出品： 杜洋工作室 DoYoung Studio
程序名：
编写人： 杜洋
编写时间： 201 年月日
硬件支持： STM32F103C8 外部晶振 8M
修改日志：
1-
*/
#include "stm32f10x.h" //STM 头文件
#include "delay.h"
#include "lm75a.h"
#include "oled0561.h"
int main(void) {//主程序
	
	u8 buffer[3];
	delay_ms(100);
	RCC_Configuration();
	I2C_Configuration();
	LM75A_GetTemp(buffer);
	
	OLED0561_Init();
	OLED_DISPLAY_LIT(200);
	
	
	OLED_DISPLAY_PIC1();
	delay_s(5);
	OLED_DISPLAY_CLEAR();
	OLED_DISPLAY_8x16_BUFFER(0, "  championTalk  ");
	OLED_DISPLAY_8x16_BUFFER(6, "  Temp:");
	
	OLED_DISPLAY_16x16(2,2*16,4);//汉字显示	 洋桃电子
	OLED_DISPLAY_16x16(2,3*16,5);
	OLED_DISPLAY_16x16(2,4*16,6);
	OLED_DISPLAY_16x16(2,5*16,7);
	
	
	while (1) {
		//无限循环程序
		
		LM75A_GetTemp(buffer);//读取lm75a的温度数据
		
		if(buffer[0]) OLED_DISPLAY_8x16(6, 7*8, '-');
		OLED_DISPLAY_8x16(6, 8*8, buffer[1]/10+0x30);
		OLED_DISPLAY_8x16(6,9*8,buffer[1]%10+0x30);//
		OLED_DISPLAY_8x16(6,10*8,'.');//
		OLED_DISPLAY_8x16(6,11*8,buffer[2]/10+0x30);//
		OLED_DISPLAY_8x16(6,12*8,buffer[2]%10+0x30);//
		OLED_DISPLAY_8x16(6,13*8,'C');//

		
		delay_ms(500);//延时
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
	}
}
