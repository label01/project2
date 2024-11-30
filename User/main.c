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
#include "bkp.h"



int main(void) {//主程序
	
	u8 buffer[3];//保存温度传感器数据
	u8 MENU=1; //主菜单编号
	u8 b;//获取编码器的值
	
	
	delay_ms(100);//上电时等待其他器件就绪
	RCC_Configuration();//系统时钟初始化
	I2C_Configuration();//I2C初始化
	LM75A_GetTemp(buffer);//读取lm75a的温度数据
	ENCODER_Init();//旋转编码器初始化
	RTC_Config();//初始化时钟
	BKP_Configuration();//初始化bkp
	
	OLED0561_Init();//OLED屏幕初始化
	OLED_DISPLAY_LIT(155);//调整屏幕亮度
	
	MENU=BKP_ReadBackupRegister(BKP_DR1);
	if(MENU<1||MENU>3)MENU=1; //如果menu的备份值不在1~3的菜单列表里，默认重新赋值menu为1
	
	while (1) {
		//无限循环程序
		
		//菜单1  显示温湿度， 背景时间
		if(MENU==1){
			OLED_DISPLAY_CLEAR(); //清屏
			INVERSE_OLED_DISPLAY_8x16(0, 0*8, 1+0x30);//0x30 是ASII码表里 0的16进制数
			INVERSE_OLED_DISPLAY_8x16(0, 1*8, 0x20);//英文空格显示白面
			INVERSE_OLED_DISPLAY_16x16(0, 1*16, 13);//汉字空格显示白面
			INVERSE_OLED_DISPLAY_16x16(0, 2*16, 13);
			INVERSE_OLED_DISPLAY_16x16(0, 3*16, 0);//主
			INVERSE_OLED_DISPLAY_16x16(0, 4*16, 1);//菜
			INVERSE_OLED_DISPLAY_16x16(0, 5*16, 2);//单
			INVERSE_OLED_DISPLAY_16x16(0, 6*16, 13);//汉字空格显示白面
			INVERSE_OLED_DISPLAY_16x16(0, 7*16, 13);//汉字空格显示白面
			
			OLED_DISPLAY_16x16(2,0*16,14);//汉字显示	 当前
			OLED_DISPLAY_16x16(2,1*16,15);
			OLED_DISPLAY_16x16(2,2*16,3);//汉字显示	 温度
			OLED_DISPLAY_16x16(2,3*16,4);
			OLED_DISPLAY_8x16(2,8*8,10+0x30);//冒号
			OLED_DISPLAY_16x16(2,7*16,16);//显示℃
			
			OLED_DISPLAY_16x16(4,0*16,14);//汉字显示	 当前
			OLED_DISPLAY_16x16(4,1*16,15);
			OLED_DISPLAY_16x16(4,2*16,5);//汉字显示	 湿度
			OLED_DISPLAY_16x16(4,3*16,4);
			OLED_DISPLAY_8x16(4,8*8,10+0x30);//冒号
			OLED_DISPLAY_8x16(4,9*8,2+0x30);
			OLED_DISPLAY_8x16(4,10*8,5+0x30);
			OLED_DISPLAY_8x16(4,11*8,0x25);//显示百分号
			OLED_DISPLAY_16x16(6,0*16,17);//汉字显示 北京时间	 
			OLED_DISPLAY_16x16(6,1*16,18);
			OLED_DISPLAY_16x16(6,2*16,19);	
			OLED_DISPLAY_16x16(6,3*16,20);
			
			//备份菜单值,掉电保存在备份寄存器
			BKP_WriteBackupRegister(BKP_DR1, MENU);
			
			MENU=11;//自动跳转到菜单11
			
		}
		
		//菜单2
		if(MENU==2){
			OLED_DISPLAY_CLEAR();//清屏
			INVERSE_OLED_DISPLAY_8x16(0,0*8,2+0x30); //数字反显示1
			INVERSE_OLED_DISPLAY_8x16(0,1*8,80+0x30); //显示白面
			INVERSE_OLED_DISPLAY_16x16(0,1*16,13);
			INVERSE_OLED_DISPLAY_16x16(0,2*16,13);

			INVERSE_OLED_DISPLAY_16x16(0,2*16,10); //显示模拟量菜单
			INVERSE_OLED_DISPLAY_16x16(0,3*16,11);
			INVERSE_OLED_DISPLAY_16x16(0,4*16,12);
			INVERSE_OLED_DISPLAY_16x16(0,5*16,1);
			INVERSE_OLED_DISPLAY_16x16(0,6*16,2);
			INVERSE_OLED_DISPLAY_16x16(0,7*16,13);

			OLED_DISPLAY_8x16_BUFFER(2,"ADC1:"); //显示字符串
			OLED_DISPLAY_8x16_BUFFER(4,"ADC2:"); //显示字符串
			OLED_DISPLAY_8x16_BUFFER(6,"ADC3: "); //显示字符串
			
			//备份菜单值,掉电保存在备份寄存器
			BKP_WriteBackupRegister(BKP_DR1, MENU);
			
			MENU=21;//自动跳转到菜单21，用来设置菜单显示的数值
		}
		
		//菜单3
		if(MENU==3){
			OLED_DISPLAY_CLEAR();//清屏
			INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //数字反显示1
			INVERSE_OLED_DISPLAY_8x16(0,1*8,80+0x30); //显示白面
			INVERSE_OLED_DISPLAY_16x16(0,1*16,13);
			INVERSE_OLED_DISPLAY_16x16(0,2*16,6);//汉字显示	 设置菜单
			INVERSE_OLED_DISPLAY_16x16(0,3*16,7);
			INVERSE_OLED_DISPLAY_16x16(0,4*16,1);
			INVERSE_OLED_DISPLAY_16x16(0,5*16,2);
			INVERSE_OLED_DISPLAY_16x16(0,6*16,13); //显示白面
			INVERSE_OLED_DISPLAY_16x16(0,7*16,13);

			OLED_DISPLAY_16x16(2,0*16,21);//汉字显示	 高温
			OLED_DISPLAY_16x16(2,1*16,3);
			OLED_DISPLAY_8x16(2,4*8,10+0x30);//冒号
			OLED_DISPLAY_16x16(4,0*16,22);//汉字显示	 低温
			OLED_DISPLAY_16x16(4,1*16,3);
			OLED_DISPLAY_8x16(4,4*8,10+0x30);//冒号
			INVERSE_OLED_DISPLAY_16x16(6,0*16,8);//汉字显示	 光照
			INVERSE_OLED_DISPLAY_16x16(6,1*16,9);
			OLED_DISPLAY_8x16(6,4*8,10+0x30);//冒号
			
			//备份菜单值,掉电保存在备份寄存器
			BKP_WriteBackupRegister(BKP_DR1, MENU);
			
			MENU=31;//自动跳转到 菜单31 显示高低温 光照
		}
		
		
		
		//菜单11 获取温度， 时间，显示到菜单1
		if(MENU==11){//菜单切换
			RTC_Get();//获取RTC时钟
			LM75A_GetTemp(buffer);//获取温度值
			
			OLED_DISPLAY_8x16(6, 8*8, rhour/10+0x30);//显示小时值
			OLED_DISPLAY_8x16(6, 9*8, rhour%10+0x30);
			OLED_DISPLAY_8x16(6, 10*8, ":");//显示冒号
			OLED_DISPLAY_8x16(6, 11*8, rmin/10+0x30);//显示分钟值
			OLED_DISPLAY_8x16(6, 12*8, rmin%10+0x30);
			OLED_DISPLAY_8x16(6, 13*8, 0x3a);//显示冒号
			OLED_DISPLAY_8x16(6, 14*8, rsec/10+0x30);//显示秒数值
			OLED_DISPLAY_8x16(6, 15*8, rsec%10+0x30);
			
			if(buffer[0]) OLED_DISPLAY_8x16(2, 9*8, 0x2d);//显示负号， 存储的温度值，第一个存储的是数值的正负， 为1时，温度为负
			else OLED_DISPLAY_8x16(2, 9*8, 0x20);//整数显示为空格
			
			OLED_DISPLAY_8x16(2, 10*8, buffer[1]/10+0x30);//温度值
			OLED_DISPLAY_8x16(2, 11*8, buffer[1]%10+0x30);
			OLED_DISPLAY_8x16(2, 12*8, 0x2e);//小数点
			OLED_DISPLAY_8x16(2, 13*8, buffer[2]/10+0x30);
			
			b=ENCODER_READ();//读出旋转编码器左右旋转的值
			if(b==1) MENU=2;
			if(b==2) MENU=3;
		}
		
		
		//菜单21 显示模拟量的数值
		if(MENU==21){
			b=ENCODER_READ();//读出旋转编码器左右旋转的值
			if(b==1) MENU=3;
			if(b==2) MENU=1;
		
		}
		
		//菜单31 显示高低温 光照
		if(MENU==31){
			b=ENCODER_READ();//读出旋转编码器左右旋转的值
			if(b==1) MENU=1;
			if(b==2) MENU=2;
		}
		
		
	}
}
