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
#include "encoder.h"
#include "rtc.h"



int main(void) {//主程序
	
	u8 buffer[3];//保存温度传感器数据
	
	
	delay_ms(100);//上电时等待其他器件就绪
	RCC_Configuration();//系统时钟初始化
	I2C_Configuration();//I2C初始化
	LM75A_GetTemp(buffer);//读取lm75a的温度数据
	ENCODER_Init();//旋转编码器初始化
	RTC_Config();//初始化时钟
	
	OLED0561_Init();//OLED屏幕初始化
	OLED_DISPLAY_LIT(200);//调整屏幕亮度
	OLED_DISPLAY_CLEAR();//清屏
	
	OLED_DISPLAY_16x16(2,2*16,4);//汉字显示	 洋桃电子
	OLED_DISPLAY_16x16(2,3*16,5);
	OLED_DISPLAY_16x16(2,4*16,6);
	OLED_DISPLAY_16x16(2,5*16,7);
	
	
	while (1) {
		//无限循环程序
		
		delay_ms(500);//延时
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
	}
}
