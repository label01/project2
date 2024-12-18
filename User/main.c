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
extern u8 INT_MARK; //中断标志位
vu16 arr1[10];//放入10个ADC值，求平均值
vu16 arr2[10];
vu16 arr3[10];


int main(void) {//主程序
	
	u8 buffer[3];//保存温度传感器数据
	u8 MENU=1; //主菜单编号
	u8 SUBMENU3=0;//菜单3的子菜单
	u8 a;//for循环参数
	u8 dht11_data[2];
	u16 Average_ADC1;//第1个ADC平均值
	u16 Average_ADC2;//第2个ADC平均值
	u16 Average_ADC3;//第3个ADC平均值
	u16 max;//ADC 最大值
	u16 min;//ADC 最小值
	u8 high_temp=0;//高温数值
	u8 low_temp=0;//低温数值
	u16 light_value=0;//光照值
	u8 humidity_value=0;//湿度值
	
	
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
	NVIC_Configuration();//设置中断优先级
	BUZZER_Init();
	OLED_DISPLAY_LIT(155);//调整屏幕亮度
	delay_ms(1000);
	MENU=BKP_ReadBackupRegister(BKP_DR1);
	if(MENU<1||MENU>3)MENU=1; //如果menu的备份值不在1~3的菜单列表里，默认重新赋值menu为1
	
	
//	high_temp=BKP_ReadBackupRegister(BKP_DR2);//从备用寄存器中读取高温的设置值
//	low_temp=BKP_ReadBackupRegister(BKP_DR3);//从备用寄存器中读取低温的设置值
//	light_value=BKP_ReadBackupRegister(BKP_DR4);//从备用寄存器中读取光照强度的设置值
//	humidity_value=BKP_ReadBackupRegister(BKP_DR5);//从备用寄存器中读取湿度的设置值
//	
//	//首次读取，如果数值异常，重新赋值
//	if(high_temp<0 || high_temp>99) high_temp=0;
//	if(low_temp<0 || low_temp>99) low_temp=0;
//	if(light_value<0 || light_value>4096) light_value=0;
//	if(humidity_value<0 || humidity_value>99) humidity_value=0;
	
	
	while (1) {
		/*无限循环程序*/
		
		/*菜单1  显示温湿度， 背景时间*/
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
			OLED_DISPLAY_8x16(4,11*8,0x25);//显示百分号
			OLED_DISPLAY_16x16(6,0*16,17);//汉字显示 北京时间	 
			OLED_DISPLAY_16x16(6,1*16,18);
			OLED_DISPLAY_16x16(6,2*16,19);	
			OLED_DISPLAY_16x16(6,3*16,20);
			
			
			BKP_WriteBackupRegister(BKP_DR1, MENU);//备份菜单值,掉电保存在备份寄存器
			
			MENU=11;//自动跳转到菜单11
		}
		
		/*菜单2 模拟量菜单*/
		if(MENU==2){
			
			OLED_DISPLAY_CLEAR();//清屏
			INVERSE_OLED_DISPLAY_8x16(0,0*8,2+0x30); //数字反显示2
			INVERSE_OLED_DISPLAY_8x16(0,1*8,0x20); //显示白面
			INVERSE_OLED_DISPLAY_16x16(0,1*16,13);//显示白面
			INVERSE_OLED_DISPLAY_16x16(0,2*16,13);

			INVERSE_OLED_DISPLAY_16x16(0,2*16,10); //显示模拟量菜单
			INVERSE_OLED_DISPLAY_16x16(0,3*16,11);
			INVERSE_OLED_DISPLAY_16x16(0,4*16,12);
			INVERSE_OLED_DISPLAY_16x16(0,5*16,1);
			INVERSE_OLED_DISPLAY_16x16(0,6*16,2);
			INVERSE_OLED_DISPLAY_16x16(0,7*16,13);//显示白面

			OLED_DISPLAY_8x16_BUFFER(2,"ADC1:"); //显示字符串
			OLED_DISPLAY_8x16_BUFFER(4,"ADC2:"); //显示字符串
			OLED_DISPLAY_8x16_BUFFER(6,"ADC3: "); //显示字符串
			
			
			BKP_WriteBackupRegister(BKP_DR1, MENU);//备份菜单值,掉电保存在备份寄存器
			
			MENU=21;//自动跳转到菜单21，用来设置菜单显示的数值
		}
		
		/*菜单3  设置菜单*/
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
			
			BKP_WriteBackupRegister(BKP_DR1, MENU);//备份菜单值,掉电保存在备份寄存器
			
			MENU=31;//自动跳转到 菜单31 显示高低温 光照
		}
		
		/*菜单4 高温设置菜单*/
		if(MENU==4){
			
			OLED_DISPLAY_CLEAR();//清屏
			INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //数字反显示3
			INVERSE_OLED_DISPLAY_8x16(0,1*8,0x2d); //反显字符 -
			INVERSE_OLED_DISPLAY_8x16(0,2*8,1+0x30); //数字反显示1
			INVERSE_OLED_DISPLAY_8x16(0,3*8,0x20); //显示白面
			INVERSE_OLED_DISPLAY_16x16(0,2*16,13);
			INVERSE_OLED_DISPLAY_16x16(0,3*16,21);//汉字显示	 高温设置
			INVERSE_OLED_DISPLAY_16x16(0,4*16,3);
			INVERSE_OLED_DISPLAY_16x16(0,5*16,6);
			INVERSE_OLED_DISPLAY_16x16(0,6*16,7);
			INVERSE_OLED_DISPLAY_16x16(0,7*16,13); //显示白面
			

			OLED_DISPLAY_16x16(2,0*16,21);//汉字显示	 高温
			OLED_DISPLAY_16x16(2,1*16,3);
			OLED_DISPLAY_8x16(2,4*8,10+0x30);//冒号
			INVERSE_OLED_DISPLAY_8x16(2,5*8,0x20);//空格
			INVERSE_OLED_DISPLAY_8x16(2, 6*8, high_temp/10+0x30);//高温十位
			INVERSE_OLED_DISPLAY_8x16(2, 7*8, high_temp%10+0x30);//高温个位
			INVERSE_OLED_DISPLAY_16x16(2, 4*16, 16);//摄氏度
			
			OLED_DISPLAY_16x16(4,0*16,22);//汉字显示	 低温
			OLED_DISPLAY_16x16(4,1*16,3);
			OLED_DISPLAY_8x16(4,4*8,10+0x30);//冒号
			OLED_DISPLAY_8x16(4,5*8,0x20);//空格
			OLED_DISPLAY_8x16(4, 6*8, low_temp/10+0x30);//低温十位
			OLED_DISPLAY_8x16(4, 7*8, low_temp%10+0x30);//低温个位
			OLED_DISPLAY_16x16(4, 4*16, 16);//摄氏度
			
			MENU=41;//跳转到调整菜单
		}
		
		/*菜单5 低温设置菜单*/
		if(MENU==5){
			
			OLED_DISPLAY_CLEAR();//清屏
			INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //数字反显示3
			INVERSE_OLED_DISPLAY_8x16(0,1*8,0x2d); //反显字符 -
			INVERSE_OLED_DISPLAY_8x16(0,2*8,2+0x30); //数字反显示2
			INVERSE_OLED_DISPLAY_8x16(0,3*8,0x20); //显示白面
			INVERSE_OLED_DISPLAY_16x16(0,2*16,13);
			INVERSE_OLED_DISPLAY_16x16(0,3*16,22);//汉字显示	 低温设置
			INVERSE_OLED_DISPLAY_16x16(0,4*16,3);
			INVERSE_OLED_DISPLAY_16x16(0,5*16,6);
			INVERSE_OLED_DISPLAY_16x16(0,6*16,7);
			INVERSE_OLED_DISPLAY_16x16(0,7*16,13); //显示白面

			OLED_DISPLAY_16x16(2,0*16,21);//汉字显示	 高温
			OLED_DISPLAY_16x16(2,1*16,3);
			OLED_DISPLAY_8x16(2,4*8,10+0x30);//冒号
			OLED_DISPLAY_8x16(2,5*8,0x20);//空格
			OLED_DISPLAY_8x16(2, 6*8, high_temp/10+0x30);//高温十位
			OLED_DISPLAY_8x16(2, 7*8, high_temp%10+0x30);//高温个位
			OLED_DISPLAY_16x16(2, 4*16, 16);//摄氏度
			OLED_DISPLAY_16x16(4,0*16,22);//汉字显示	 低温
			OLED_DISPLAY_16x16(4,1*16,3);
			OLED_DISPLAY_8x16(4,4*8,10+0x30);//冒号
			INVERSE_OLED_DISPLAY_8x16(4,5*8,0x20);//空格
			INVERSE_OLED_DISPLAY_8x16(4, 6*8, low_temp/10+0x30);//低温十位
			INVERSE_OLED_DISPLAY_8x16(4, 7*8, low_temp%10+0x30);//低温个位
			INVERSE_OLED_DISPLAY_16x16(4, 4*16, 16);//摄氏度
			
			MENU=51;//进入低温调整菜单
			
		}
		
		/*菜单6 光照设置菜单*/
		if(MENU==6){
			
			OLED_DISPLAY_CLEAR();//清屏
			INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //数字反显示3
			INVERSE_OLED_DISPLAY_8x16(0,1*8,0x2d); //反显字符 -
			INVERSE_OLED_DISPLAY_8x16(0,2*8,3+0x30); //数字反显示3
			INVERSE_OLED_DISPLAY_8x16(0,3*8,0x20); //显示白面
			INVERSE_OLED_DISPLAY_16x16(0,2*16,13);
			INVERSE_OLED_DISPLAY_16x16(0,3*16,8);//汉字显示	 光照设置
			INVERSE_OLED_DISPLAY_16x16(0,4*16,9);
			INVERSE_OLED_DISPLAY_16x16(0,5*16,6);
			INVERSE_OLED_DISPLAY_16x16(0,6*16,7);
			INVERSE_OLED_DISPLAY_16x16(0,7*16,13); //显示白面

			OLED_DISPLAY_16x16(4,0*16,8);//汉字显示	 光照
			OLED_DISPLAY_16x16(4,1*16,9);
			OLED_DISPLAY_8x16(4,4*8,10+0x30);//冒号
			OLED_DISPLAY_8x16(4,5*8,0x20);//空格
			INVERSE_OLED_DISPLAY_8x16(4, 6*8, light_value/1000+0x30);//反色显示光照强度设置值
			INVERSE_OLED_DISPLAY_8x16(4, 7*8, light_value/100%10+0x30);
			INVERSE_OLED_DISPLAY_8x16(4, 8*8, light_value/10%10+0x30);
			INVERSE_OLED_DISPLAY_8x16(4, 9*8, light_value%10+0x30);
			
			MENU=61; //进入光照调整菜单
		}
		
		/*菜单7 湿度设置菜单*/
		if(MENU==7){
			
			OLED_DISPLAY_CLEAR();//清屏
			INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //数字反显示3
			INVERSE_OLED_DISPLAY_8x16(0,1*8,0x2d); //反显字符 -
			INVERSE_OLED_DISPLAY_8x16(0,2*8,4+0x30); //数字反显示4
			INVERSE_OLED_DISPLAY_8x16(0,3*8,0x20); //显示白面
			INVERSE_OLED_DISPLAY_16x16(0,2*16,13);
			INVERSE_OLED_DISPLAY_16x16(0,3*16,5);//汉字显示	 湿度设置
			INVERSE_OLED_DISPLAY_16x16(0,4*16,4);
			INVERSE_OLED_DISPLAY_16x16(0,5*16,6);
			INVERSE_OLED_DISPLAY_16x16(0,6*16,7);
			INVERSE_OLED_DISPLAY_16x16(0,7*16,13); //显示白面
			
			OLED_DISPLAY_16x16(4,0*16,5);//汉字显示	 湿度
			OLED_DISPLAY_16x16(4,1*16,4);
			OLED_DISPLAY_8x16(4,4*8,10+0x30);//冒号
			OLED_DISPLAY_8x16(4,5*8,0x20);//空格
			INVERSE_OLED_DISPLAY_8x16(4, 6*8, humidity_value/10+0x30);//反色显示湿度设置值
			INVERSE_OLED_DISPLAY_8x16(4, 7*8, humidity_value%10+0x30);
			INVERSE_OLED_DISPLAY_8x16(4, 8*8, 0x25);
			
			MENU=71;//进入湿度调整菜单
			
		}
		
		
		/*菜单11 获取温度， 时间，显示到菜单1*/
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
			
			/*读取湿度值*/
			OLED_DISPLAY_8x16(4,9*8,2+0x30);
			OLED_DISPLAY_8x16(4,10*8,5+0x30);
			
			/*刷新湿度*/
			
		}
		
		
		/*菜单21 显示模拟量的数值*/
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
		
		/*菜单31 设置菜单*/
		if(MENU==31){
			delay_ms(50);
		}
		
		/*主菜单旋钮切换*/
		if(INT_MARK!=0 && MENU>=11 && MENU<=31){
			/*主菜单队列循环切换*/
			switch(MENU){
				case 11:
					if(INT_MARK==1){//右转
						MENU=2;
					}else if(INT_MARK==2){//左转
						MENU=3;
					}
					break;
				case 21:
					if(INT_MARK==1){//右转
						MENU=3;
					}else if(INT_MARK==2){//左转
						MENU=1;
					}
					break;
				case 31:
					if(INT_MARK==1){//右转
						MENU=1;
					}else if(INT_MARK==2){//左转
						MENU=2;
					}else if(INT_MARK==3){
						INT_MARK=0;//标志位清零
						SUBMENU3=1;//菜单3子菜单选项
						MENU=0;//不进入主菜单页面
						BUZZER_BEEP3();//确认提示音
					}
					break;
				default:
					MENU=1;
					break;
			}
			
			INT_MARK=0;//标志位清零
		}
		
		
		
//		/*菜单3子菜单中断切换*/
//		if(INT_MARK!=0 && MENU>=32 && MENU<=35){
//			/*高温设置*/
//			if(MENU==32 && INT_MARK==1) 
//			{
//				MENU=33;//右转，进入低温设置
//				INT_MARK=0;//标志位清零
//				OLED_DISPLAY_CLEAR();//清屏
//				//BUZZER_BEEP1();//提示音
//			}
//			if(MENU==32 && INT_MARK==2) 
//			{
//				MENU=35;//左转，湿度设置
//				INT_MARK=0;//标志位清零
//				OLED_DISPLAY_CLEAR();//清屏
//				//BUZZER_BEEP1();//提示音
//			}
//			/*低温设置*/
//			if(MENU==33 && INT_MARK==1) 
//			{
//				MENU=34;//右转，光照设置
//				INT_MARK=0;//标志位清零
//				OLED_DISPLAY_CLEAR();//清屏
//				//BUZZER_BEEP1();//提示音
//			}
//			if(MENU==33 && INT_MARK==2)
//			{
//				MENU=32;//左转，高温设置
//				INT_MARK=0;//标志位清零
//				OLED_DISPLAY_CLEAR();//清屏
//				//BUZZER_BEEP1();//提示音
//			}
//			/*光照设置*/
//			if(MENU==34 && INT_MARK==1)
//			{
//				MENU=35;//右转，湿度设置
//				INT_MARK=0;//标志位清零
//				OLED_DISPLAY_CLEAR();//清屏
//				//BUZZER_BEEP1();//提示音
//			}
//			if(MENU==34 && INT_MARK==2) 
//			{
//				MENU=33;//左转，低温设置
//				INT_MARK=0;//标志位清零
//				OLED_DISPLAY_CLEAR();//清屏
//				//BUZZER_BEEP1();//提示音
//			}	
//			/*湿度设置*/
//			if(MENU==35 && INT_MARK==1) 
//			{
//				MENU=32;//右转，高温设置
//				INT_MARK=0;//标志位清零
//				OLED_DISPLAY_CLEAR();//清屏
//				//BUZZER_BEEP1();//提示音
//			}	
//			if(MENU==35 && INT_MARK==2) 
//			{
//				MENU=34;//左转，光照设置
//				INT_MARK=0;//标志位清零
//				OLED_DISPLAY_CLEAR();//清屏
//				//BUZZER_BEEP1();//提示音
//			}
//			/*旋钮按下，进入菜单3子菜单设置*/
//			if(INT_MARK==3)
//			{
//				MENU=MENU%10+2;
//				INT_MARK=0;
//				BUZZER_BEEP3();//确认提示音
//			}
//		}
		
		
		/*菜单32 菜单3子菜单， 高温设置反显*/
//		if(MENU==100){
//			delay_ms(100);
//			INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //数字反显示3
//			INVERSE_OLED_DISPLAY_8x16(0,1*8,0x20); //显示白面
//			INVERSE_OLED_DISPLAY_16x16(0,1*16,13);
//			INVERSE_OLED_DISPLAY_16x16(0,2*16,6);//汉字显示	 设置菜单
//			INVERSE_OLED_DISPLAY_16x16(0,3*16,7);
//			INVERSE_OLED_DISPLAY_16x16(0,4*16,1);
//			INVERSE_OLED_DISPLAY_16x16(0,5*16,2);
//			INVERSE_OLED_DISPLAY_16x16(0,6*16,13); //显示白面
//			INVERSE_OLED_DISPLAY_16x16(0,7*16,13);
//			
//			INVERSE_OLED_DISPLAY_16x16(2,0*16,21); //反显高温设置
//			INVERSE_OLED_DISPLAY_16x16(2,1*16,3);
//			INVERSE_OLED_DISPLAY_16x16(2,2*16,6); 
//			INVERSE_OLED_DISPLAY_16x16(2,3*16,7);
//			INVERSE_OLED_DISPLAY_16x16(2,4*16,13); //显示白面
//			INVERSE_OLED_DISPLAY_16x16(2,5*16,13);
//			INVERSE_OLED_DISPLAY_16x16(2,6*16,13); //显示白面
//			INVERSE_OLED_DISPLAY_16x16(2,7*16,13);
//			
//			OLED_DISPLAY_16x16(4,0*16,22);//汉字显示	 低温设置
//			OLED_DISPLAY_16x16(4,1*16,3);
//			OLED_DISPLAY_16x16(4,2*16,6);
//			OLED_DISPLAY_16x16(4,3*16,7);
//			OLED_DISPLAY_16x16(4,4*16,13);//显示空格
//			OLED_DISPLAY_16x16(4,5*16,13);
//			OLED_DISPLAY_16x16(4,6*16,13); 
//			OLED_DISPLAY_16x16(4,7*16,13);
//			
//			OLED_DISPLAY_16x16(6,0*16,8);//汉字显示	 光照设置
//			OLED_DISPLAY_16x16(6,1*16,9);
//			OLED_DISPLAY_16x16(6,2*16,6);
//			OLED_DISPLAY_16x16(6,3*16,7);
//			OLED_DISPLAY_16x16(6,4*16,13);//显示空格
//			OLED_DISPLAY_16x16(6,5*16,13);
//			OLED_DISPLAY_16x16(6,6*16,13); 
//			OLED_DISPLAY_16x16(6,7*16,13);
//		}
//	
//		/*菜单33 菜单3子菜单， 低温设置反显*/
//		if(MENU==200){
//			delay_ms(100);
//			INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //数字反显示3
//			INVERSE_OLED_DISPLAY_8x16(0,1*8,0x20); //显示白面
//			INVERSE_OLED_DISPLAY_16x16(0,1*16,13);
//			INVERSE_OLED_DISPLAY_16x16(0,2*16,6);//汉字显示	 设置菜单
//			INVERSE_OLED_DISPLAY_16x16(0,3*16,7);
//			INVERSE_OLED_DISPLAY_16x16(0,4*16,1);
//			INVERSE_OLED_DISPLAY_16x16(0,5*16,2);
//			INVERSE_OLED_DISPLAY_16x16(0,6*16,13); //显示白面
//			INVERSE_OLED_DISPLAY_16x16(0,7*16,13);
//			
//			OLED_DISPLAY_16x16(2,0*16,21);//汉字显示	 高温设置
//			OLED_DISPLAY_16x16(2,1*16,3);
//			OLED_DISPLAY_16x16(2,2*16,6);
//			OLED_DISPLAY_16x16(2,3*16,7);
//			OLED_DISPLAY_16x16(2,4*16,13);//显示空格
//			OLED_DISPLAY_16x16(2,5*16,13);
//			OLED_DISPLAY_16x16(2,6*16,13); 
//			OLED_DISPLAY_16x16(2,7*16,13);
//			
//			INVERSE_OLED_DISPLAY_16x16(4,0*16,22); //反显设置
//			INVERSE_OLED_DISPLAY_16x16(4,1*16,3);
//			INVERSE_OLED_DISPLAY_16x16(4,2*16,6); 
//			INVERSE_OLED_DISPLAY_16x16(4,3*16,7);
//			INVERSE_OLED_DISPLAY_16x16(4,4*16,13); //显示白面
//			INVERSE_OLED_DISPLAY_16x16(4,5*16,13);
//			INVERSE_OLED_DISPLAY_16x16(4,6*16,13); //显示白面
//			INVERSE_OLED_DISPLAY_16x16(4,7*16,13);
//			
//			OLED_DISPLAY_16x16(6,0*16,8);//汉字显示	 光照设置
//			OLED_DISPLAY_16x16(6,1*16,9);
//			OLED_DISPLAY_16x16(6,2*16,6);
//			OLED_DISPLAY_16x16(6,3*16,7);
//			OLED_DISPLAY_16x16(6,4*16,13);
//			OLED_DISPLAY_16x16(6,5*16,13);
//			OLED_DISPLAY_16x16(6,6*16,13); 
//			OLED_DISPLAY_16x16(6,7*16,13);
//		}
//		
//		/*菜单34 菜单3子菜单， 光照设置反显*/
//		if(MENU==250){
//			delay_ms(100);
//			INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //数字反显示3
//			INVERSE_OLED_DISPLAY_8x16(0,1*8,0x20); //显示白面
//			INVERSE_OLED_DISPLAY_16x16(0,1*16,13);
//			INVERSE_OLED_DISPLAY_16x16(0,2*16,6);//汉字显示	 设置菜单
//			INVERSE_OLED_DISPLAY_16x16(0,3*16,7);
//			INVERSE_OLED_DISPLAY_16x16(0,4*16,1);
//			INVERSE_OLED_DISPLAY_16x16(0,5*16,2);
//			INVERSE_OLED_DISPLAY_16x16(0,6*16,13); //显示白面
//			INVERSE_OLED_DISPLAY_16x16(0,7*16,13);
//			
//			OLED_DISPLAY_16x16(2,0*16,21);//汉字显示	 高温设置
//			OLED_DISPLAY_16x16(2,1*16,3);
//			OLED_DISPLAY_16x16(2,2*16,6);
//			OLED_DISPLAY_16x16(2,3*16,7);
//			OLED_DISPLAY_16x16(2,4*16,13);//显示空格
//			OLED_DISPLAY_16x16(2,5*16,13);
//			OLED_DISPLAY_16x16(2,6*16,13);//显示空格
//			OLED_DISPLAY_16x16(2,7*16,13);
//			
//			OLED_DISPLAY_16x16(4,0*16,22);//汉字显示	 低温设置
//			OLED_DISPLAY_16x16(4,1*16,3);
//			OLED_DISPLAY_16x16(4,2*16,6);
//			OLED_DISPLAY_16x16(4,3*16,7);
//			OLED_DISPLAY_16x16(4,4*16,13);//显示空格
//			OLED_DISPLAY_16x16(4,5*16,13);
//			OLED_DISPLAY_16x16(4,6*16,13); //显示空格
//			OLED_DISPLAY_16x16(4,7*16,13);
//			
//			INVERSE_OLED_DISPLAY_16x16(6,0*16,8); //反显光照设置
//			INVERSE_OLED_DISPLAY_16x16(6,1*16,9);
//			INVERSE_OLED_DISPLAY_16x16(6,2*16,6); 
//			INVERSE_OLED_DISPLAY_16x16(6,3*16,7);
//			INVERSE_OLED_DISPLAY_16x16(6,4*16,13); //显示白面
//			INVERSE_OLED_DISPLAY_16x16(6,5*16,13);
//			INVERSE_OLED_DISPLAY_16x16(6,6*16,13); //显示白面
//			INVERSE_OLED_DISPLAY_16x16(6,7*16,13);
//		}
//		
//		
//		/*菜单35 菜单3子菜单， 湿度设置反显*/
//		if(MENU==230){
//			OLED_DISPLAY_16x16(2,0*16,22);//汉字显示	 低温设置
//			OLED_DISPLAY_16x16(2,1*16,3);
//			OLED_DISPLAY_16x16(2,2*16,6);
//			OLED_DISPLAY_16x16(2,3*16,7);
//			OLED_DISPLAY_16x16(2,4*16,13);
//			OLED_DISPLAY_16x16(2,5*16,13);
//			OLED_DISPLAY_16x16(2,6*16,13); 
//			OLED_DISPLAY_16x16(2,7*16,13);
//			
//			OLED_DISPLAY_16x16(4,0*16,8);//汉字显示	 光照设置
//			OLED_DISPLAY_16x16(4,1*16,9);
//			OLED_DISPLAY_16x16(4,2*16,6);
//			OLED_DISPLAY_16x16(4,3*16,7);
//			OLED_DISPLAY_16x16(4,4*16,13);//显示空格
//			OLED_DISPLAY_16x16(4,5*16,13);
//			OLED_DISPLAY_16x16(4,6*16,13); 
//			OLED_DISPLAY_16x16(4,7*16,13);
//			
//			INVERSE_OLED_DISPLAY_16x16(6,0*16,5);//汉字显示	 湿度设置
//			INVERSE_OLED_DISPLAY_16x16(6,1*16,4);
//			INVERSE_OLED_DISPLAY_16x16(6,2*16,6);
//			INVERSE_OLED_DISPLAY_16x16(6,3*16,7);
//			INVERSE_OLED_DISPLAY_16x16(6,4*16,13); //显示白面
//			INVERSE_OLED_DISPLAY_16x16(6,5*16,13);
//			INVERSE_OLED_DISPLAY_16x16(6,6*16,13); //显示白面
//			INVERSE_OLED_DISPLAY_16x16(6,7*16,13);
//		}


//		
//		/*菜单41 高温设置数值调整*/
//		if(MENU==41 && INT_MARK!=0){
//			if(INT_MARK==1){
//				//BUZZER_BEEP1();//提示音
//				INT_MARK=0;//复位中断值
//				high_temp++;
//				if(high_temp>99) high_temp=low_temp;
//			}
//			if(INT_MARK==2){
//				//BUZZER_BEEP1();//提示音
//				INT_MARK=0;//复位中断值
//				if(high_temp==low_temp) high_temp=100;
//				high_temp--;
//			}
//			if(INT_MARK==3){//按下旋钮，确认保存设置值并返回菜单3
//				INT_MARK=0;//复位中断值
//				MENU=3;//返回菜单3
//				BUZZER_BEEP4();//退出音效
//			}
//			
//			/*显示高温调整数值*/
//			INVERSE_OLED_DISPLAY_8x16(2,5*8,0x20);//空格
//			INVERSE_OLED_DISPLAY_8x16(2, 6*8, high_temp/10+0x30);//高温十位
//			INVERSE_OLED_DISPLAY_8x16(2, 7*8, high_temp%10+0x30);//高温个位
//			INVERSE_OLED_DISPLAY_16x16(2, 4*16, 16);//摄氏度
//		}
//		
//		/*菜单51 低温设置数值调整*/
//		if(MENU==51 && INT_MARK!=0){
//			if(INT_MARK==1){
//				//BUZZER_BEEP1();//提示音
//				INT_MARK=0;//复位中断值
//				low_temp++;
//				if(low_temp>high_temp) low_temp=0;//温度高于高温，低温设置为0度
//			}
//			if(INT_MARK==2){
//				//BUZZER_BEEP1();//提示音
//				INT_MARK=0;//复位中断值
//				if(low_temp==0) low_temp=high_temp;
//				low_temp--;
//			}
//			if(INT_MARK==3){//按下旋钮，确认保存设置值并返回菜单3
//				INT_MARK=0;//复位中断值
//				MENU=3;//返回菜单3
//				BUZZER_BEEP4();//退出音效
//			}
//			
//			/*显示低温调整数值*/
//			INVERSE_OLED_DISPLAY_8x16(4,5*8,0x20);//空格
//			INVERSE_OLED_DISPLAY_8x16(4, 6*8, low_temp/10+0x30);//低温十位
//			INVERSE_OLED_DISPLAY_8x16(4, 7*8, low_temp%10+0x30);//低温个位
//			INVERSE_OLED_DISPLAY_16x16(4, 4*16, 16);//摄氏度
//		}
//		
//		/*光照强度设置数值调整*/
//		if(MENU==61 && INT_MARK!=0){
//			if(INT_MARK==1){
//				//BUZZER_BEEP1();//提示音
//				INT_MARK=0;//复位中断值
//				light_value++;
//				if(light_value>4095) light_value=0;
//				
//			}
//			if(INT_MARK==2){
//				//BUZZER_BEEP1();//提示音
//				INT_MARK=0;//复位中断值
//				if(light_value==0) light_value=4096;
//				light_value--;
//			}
//			if(INT_MARK==3){//按下旋钮，确认保存设置值并返回菜单3
//				INT_MARK=0;//复位中断值
//				MENU=3;//返回菜单3
//				BUZZER_BEEP4();//退出音效
//			}
//			
//			/*显示光照强度调整数值*/
//			INVERSE_OLED_DISPLAY_8x16(4, 6*8, light_value/1000+0x30);//反色显示光照强度设置值
//			INVERSE_OLED_DISPLAY_8x16(4, 7*8, light_value/100%10+0x30);
//			INVERSE_OLED_DISPLAY_8x16(4, 8*8, light_value/10%10+0x30);
//			INVERSE_OLED_DISPLAY_8x16(4, 9*8, light_value%10+0x30);
//		}
//		
//		/*湿度设置数值调整*/
//		if(MENU==71 && INT_MARK!=0){
//			if(INT_MARK==1){
//				//BUZZER_BEEP1();//提示音
//				INT_MARK=0;//复位中断值
//				humidity_value++;
//				if(humidity_value>90) humidity_value=0;
//				
//			}
//			if(INT_MARK==2){
//				//BUZZER_BEEP1();//提示音
//				INT_MARK=0;//复位中断值
//				if(humidity_value==0) humidity_value=90;
//				humidity_value--;
//			}
//			if(INT_MARK==3){//按下旋钮，确认保存设置值并返回菜单3
//				INT_MARK=0;//复位中断值
//				MENU=3;//返回菜单3
//				BUZZER_BEEP4();//退出音效
//			}
//			/*显示湿度调整数值*/
//			INVERSE_OLED_DISPLAY_8x16(4, 6*8, humidity_value/10+0x30);//反色显示湿度设置值
//			INVERSE_OLED_DISPLAY_8x16(4, 7*8, humidity_value%10+0x30);
//			INVERSE_OLED_DISPLAY_8x16(4, 8*8, 0x25);
//		}
		
		
		
		
		
	}
}
