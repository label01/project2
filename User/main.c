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
int main(void) {//������
	
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
	
	OLED_DISPLAY_16x16(2,2*16,4);//������ʾ	 ���ҵ���
	OLED_DISPLAY_16x16(2,3*16,5);
	OLED_DISPLAY_16x16(2,4*16,6);
	OLED_DISPLAY_16x16(2,5*16,7);
	
	
	while (1) {
		//����ѭ������
		
		LM75A_GetTemp(buffer);//��ȡlm75a���¶�����
		
		if(buffer[0]) OLED_DISPLAY_8x16(6, 7*8, '-');
		OLED_DISPLAY_8x16(6, 8*8, buffer[1]/10+0x30);
		OLED_DISPLAY_8x16(6,9*8,buffer[1]%10+0x30);//
		OLED_DISPLAY_8x16(6,10*8,'.');//
		OLED_DISPLAY_8x16(6,11*8,buffer[2]/10+0x30);//
		OLED_DISPLAY_8x16(6,12*8,buffer[2]%10+0x30);//
		OLED_DISPLAY_8x16(6,13*8,'C');//

		
		delay_ms(500);//��ʱ
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
	}
}
