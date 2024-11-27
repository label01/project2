/*
��Ʒ�� �������� DoYoung Studio
��������
��д�ˣ� ����
��дʱ�䣺 201 ������
Ӳ��֧�֣� STM32F103C8 �ⲿ���� 8M
�޸���־��
1-
*/
#include "stm32f10x.h" //STM ͷ�ļ�
#include "delay.h"
#include "lm75a.h"
#include "oled0561.h"
#include "encoder.h"
#include "rtc.h"



int main(void) {//������
	
	u8 buffer[3];//�����¶ȴ���������
	
	
	delay_ms(100);//�ϵ�ʱ�ȴ�������������
	RCC_Configuration();//ϵͳʱ�ӳ�ʼ��
	I2C_Configuration();//I2C��ʼ��
	LM75A_GetTemp(buffer);//��ȡlm75a���¶�����
	ENCODER_Init();//��ת��������ʼ��
	RTC_Config();//��ʼ��ʱ��
	
	OLED0561_Init();//OLED��Ļ��ʼ��
	OLED_DISPLAY_LIT(200);//������Ļ����
	OLED_DISPLAY_CLEAR();//����
	
	OLED_DISPLAY_16x16(2,2*16,4);//������ʾ	 ���ҵ���
	OLED_DISPLAY_16x16(2,3*16,5);
	OLED_DISPLAY_16x16(2,4*16,6);
	OLED_DISPLAY_16x16(2,5*16,7);
	
	
	while (1) {
		//����ѭ������
		
		delay_ms(500);//��ʱ
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
	}
}
