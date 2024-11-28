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
	int MENU=1; //���˵����
	
	
	delay_ms(100);//�ϵ�ʱ�ȴ�������������
	RCC_Configuration();//ϵͳʱ�ӳ�ʼ��
	I2C_Configuration();//I2C��ʼ��
	LM75A_GetTemp(buffer);//��ȡlm75a���¶�����
	ENCODER_Init();//��ת��������ʼ��
	RTC_Config();//��ʼ��ʱ��
	
	OLED0561_Init();//OLED��Ļ��ʼ��
	OLED_DISPLAY_LIT(155);//������Ļ����
	
	
	while (1) {
		//����ѭ������
		
		//�˵�1
		if(MENU==1){
			OLED_DISPLAY_CLEAR(); //����
			INVERSE_OLED_DISPLAY_8x16(0, 0*8, 1+0x30);//0x30 ��ASII����� 0��16������
			INVERSE_OLED_DISPLAY_8x16(0, 1*8, 0x20);//Ӣ�Ŀո���ʾ����
			INVERSE_OLED_DISPLAY_16x16(0, 1*16, 13);//���ֿո���ʾ����
			INVERSE_OLED_DISPLAY_16x16(0, 2*16, 13);
			INVERSE_OLED_DISPLAY_16x16(0, 3*16, 0);//��
			INVERSE_OLED_DISPLAY_16x16(0, 4*16, 1);//��
			INVERSE_OLED_DISPLAY_16x16(0, 5*16, 2);//��
			INVERSE_OLED_DISPLAY_16x16(0, 6*16, 13);//���ֿո���ʾ����
			INVERSE_OLED_DISPLAY_16x16(0, 7*16, 13);//���ֿո���ʾ����
			OLED_DISPLAY_16x16(2,0*16,14);//������ʾ	 ��ǰ
			OLED_DISPLAY_16x16(2,1*16,15);
			OLED_DISPLAY_16x16(2,2*16,3);//������ʾ	 �¶�
			OLED_DISPLAY_16x16(2,3*16,4);
			OLED_DISPLAY_8x16(2,8*8,10+0x30);//ð��
			OLED_DISPLAY_16x16(2,7*16,16);//��ʾ��
			OLED_DISPLAY_16x16(4,0*16,14);//������ʾ	 ��ǰ
			OLED_DISPLAY_16x16(4,1*16,15);
			OLED_DISPLAY_16x16(4,2*16,5);//������ʾ	 ʪ��
			OLED_DISPLAY_16x16(4,3*16,4);
			OLED_DISPLAY_8x16(4,8*8,10+0x30);//ð��
			OLED_DISPLAY_8x16(4,9*8,2+0x30);
			OLED_DISPLAY_8x16(4,10*8,5+0x30);
			OLED_DISPLAY_8x16(4,11*8,0x25);//��ʾ�ٷֺ�
			OLED_DISPLAY_16x16(6,0*16,17);//������ʾ ����ʱ��	 
			OLED_DISPLAY_16x16(6,1*16,18);
			OLED_DISPLAY_16x16(6,2*16,19);	
			OLED_DISPLAY_16x16(6,3*16,20);
		}
		
		//�˵�2
		if(MENU==2){
			OLED_DISPLAY_CLEAR();//����
			INVERSE_OLED_DISPLAY_8x16(0,0*8,2+0x30); //���ַ���ʾ1
			INVERSE_OLED_DISPLAY_8x16(0,1*8,80+0x30); //��ʾ����
			INVERSE_OLED_DISPLAY_16x16(0,1*16,13);
			INVERSE_OLED_DISPLAY_16x16(0,2*16,13);

			INVERSE_OLED_DISPLAY_16x16(0,2*16,10); //��ʾģ�����˵�
			INVERSE_OLED_DISPLAY_16x16(0,3*16,11);
			INVERSE_OLED_DISPLAY_16x16(0,4*16,12);
			INVERSE_OLED_DISPLAY_16x16(0,5*16,1);
			INVERSE_OLED_DISPLAY_16x16(0,6*16,2);
			INVERSE_OLED_DISPLAY_16x16(0,7*16,13);

			OLED_DISPLAY_8x16_BUFFER(2,"ADC1:"); //��ʾ�ַ���
			OLED_DISPLAY_8x16_BUFFER(4,"ADC2:"); //��ʾ�ַ���
			OLED_DISPLAY_8x16_BUFFER(6,"ADC3: "); //��ʾ�ַ���
		}
		
		//�˵�3
		if(MENU==3){
			OLED_DISPLAY_CLEAR();//����
			INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //���ַ���ʾ1
			INVERSE_OLED_DISPLAY_8x16(0,1*8,80+0x30); //��ʾ����
			INVERSE_OLED_DISPLAY_16x16(0,1*16,13);
			INVERSE_OLED_DISPLAY_16x16(0,2*16,6);//������ʾ	 ���ò˵�
			INVERSE_OLED_DISPLAY_16x16(0,3*16,7);
			INVERSE_OLED_DISPLAY_16x16(0,4*16,1);
			INVERSE_OLED_DISPLAY_16x16(0,5*16,2);
			INVERSE_OLED_DISPLAY_16x16(0,6*16,13); //��ʾ����
			INVERSE_OLED_DISPLAY_16x16(0,7*16,13);

			OLED_DISPLAY_16x16(2,0*16,21);//������ʾ	 ����
			OLED_DISPLAY_16x16(2,1*16,3);
			OLED_DISPLAY_8x16(2,4*8,10+0x30);//ð��
			OLED_DISPLAY_16x16(4,0*16,22);//������ʾ	 ����
			OLED_DISPLAY_16x16(4,1*16,3);
			OLED_DISPLAY_8x16(4,4*8,10+0x30);//ð��
			INVERSE_OLED_DISPLAY_16x16(6,0*16,8);//������ʾ	 ����
			INVERSE_OLED_DISPLAY_16x16(6,1*16,9);
			OLED_DISPLAY_8x16(6,4*8,10+0x30);//ð��
		}
		
		
		
		
		
	}
}
