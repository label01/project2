



#include "touch_key.h"



void TOUCH_KEY_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ������ʱ��
	GPIO_InitTypeDef	GPIO_InitStructure; //����gpio�ĳ�ʼ��ö�ٽṹ
	GPIO_InitStructure.GPIO_Pin = TOUCH_KEY_A |TOUCH_KEY_B |TOUCH_KEY_C |TOUCH_KEY_D;//ѡ��˿�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//ѡ����������ģʽ
	GPIO_Init(TOUCH_KEYPORT, &GPIO_InitStructure);
}


















