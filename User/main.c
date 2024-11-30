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
#include "bkp.h"



int main(void) {//������
	
	u8 buffer[3];//�����¶ȴ���������
	u8 MENU=1; //���˵����
	u8 b;//��ȡ��������ֵ
	
	
	delay_ms(100);//�ϵ�ʱ�ȴ�������������
	RCC_Configuration();//ϵͳʱ�ӳ�ʼ��
	I2C_Configuration();//I2C��ʼ��
	LM75A_GetTemp(buffer);//��ȡlm75a���¶�����
	ENCODER_Init();//��ת��������ʼ��
	RTC_Config();//��ʼ��ʱ��
	BKP_Configuration();//��ʼ��bkp
	
	OLED0561_Init();//OLED��Ļ��ʼ��
	OLED_DISPLAY_LIT(155);//������Ļ����
	
	MENU=BKP_ReadBackupRegister(BKP_DR1);
	if(MENU<1||MENU>3)MENU=1; //���menu�ı���ֵ����1~3�Ĳ˵��б��Ĭ�����¸�ֵmenuΪ1
	
	while (1) {
		//����ѭ������
		
		//�˵�1  ��ʾ��ʪ�ȣ� ����ʱ��
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
			
			//���ݲ˵�ֵ,���籣���ڱ��ݼĴ���
			BKP_WriteBackupRegister(BKP_DR1, MENU);
			
			MENU=11;//�Զ���ת���˵�11
			
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
			
			//���ݲ˵�ֵ,���籣���ڱ��ݼĴ���
			BKP_WriteBackupRegister(BKP_DR1, MENU);
			
			MENU=21;//�Զ���ת���˵�21���������ò˵���ʾ����ֵ
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
			
			//���ݲ˵�ֵ,���籣���ڱ��ݼĴ���
			BKP_WriteBackupRegister(BKP_DR1, MENU);
			
			MENU=31;//�Զ���ת�� �˵�31 ��ʾ�ߵ��� ����
		}
		
		
		
		//�˵�11 ��ȡ�¶ȣ� ʱ�䣬��ʾ���˵�1
		if(MENU==11){//�˵��л�
			RTC_Get();//��ȡRTCʱ��
			LM75A_GetTemp(buffer);//��ȡ�¶�ֵ
			
			OLED_DISPLAY_8x16(6, 8*8, rhour/10+0x30);//��ʾСʱֵ
			OLED_DISPLAY_8x16(6, 9*8, rhour%10+0x30);
			OLED_DISPLAY_8x16(6, 10*8, ":");//��ʾð��
			OLED_DISPLAY_8x16(6, 11*8, rmin/10+0x30);//��ʾ����ֵ
			OLED_DISPLAY_8x16(6, 12*8, rmin%10+0x30);
			OLED_DISPLAY_8x16(6, 13*8, 0x3a);//��ʾð��
			OLED_DISPLAY_8x16(6, 14*8, rsec/10+0x30);//��ʾ����ֵ
			OLED_DISPLAY_8x16(6, 15*8, rsec%10+0x30);
			
			if(buffer[0]) OLED_DISPLAY_8x16(2, 9*8, 0x2d);//��ʾ���ţ� �洢���¶�ֵ����һ���洢������ֵ�������� Ϊ1ʱ���¶�Ϊ��
			else OLED_DISPLAY_8x16(2, 9*8, 0x20);//������ʾΪ�ո�
			
			OLED_DISPLAY_8x16(2, 10*8, buffer[1]/10+0x30);//�¶�ֵ
			OLED_DISPLAY_8x16(2, 11*8, buffer[1]%10+0x30);
			OLED_DISPLAY_8x16(2, 12*8, 0x2e);//С����
			OLED_DISPLAY_8x16(2, 13*8, buffer[2]/10+0x30);
			
			b=ENCODER_READ();//������ת������������ת��ֵ
			if(b==1) MENU=2;
			if(b==2) MENU=3;
		}
		
		
		//�˵�21 ��ʾģ��������ֵ
		if(MENU==21){
			b=ENCODER_READ();//������ת������������ת��ֵ
			if(b==1) MENU=3;
			if(b==2) MENU=1;
		
		}
		
		//�˵�31 ��ʾ�ߵ��� ����
		if(MENU==31){
			b=ENCODER_READ();//������ת������������ת��ֵ
			if(b==1) MENU=1;
			if(b==2) MENU=2;
		}
		
		
	}
}
