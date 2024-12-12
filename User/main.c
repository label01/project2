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
#include "adc.h"
#include "nvic.h"
#include "buzzer.h"

extern vu16 ADC_DMA_IN[3]; //声明外部变量

vu16 arr1[10];//放入10个ADC值，求平均值
vu16 arr2[10];
vu16 arr3[10];


int main(void) {//主程序
	
	u8 buffer[3];//保存温度传感器数据
	u8 MENU=1; //主菜单编号
	u8 a;//for循环参数
	u8 dht11_data[2];
	u16 Average_ADC1;//第1个ADC平均值
	u16 Average_ADC2;//第2个ADC平均值
	u16 Average_ADC3;//第3个ADC平均值
	u16 max;//ADC 最大值
	u16 min;//ADC 最小值
	u8 high_temp=0;//高温数值
	u8 low_temp=0;//低温数值
	
	
	delay_ms(100);//上电时等待其他器件就绪
	RCC_Configuration();//系统时钟初始化
	I2C_Configuration();//I2C初始化
	LM75A_GetTemp(buffer);//读取lm75a的温度数据
	ENCODER_Init();//旋转编码器初始化
	OLED0561_Init();//OLED屏幕初始化
	RTC_Config();//初始化时钟
	ENCODER_INT_INIT();//编码器中断初始化
	BKP_Configuration();//初始化bkp
	ADC_Configuration(); //初始化ADC
	BUZZER_Init();
	OLED_DISPLAY_LIT(155);//调整屏幕亮度
	delay_ms(1000);
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
			INVERSE_OLED_DISPLAY_8x16(0,0*8,2+0x30); //数字反显示2
			INVERSE_OLED_DISPLAY_8x16(0,1*8,0x20); //显示白面
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
			INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //数字反显示3
			INVERSE_OLED_DISPLAY_8x16(0,1*8,0x20); //显示白面
			INVERSE_OLED_DISPLAY_16x16(0,1*16,13);
			INVERSE_OLED_DISPLAY_16x16(0,2*16,6);//汉字显示	 设置菜单
			INVERSE_OLED_DISPLAY_16x16(0,3*16,7);
			INVERSE_OLED_DISPLAY_16x16(0,4*16,1);
			INVERSE_OLED_DISPLAY_16x16(0,5*16,2);
			INVERSE_OLED_DISPLAY_16x16(0,6*16,13); //显示白面
			INVERSE_OLED_DISPLAY_16x16(0,7*16,13);

			OLED_DISPLAY_16x16(2,0*16,21);//汉字显示	 高温设置
			OLED_DISPLAY_16x16(2,1*16,3);
			OLED_DISPLAY_16x16(2,2*16,6);
			OLED_DISPLAY_16x16(2,3*16,7);
			
			OLED_DISPLAY_16x16(4,0*16,22);//汉字显示	 低温设置
			OLED_DISPLAY_16x16(4,1*16,3);
			OLED_DISPLAY_16x16(4,2*16,6);
			OLED_DISPLAY_16x16(4,3*16,7);
			
			OLED_DISPLAY_16x16(6,0*16,8);//汉字显示	 光照
			OLED_DISPLAY_16x16(6,1*16,9);
			OLED_DISPLAY_16x16(6,2*16,6);
			OLED_DISPLAY_16x16(6,3*16,7);
			
			
			//备份菜单值,掉电保存在备份寄存器
			BKP_WriteBackupRegister(BKP_DR1, MENU);
			
			MENU=31;//自动跳转到 菜单31 显示高低温 光照
		}
		
		//菜单4 高温设置菜单
		if(MENU==4){
			OLED_DISPLAY_CLEAR();//清屏
			INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //数字反显示3
			INVERSE_OLED_DISPLAY_8x16(0,1*8,0x2d); //反显字符 -
			INVERSE_OLED_DISPLAY_8x16(0,2*8,1+0x30); //数字反显示1
			INVERSE_OLED_DISPLAY_8x16(0,3*8,0x20); //显示白面
			INVERSE_OLED_DISPLAY_16x16(0,2*16,13);
			INVERSE_OLED_DISPLAY_16x16(0,3*16,6);//汉字显示	 设置菜单
			INVERSE_OLED_DISPLAY_16x16(0,4*16,7);
			INVERSE_OLED_DISPLAY_16x16(0,5*16,1);
			INVERSE_OLED_DISPLAY_16x16(0,6*16,2);
			INVERSE_OLED_DISPLAY_16x16(0,7*16,13); //显示白面
			

			OLED_DISPLAY_16x16(2,0*16,21);//汉字显示	 高温
			OLED_DISPLAY_16x16(2,1*16,3);
			OLED_DISPLAY_8x16(2,4*8,10+0x30);//冒号
			OLED_DISPLAY_16x16(4,0*16,22);//汉字显示	 低温
			OLED_DISPLAY_16x16(4,1*16,3);
			OLED_DISPLAY_8x16(4,4*8,10+0x30);//冒号
			INVERSE_OLED_DISPLAY_16x16(6,0*16,8);//汉字显示	 光照
			INVERSE_OLED_DISPLAY_16x16(6,1*16,9);
			OLED_DISPLAY_8x16(6,4*8,10+0x30);//冒号
		}
		
		//菜单5 低温设置菜单
		if(MENU==5){
			OLED_DISPLAY_CLEAR();//清屏
			INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //数字反显示3
			INVERSE_OLED_DISPLAY_8x16(0,1*8,0x20); //显示白面
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
		}
		
		//菜单6 光照设置菜单
		if(MENU==6){
			OLED_DISPLAY_CLEAR();//清屏
			INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //数字反显示3
			INVERSE_OLED_DISPLAY_8x16(0,1*8,0x20); //显示白面
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
		}
		
		//菜单7 湿度设置菜单
		if(MENU==7){
			OLED_DISPLAY_CLEAR();//清屏
			INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //数字反显示3
			INVERSE_OLED_DISPLAY_8x16(0,1*8,0x20); //显示白面
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
		}
		
		
		
		//菜单11 获取温度， 时间，显示到菜单1
		if(MENU==11){//菜单切换
			RTC_Get();//获取RTC时钟
			LM75A_GetTemp(buffer);//获取温度值
			
			OLED_DISPLAY_8x16(6, 8*8, rhour/10+0x30);//显示小时值
			OLED_DISPLAY_8x16(6, 9*8, rhour%10+0x30);
			OLED_DISPLAY_8x16(6, 10*8, 0x3a);//显示冒号
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
		}
		
		
		//菜单21 显示模拟量的数值
		if(MENU==21){
			Average_ADC1=0;//复位变量值
			Average_ADC2=0;
			Average_ADC3=0;
			
			for(a=0;a<10;a++){
				arr1[a]=ADC_DMA_IN[0]; //读取第1路ADC值
				arr2[a]=ADC_DMA_IN[1]; //读取第2路ADC值
				arr3[a]=ADC_DMA_IN[2]; //读取第3路ADC值
				while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//判断ADC转换结束标识位
				delay_us(1);//延时1ms等待ADC把数据写入ADC1_DR， 不延时1ms会出现死机
				ADC_ClearFlag(ADC1, ADC_FLAG_EOC);//清除标志位
			}
			
			/*求第一路平均值*/
			max=arr1[0];
			min=arr1[0];
			for(a=0; a<10; a++){
				if(max<arr1[a]) max=arr1[a];//检索最大值
				if(min>arr1[a])	min=arr1[a];//检索最小值
			}
			for(a=0; a<10; a++){
				Average_ADC1=Average_ADC1+arr1[a];//求和
			}
			Average_ADC1=(Average_ADC1-max-min)/8;//求平均值
			
			/*求第二路平均值*/
			max=arr2[0];
			min=arr2[0];
			for(a=0; a<10; a++){
				if(max<arr2[a]) max=arr2[a];//检索最大值
				if(min>arr2[a])	min=arr2[a];//检索最小值
			}
			for(a=0; a<10; a++){
				Average_ADC2=Average_ADC2+arr2[a];//求和
			}
			Average_ADC2=(Average_ADC2-max-min)/8;//求平均值
			
			/*求第三路平均值*/
			max=arr3[0];
			min=arr3[0];
			for(a=0; a<10; a++){
				if(max<arr3[a]) max=arr3[a];//检索最大值
				if(min>arr3[a])	min=arr3[a];//检索最小值
			}
			for(a=0; a<10; a++){
				Average_ADC3=Average_ADC3+arr3[a];//求和
			}
			Average_ADC3=(Average_ADC3-max-min)/8;//求平均值
			/*显示第一路平均值*/
			OLED_DISPLAY_8x16(2,5*8,Average_ADC1/1000+0x30);
			OLED_DISPLAY_8x16(2,6*8,Average_ADC1%1000/100+0x30);                         
			OLED_DISPLAY_8x16(2,7*8,Average_ADC1%100/10+0x30);
			OLED_DISPLAY_8x16(2,8*8,Average_ADC1%10+0x30);
			/*显示第二路平均值*/
			OLED_DISPLAY_8x16(4,5*8,Average_ADC2/1000+0x30);
			OLED_DISPLAY_8x16(4,6*8,Average_ADC2%1000/100+0x30);                         
			OLED_DISPLAY_8x16(4,7*8,Average_ADC2%100/10+0x30);
			OLED_DISPLAY_8x16(4,8*8,Average_ADC2%10+0x30);
			/*显示第三路平均值*/
			OLED_DISPLAY_8x16(6,5*8,Average_ADC3/1000+0x30);
			OLED_DISPLAY_8x16(6,6*8,Average_ADC3%1000/100+0x30);                         
			OLED_DISPLAY_8x16(6,7*8,Average_ADC3%100/10+0x30);
			OLED_DISPLAY_8x16(6,8*8,Average_ADC3%10+0x30);
		}
		
		//菜单31 设置菜单
		if(MENU==31){
			OLED_DISPLAY_CLEAR();//清屏
			INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //数字反显示3
			INVERSE_OLED_DISPLAY_8x16(0,1*8,0x20); //显示白面
			INVERSE_OLED_DISPLAY_16x16(0,1*16,13);
			INVERSE_OLED_DISPLAY_16x16(0,2*16,6);//汉字显示	 设置菜单
			INVERSE_OLED_DISPLAY_16x16(0,3*16,7);
			INVERSE_OLED_DISPLAY_16x16(0,4*16,1);
			INVERSE_OLED_DISPLAY_16x16(0,5*16,2);
			INVERSE_OLED_DISPLAY_16x16(0,6*16,13); //显示白面
			INVERSE_OLED_DISPLAY_16x16(0,7*16,13);

			OLED_DISPLAY_16x16(2,0*16,21);//汉字显示	 高温设置
			OLED_DISPLAY_16x16(2,1*16,3);
			OLED_DISPLAY_16x16(2,2*16,6);
			OLED_DISPLAY_16x16(2,3*16,7);
			
			OLED_DISPLAY_16x16(4,0*16,22);//汉字显示	 低温设置
			OLED_DISPLAY_16x16(4,1*16,3);
			OLED_DISPLAY_16x16(4,2*16,6);
			OLED_DISPLAY_16x16(4,3*16,7);
			
			OLED_DISPLAY_16x16(6,0*16,8);//汉字显示	 光照设置
			OLED_DISPLAY_16x16(6,1*16,9);
			OLED_DISPLAY_16x16(6,2*16,6);
			OLED_DISPLAY_16x16(6,3*16,7);
		}
		
		//菜单32 菜单3子菜单， 高温设置反显
		if(MENU==32){
			OLED_DISPLAY_CLEAR();//清屏
			INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //数字反显示3
			INVERSE_OLED_DISPLAY_8x16(0,1*8,0x20); //显示白面
			INVERSE_OLED_DISPLAY_16x16(0,1*16,13);
			INVERSE_OLED_DISPLAY_16x16(0,2*16,6);//汉字显示	 设置菜单
			INVERSE_OLED_DISPLAY_16x16(0,3*16,7);
			INVERSE_OLED_DISPLAY_16x16(0,4*16,1);
			INVERSE_OLED_DISPLAY_16x16(0,5*16,2);
			INVERSE_OLED_DISPLAY_16x16(0,6*16,13); //显示白面
			INVERSE_OLED_DISPLAY_16x16(0,7*16,13);
			
			INVERSE_OLED_DISPLAY_16x16(2,0*16,21); //反显高温设置
			INVERSE_OLED_DISPLAY_16x16(2,1*16,3);
			INVERSE_OLED_DISPLAY_16x16(2,2*16,6); 
			INVERSE_OLED_DISPLAY_16x16(2,3*16,7);
			INVERSE_OLED_DISPLAY_16x16(2,4*16,13); //显示白面
			INVERSE_OLED_DISPLAY_16x16(2,5*16,13);
			INVERSE_OLED_DISPLAY_16x16(2,6*16,13); //显示白面
			INVERSE_OLED_DISPLAY_16x16(2,7*16,13);
			
			OLED_DISPLAY_16x16(4,0*16,22);//汉字显示	 低温设置
			OLED_DISPLAY_16x16(4,1*16,3);
			OLED_DISPLAY_16x16(4,2*16,6);
			OLED_DISPLAY_16x16(4,3*16,7);
			OLED_DISPLAY_16x16(6,0*16,8);//汉字显示	 光照设置
			OLED_DISPLAY_16x16(6,1*16,9);
			OLED_DISPLAY_16x16(6,2*16,6);
			OLED_DISPLAY_16x16(6,3*16,7);
		}
		
		
	
		//菜单33 菜单3子菜单， 低温设置反显
		if(MENU==33){
			OLED_DISPLAY_CLEAR();//清屏
			INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //数字反显示3
			INVERSE_OLED_DISPLAY_8x16(0,1*8,0x20); //显示白面
			INVERSE_OLED_DISPLAY_16x16(0,1*16,13);
			INVERSE_OLED_DISPLAY_16x16(0,2*16,6);//汉字显示	 设置菜单
			INVERSE_OLED_DISPLAY_16x16(0,3*16,7);
			INVERSE_OLED_DISPLAY_16x16(0,4*16,1);
			INVERSE_OLED_DISPLAY_16x16(0,5*16,2);
			INVERSE_OLED_DISPLAY_16x16(0,6*16,13); //显示白面
			INVERSE_OLED_DISPLAY_16x16(0,7*16,13);
			
			OLED_DISPLAY_16x16(2,0*16,21);//汉字显示	 高温设置
			OLED_DISPLAY_16x16(2,1*16,3);
			OLED_DISPLAY_16x16(2,2*16,6);
			OLED_DISPLAY_16x16(2,3*16,7);
			
			INVERSE_OLED_DISPLAY_16x16(4,0*16,22); //反显设置
			INVERSE_OLED_DISPLAY_16x16(4,1*16,3);
			INVERSE_OLED_DISPLAY_16x16(4,2*16,6); 
			INVERSE_OLED_DISPLAY_16x16(4,3*16,7);
			INVERSE_OLED_DISPLAY_16x16(4,4*16,13); //显示白面
			INVERSE_OLED_DISPLAY_16x16(4,5*16,13);
			INVERSE_OLED_DISPLAY_16x16(4,6*16,13); //显示白面
			INVERSE_OLED_DISPLAY_16x16(4,7*16,13);
			
			OLED_DISPLAY_16x16(6,0*16,8);//汉字显示	 光照设置
			OLED_DISPLAY_16x16(6,1*16,9);
			OLED_DISPLAY_16x16(6,2*16,6);
			OLED_DISPLAY_16x16(6,3*16,7);
		}
		
		//菜单34 菜单3子菜单， 光照设置反显
		if(MENU==34){
			OLED_DISPLAY_CLEAR();//清屏
			INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //数字反显示3
			INVERSE_OLED_DISPLAY_8x16(0,1*8,0x20); //显示白面
			INVERSE_OLED_DISPLAY_16x16(0,1*16,13);
			INVERSE_OLED_DISPLAY_16x16(0,2*16,6);//汉字显示	 设置菜单
			INVERSE_OLED_DISPLAY_16x16(0,3*16,7);
			INVERSE_OLED_DISPLAY_16x16(0,4*16,1);
			INVERSE_OLED_DISPLAY_16x16(0,5*16,2);
			INVERSE_OLED_DISPLAY_16x16(0,6*16,13); //显示白面
			INVERSE_OLED_DISPLAY_16x16(0,7*16,13);
			
			OLED_DISPLAY_16x16(2,0*16,21);//汉字显示	 高温设置
			OLED_DISPLAY_16x16(2,1*16,3);
			OLED_DISPLAY_16x16(2,2*16,6);
			OLED_DISPLAY_16x16(2,3*16,7);
			
			OLED_DISPLAY_16x16(4,0*16,22);//汉字显示	 低温设置
			OLED_DISPLAY_16x16(4,1*16,3);
			OLED_DISPLAY_16x16(4,2*16,6);
			OLED_DISPLAY_16x16(4,3*16,7);
			
			INVERSE_OLED_DISPLAY_16x16(6,0*16,8); //反显光照设置
			INVERSE_OLED_DISPLAY_16x16(6,1*16,9);
			INVERSE_OLED_DISPLAY_16x16(6,2*16,6); 
			INVERSE_OLED_DISPLAY_16x16(6,3*16,7);
			INVERSE_OLED_DISPLAY_16x16(6,4*16,13); //显示白面
			INVERSE_OLED_DISPLAY_16x16(6,5*16,13);
			INVERSE_OLED_DISPLAY_16x16(6,6*16,13); //显示白面
			INVERSE_OLED_DISPLAY_16x16(6,7*16,13);
		}
		
		
		//菜单35 菜单3子菜单， 湿度设置反显
		if(MENU==35){
			OLED_DISPLAY_CLEAR();//清屏
			INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //数字反显示3
			INVERSE_OLED_DISPLAY_8x16(0,1*8,0x20); //显示白面
			INVERSE_OLED_DISPLAY_16x16(0,1*16,13);
			INVERSE_OLED_DISPLAY_16x16(0,2*16,6);//汉字显示	 设置菜单
			INVERSE_OLED_DISPLAY_16x16(0,3*16,7);
			INVERSE_OLED_DISPLAY_16x16(0,4*16,1);
			INVERSE_OLED_DISPLAY_16x16(0,5*16,2);
			INVERSE_OLED_DISPLAY_16x16(0,6*16,13); //显示白面
			INVERSE_OLED_DISPLAY_16x16(0,7*16,13);

			OLED_DISPLAY_16x16(2,0*16,22);//汉字显示	 低温设置
			OLED_DISPLAY_16x16(2,1*16,3);
			OLED_DISPLAY_16x16(2,2*16,6);
			OLED_DISPLAY_16x16(2,3*16,7);
			
			OLED_DISPLAY_16x16(4,0*16,8);//汉字显示	 光照设置
			OLED_DISPLAY_16x16(4,1*16,9);
			OLED_DISPLAY_16x16(4,2*16,6);
			OLED_DISPLAY_16x16(4,3*16,7);
			
			INVERSE_OLED_DISPLAY_16x16(6,0*16,5);//汉字显示	 湿度设置
			INVERSE_OLED_DISPLAY_16x16(6,1*16,4);
			INVERSE_OLED_DISPLAY_16x16(6,2*16,6);
			INVERSE_OLED_DISPLAY_16x16(6,3*16,7);
			INVERSE_OLED_DISPLAY_16x16(6,4*16,13); //显示白面
			INVERSE_OLED_DISPLAY_16x16(6,5*16,13);
			INVERSE_OLED_DISPLAY_16x16(6,6*16,13); //显示白面
			INVERSE_OLED_DISPLAY_16x16(6,7*16,13);
		}
		
		
		
		
		
		
		
		
		
		
		//中断判断切换菜单
		if(INT_MARK>0){
			BUZZER_BEEP1();
			//主菜单队列循环切换
			if(MENU==11 && INT_MARK==1) MENU=2;//右转
			if(MENU==11 && INT_MARK==2) MENU=3;//左转
			if(MENU==21 && INT_MARK==1) MENU=3;//右转
			if(MENU==21 && INT_MARK==2) MENU=1;//左转
			if(MENU==31 && INT_MARK==1) MENU=1;//右转
			if(MENU==31 && INT_MARK==2) MENU=2;//左转
			
			if(MENU==31 && INT_MARK==3) MENU=32;//确定，进入菜单3子菜单队列 高温设置
			
			if(MENU==32 && INT_MARK==1) MENU=33;//右转递加，进入低温设置
			if(MENU==33 && INT_MARK==1) MENU=34;//右转，光照设置
			if(MENU==34 && INT_MARK==1) MENU=35;//右转，湿度设置
			if(MENU==35 && INT_MARK==1) MENU=32;//右转，高温设置
			if(MENU==32 && INT_MARK==2) MENU=35;//左转，湿度设置
			if(MENU==35 && INT_MARK==2) MENU=34;//左转，光照设置
			if(MENU==34 && INT_MARK==2) MENU=33;//左转，低温设置
			if(MENU==33 && INT_MARK==2) MENU=32;//左转，高温设置
			
			INT_MARK=0;//标志位清零
		}
		
	}
}
