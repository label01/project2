



#include "touch_key.h"



void TOUCH_KEY_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能外设时钟
	GPIO_InitTypeDef	GPIO_InitStructure; //定义gpio的初始化枚举结构
	GPIO_InitStructure.GPIO_Pin = TOUCH_KEY_A |TOUCH_KEY_B |TOUCH_KEY_C |TOUCH_KEY_D;//选择端口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//选择上拉输入模式
	GPIO_Init(TOUCH_KEYPORT, &GPIO_InitStructure);
}


















