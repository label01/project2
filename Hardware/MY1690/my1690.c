

#include "my1690.h"
#include "delay.h"


void MY1690_Init(void)//初始化
{
	USART3_Init(9600); //芯片手册要求波特率为9600
	MY1690_CMD2(0x35, 1);//设备切换
	delay_ms(25);
	MY1690_CMD2(0x33, 0);//循环方式
	delay_ms(25);
	MY1690_CMD2(0x32, 1);//设置EQ
	delay_ms(25);
	MY1690_CMD2(0x31, 10);//设置音量
	delay_ms(25);
	MY1690_STOP();
}

void MY1690_STOP(void){
	USART3_printf("\x7e\x03\x1E\x1D\xef");
}

void MY1690_PLAY(void){ //播放
	USART3_printf("\x7e\x03\x11\x12\xef"); //其中 \x 后接十六进制数据
}

void MY1690_PAUSE(void){ //暂停
	//USART3_printf("\x7e\x03\x12\x11\xef");
	//USART3_printf("\x7e\x03\x1E\x1D\xef");
	USART3_printf("\x7e\x03\x1c\x1f\xef");
}

void MY1690_PREV(void){ //上一曲
	USART3_printf("\x7e\x03\x14\x17\xef");
}

void MY1690_NEXT(void){ //下一曲
	USART3_printf("\x7e\x03\x13\x10\xef");
}

void MY1690_VUP(void){ //音量加1
	USART3_printf("\x7e\x03\x15\x16\xef");
}

void MY1690_VDOWN(void){ //音量减1
	USART3_printf("\x7e\x03\x16\x15\xef");
}



void MY1690_CMD1(u8 a){	//无参数的指令发送  a操作码
	u8 i;
	i=3^a;//取校验码（异或）
	USART_SendData(USART3 , 0x7e);	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //检查发送中断标志位
	USART_SendData(USART3 , 0x03);	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //检查发送中断标志位
	USART_SendData(USART3 , a);		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //检查发送中断标志位
	USART_SendData(USART3 , i);		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //检查发送中断标志位
	USART_SendData(USART3 , 0xef);	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //检查发送中断标志位
}
void MY1690_CMD2(u8 a,u8 b){ //有参数的指令发送 a操作码 b参数
	u8 i;
	i=4^a;//取校验码（异或）
	i=i^b;//取校验码（异或）
	USART_SendData(USART3 , 0x7e);	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //检查发送中断标志位
	USART_SendData(USART3 , 0x04);	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //检查发送中断标志位
	USART_SendData(USART3 , a);		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //检查发送中断标志位
	USART_SendData(USART3 , b);		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //检查发送中断标志位
	USART_SendData(USART3 , i);		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //检查发送中断标志位
	USART_SendData(USART3 , 0xef);	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //检查发送中断标志位
}
void MY1690_CMD3(u8 a,u16 b){ //有参数的指令发送 a操作码 b参数（16位）
	u8 i,c,d;
	c=b/0x100; //将16位参数分成2个8位参数
	d=b%0x100;
	i=5^a;//取校验码（异或）
	i=i^c;//取校验码（异或）
	i=i^d;//取校验码（异或）
	USART_SendData(USART3 , 0x7e);	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //检查发送中断标志位
	USART_SendData(USART3 , 0x05);	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //检查发送中断标志位
	USART_SendData(USART3 , a);		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //检查发送中断标志位
	USART_SendData(USART3 , c);		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //检查发送中断标志位
	USART_SendData(USART3 , d);		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //检查发送中断标志位
	USART_SendData(USART3 , i);		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //检查发送中断标志位
	USART_SendData(USART3 , 0xef);	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //检查发送中断标志位
}

/*
* MY1690设备切换
*0-U盘 1-SD卡
*/
void MY1690_SetDevice(u8 Data){
	MY1690_CMD2(0x35, Data);//设备切换
}

/*
* MY1690设置循环方式
*0-全盘 1-文件夹 2-单曲 3-随机 4-不循环
*/
void MY1690_SetLoop(u8 Data){
	MY1690_CMD2(0x33, Data);//循环方式
}

/*
*MY1690 设置EQ
*0-NO 1-POP 2-ROCK 3-JAZZ 4-CLASSIC 5-BASS
*/
void MY1690_SetEQ(u8 Data){
	MY1690_CMD2(0x32, Data);//设置EQ
}

/*
*MY1690 设置EQ
*0-NO 1-POP 2-ROCK 3-JAZZ 4-CLASSIC 5-BASS
*/
void MY1690_SetVOL(u8 Data){
	MY1690_CMD2(0x31, Data);//设置音量
}






















