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
#include "adc.h"
#include "nvic.h"
#include "buzzer.h"
#include "dht11.h"
#include "led.h"

extern vu16 ADC_DMA_IN[3]; //�����ⲿ����
extern u8 INT_MARK; //�жϱ�־λ
vu16 arr1[10];//����10��ADCֵ����ƽ��ֵ
vu16 arr2[10];
vu16 arr3[10];


int main(void) {//������
	
	u8 buffer[3];//�����¶ȴ���������
	u8 MENU=1; //���˵����
	u8 SUBMENU3=0;//�˵�3���Ӳ˵�
	u8 a;//forѭ������
	u8 dht11_data[2];
	u16 Average_ADC1;//��1��ADCƽ��ֵ
	u16 Average_ADC2;//��2��ADCƽ��ֵ
	u16 Average_ADC3;//��3��ADCƽ��ֵ
	u16 max;//ADC ���ֵ
	u16 min;//ADC ��Сֵ
	int high_temp;//������ֵ
	int low_temp;//������ֵ
	u16 light_value;//����ֵ
	u8 humidity_value;//ʪ��ֵ
	
	
	delay_ms(100);//�ϵ�ʱ�ȴ�������������
	RCC_Configuration();//ϵͳʱ�ӳ�ʼ��
	I2C_Configuration();//I2C��ʼ��
	LM75A_GetTemp(buffer);//��ȡlm75a���¶�����
	ENCODER_Init();//��ת��������ʼ��
	OLED0561_Init();//OLED��Ļ��ʼ��
	RTC_Config();//��ʼ��ʱ��
	ENCODER_INT_INIT();//�������жϳ�ʼ��
	BKP_Configuration();//��ʼ��bkp
	ADC_Configuration(); //��ʼ��ADC
	NVIC_Configuration();//�����ж����ȼ�
	BUZZER_Init();//��������ʼ��
	LED_Init();
	OLED_DISPLAY_LIT(155);//������Ļ����
	delay_ms(1000);
	MENU=BKP_ReadBackupRegister(BKP_DR2);
	if(MENU<1||MENU>3)MENU=1; //���menu�ı���ֵ����1~3�Ĳ˵��б��Ĭ�����¸�ֵmenuΪ1
	
	
	high_temp=BKP_ReadBackupRegister(BKP_DR3);//�ӱ��üĴ����ж�ȡ���µ�����ֵ
	low_temp=BKP_ReadBackupRegister(BKP_DR4);//�ӱ��üĴ����ж�ȡ���µ�����ֵ
	light_value=BKP_ReadBackupRegister(BKP_DR5);//�ӱ��üĴ����ж�ȡ����ǿ�ȵ�����ֵ
	humidity_value=BKP_ReadBackupRegister(BKP_DR6);//�ӱ��üĴ����ж�ȡʪ�ȵ�����ֵ
	
	//�״ζ�ȡ�������ֵ�쳣�����¸�ֵ
	if(high_temp>99) high_temp=0;
	if(low_temp<0 || low_temp>99) low_temp=0;
	if(light_value>4096) light_value=0;
	if(humidity_value>99) humidity_value=15;
	
	
	/*�������*/
	OLED_DISPLAY_CLEAR(); //����
	INVERSE_OLED_DISPLAY_16x16(0, 0*16, 13);//���ֿո���ʾ����
	INVERSE_OLED_DISPLAY_16x16(0, 1*16, 13);//���ֿո���ʾ����
	INVERSE_OLED_DISPLAY_16x16(0, 2*16, 13);
	INVERSE_OLED_DISPLAY_16x16(0, 3*16, 25);//��
	INVERSE_OLED_DISPLAY_16x16(0, 4*16, 26);//��
	INVERSE_OLED_DISPLAY_16x16(0, 5*16, 13);//
	INVERSE_OLED_DISPLAY_16x16(0, 6*16, 13);//���ֿո���ʾ����
	INVERSE_OLED_DISPLAY_16x16(0, 7*16, 13);//���ֿո���ʾ����
	/*���dht11������*/
	if(!DHT11_Init()){//�ж�DHT11ͨ���Ƿ�����
		OLED_DISPLAY_8x16(2, 0*8, 'D');
		OLED_DISPLAY_8x16(2, 1*8, 'H');
		OLED_DISPLAY_8x16(2, 2*8, 'T');
		OLED_DISPLAY_8x16(2, 3*8, '1');
		OLED_DISPLAY_8x16(2, 4*8, '1');
		OLED_DISPLAY_8x16(2, 5*8, ':'); 
		OLED_DISPLAY_16x16(2, 3*16, 27);//��
		OLED_DISPLAY_16x16(2, 4*16, 28);//��
		OLED_DISPLAY_16x16(2, 5*16, 29);//��
		DHT11_ReadData(dht11_data);//��ȡ����
		delay_ms(2000);//��ʱ2S
	}else{//������DHT11ͨ���쳣
		OLED_DISPLAY_8x16(2, 0*8, 'D');
		OLED_DISPLAY_8x16(2, 1*8, 'H');
		OLED_DISPLAY_8x16(2, 2*8, 'T');
		OLED_DISPLAY_8x16(2, 3*8, '1');
		OLED_DISPLAY_8x16(2, 4*8, '1');
		OLED_DISPLAY_8x16(2, 5*8, ':'); 
		OLED_DISPLAY_16x16(2, 3*16, 31);//��
		OLED_DISPLAY_16x16(2, 4*16, 28);//��
		OLED_DISPLAY_16x16(2, 5*16, 30);//��
		while(DHT11_Check()){
			BUZZER_BEEP1();//����������
			delay_ms(500);//��ʱ
		}
	}
	
	
	
	while (1) {
		/*����ѭ������*/
		if (MENU>0){//���˵�
			switch(MENU){
				/*�˵�1  ��ʾ��ʪ�ȣ� ����ʱ��*/
				case 1:
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
					OLED_DISPLAY_8x16(4,11*8,0x25);//��ʾ�ٷֺ�
					OLED_DISPLAY_16x16(6,0*16,17);//������ʾ ����ʱ��	 
					OLED_DISPLAY_16x16(6,1*16,18);
					OLED_DISPLAY_16x16(6,2*16,19);	
					OLED_DISPLAY_16x16(6,3*16,20);
					
					
					BKP_WriteBackupRegister(BKP_DR2, MENU);//���ݲ˵�ֵ,���籣���ڱ��ݼĴ���
					
					MENU=11;//�Զ���ת���˵�11
					break;
				/*�˵�2 ģ�����˵�*/
				case 2:
					OLED_DISPLAY_CLEAR();//����
					INVERSE_OLED_DISPLAY_8x16(0,0*8,2+0x30); //���ַ���ʾ2
					INVERSE_OLED_DISPLAY_8x16(0,1*8,0x20); //��ʾ����
					INVERSE_OLED_DISPLAY_16x16(0,1*16,13);//��ʾ����
					INVERSE_OLED_DISPLAY_16x16(0,2*16,13);

					INVERSE_OLED_DISPLAY_16x16(0,2*16,10); //��ʾģ�����˵�
					INVERSE_OLED_DISPLAY_16x16(0,3*16,11);
					INVERSE_OLED_DISPLAY_16x16(0,4*16,12);
					INVERSE_OLED_DISPLAY_16x16(0,5*16,1);
					INVERSE_OLED_DISPLAY_16x16(0,6*16,2);
					INVERSE_OLED_DISPLAY_16x16(0,7*16,13);//��ʾ����

					/*��ʾ�ַ���ADC1*/
					OLED_DISPLAY_8x16(2, 0*8, 'A');
					OLED_DISPLAY_8x16(2, 1*8, 'D');
					OLED_DISPLAY_8x16(2, 2*8, 'C');
					OLED_DISPLAY_8x16(2, 3*8, '1');
					OLED_DISPLAY_8x16(2, 4*8, ':');
					/*��ʾ�ַ���ADC2*/
					OLED_DISPLAY_8x16(4, 0*8, 'A');
					OLED_DISPLAY_8x16(4, 1*8, 'D');
					OLED_DISPLAY_8x16(4, 2*8, 'C');
					OLED_DISPLAY_8x16(4, 3*8, '2');
					OLED_DISPLAY_8x16(4, 4*8, ':');
					/*��ʾ�ַ���ADC3*/
					OLED_DISPLAY_8x16(6, 0*8, 'A');
					OLED_DISPLAY_8x16(6, 1*8, 'D');
					OLED_DISPLAY_8x16(6, 2*8, 'C');
					OLED_DISPLAY_8x16(6, 3*8, '3');
					OLED_DISPLAY_8x16(6, 4*8, ':');
					BKP_WriteBackupRegister(BKP_DR2, MENU);//���ݲ˵�ֵ,���籣���ڱ��ݼĴ���
					
					MENU=21;//�Զ���ת���˵�21���������ò˵���ʾ����ֵ
					break;
				/*�˵�3  ���ò˵�*/
				case 3:
					OLED_DISPLAY_CLEAR();//����
					INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //���ַ���ʾ3
					INVERSE_OLED_DISPLAY_8x16(0,1*8,0x20); //��ʾ����
					INVERSE_OLED_DISPLAY_16x16(0,1*16,13);
					INVERSE_OLED_DISPLAY_16x16(0,2*16,6);//������ʾ	 ���ò˵�
					INVERSE_OLED_DISPLAY_16x16(0,3*16,7);
					INVERSE_OLED_DISPLAY_16x16(0,4*16,1);
					INVERSE_OLED_DISPLAY_16x16(0,5*16,2);
					INVERSE_OLED_DISPLAY_16x16(0,6*16,13); //��ʾ����
					INVERSE_OLED_DISPLAY_16x16(0,7*16,13);

					OLED_DISPLAY_16x16(2,0*16,21);//������ʾ	 ��������
					OLED_DISPLAY_16x16(2,1*16,3);
					OLED_DISPLAY_16x16(2,2*16,6);
					OLED_DISPLAY_16x16(2,3*16,7);
					
					OLED_DISPLAY_16x16(4,0*16,22);//������ʾ	 ��������
					OLED_DISPLAY_16x16(4,1*16,3);
					OLED_DISPLAY_16x16(4,2*16,6);
					OLED_DISPLAY_16x16(4,3*16,7);
					
					OLED_DISPLAY_16x16(6,0*16,8);//������ʾ	 ����
					OLED_DISPLAY_16x16(6,1*16,9);
					OLED_DISPLAY_16x16(6,2*16,6);
					OLED_DISPLAY_16x16(6,3*16,7);
					
					BKP_WriteBackupRegister(BKP_DR2, MENU);//���ݲ˵�ֵ,���籣���ڱ��ݼĴ���
					
					MENU=31;//�Զ���ת�� �˵�31 ��ʾ�ߵ��� ����
					break;
				/*�˵�4 �������ò˵�*/
				case 4:
					OLED_DISPLAY_CLEAR();//����
					INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //���ַ���ʾ3
					INVERSE_OLED_DISPLAY_8x16(0,1*8,0x2d); //�����ַ� -
					INVERSE_OLED_DISPLAY_8x16(0,2*8,1+0x30); //���ַ���ʾ1
					INVERSE_OLED_DISPLAY_8x16(0,3*8,0x20); //��ʾ����
					INVERSE_OLED_DISPLAY_16x16(0,2*16,13);
					INVERSE_OLED_DISPLAY_16x16(0,3*16,21);//������ʾ	 ��������
					INVERSE_OLED_DISPLAY_16x16(0,4*16,3);
					INVERSE_OLED_DISPLAY_16x16(0,5*16,6);
					INVERSE_OLED_DISPLAY_16x16(0,6*16,7);
					INVERSE_OLED_DISPLAY_16x16(0,7*16,13); //��ʾ����
					

					OLED_DISPLAY_16x16(2,0*16,21);//������ʾ	 ����
					OLED_DISPLAY_16x16(2,1*16,3);
					OLED_DISPLAY_8x16(2,4*8,10+0x30);//ð��
					INVERSE_OLED_DISPLAY_8x16(2,5*8,0x20);//�ո�
					INVERSE_OLED_DISPLAY_8x16(2, 6*8, high_temp/10%10+0x30);//����ʮλ
					INVERSE_OLED_DISPLAY_8x16(2, 7*8, high_temp%10+0x30);//���¸�λ
					INVERSE_OLED_DISPLAY_16x16(2, 4*16, 16);//���϶�
					
					OLED_DISPLAY_16x16(4,0*16,22);//������ʾ	 ����
					OLED_DISPLAY_16x16(4,1*16,3);
					OLED_DISPLAY_8x16(4,4*8,10+0x30);//ð��
					OLED_DISPLAY_8x16(4,5*8,0x20);//�ո�
					OLED_DISPLAY_8x16(4, 6*8, low_temp/10+0x30);//����ʮλ
					OLED_DISPLAY_8x16(4, 7*8, low_temp%10+0x30);//���¸�λ
					OLED_DISPLAY_16x16(4, 4*16, 16);//���϶�
					
					MENU=41;//��ת�������˵�
					break;
				/*�˵�5 �������ò˵�*/
				case 5:
					OLED_DISPLAY_CLEAR();//����
					INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //���ַ���ʾ3
					INVERSE_OLED_DISPLAY_8x16(0,1*8,0x2d); //�����ַ� -
					INVERSE_OLED_DISPLAY_8x16(0,2*8,2+0x30); //���ַ���ʾ2
					INVERSE_OLED_DISPLAY_8x16(0,3*8,0x20); //��ʾ����
					INVERSE_OLED_DISPLAY_16x16(0,2*16,13);
					INVERSE_OLED_DISPLAY_16x16(0,3*16,22);//������ʾ	 ��������
					INVERSE_OLED_DISPLAY_16x16(0,4*16,3);
					INVERSE_OLED_DISPLAY_16x16(0,5*16,6);
					INVERSE_OLED_DISPLAY_16x16(0,6*16,7);
					INVERSE_OLED_DISPLAY_16x16(0,7*16,13); //��ʾ����

					OLED_DISPLAY_16x16(2,0*16,21);//������ʾ	 ����
					OLED_DISPLAY_16x16(2,1*16,3);
					OLED_DISPLAY_8x16(2,4*8,10+0x30);//ð��
					OLED_DISPLAY_8x16(2,5*8,0x20);//�ո�
					OLED_DISPLAY_8x16(2, 6*8, high_temp/10%10+0x30);//����ʮλ
					OLED_DISPLAY_8x16(2, 7*8, high_temp%10+0x30);//���¸�λ
					OLED_DISPLAY_16x16(2, 4*16, 16);//���϶�
					OLED_DISPLAY_16x16(4,0*16,22);//������ʾ	 ����
					OLED_DISPLAY_16x16(4,1*16,3);
					OLED_DISPLAY_8x16(4,4*8,10+0x30);//ð��
					INVERSE_OLED_DISPLAY_8x16(4,5*8,0x20);//�ո�
					INVERSE_OLED_DISPLAY_8x16(4, 6*8, low_temp/10%10+0x30);//����ʮλ
					INVERSE_OLED_DISPLAY_8x16(4, 7*8, low_temp%10+0x30);//���¸�λ
					INVERSE_OLED_DISPLAY_16x16(4, 4*16, 16);//���϶�
					
					MENU=51;//������µ����˵�
					break;
				/*�˵�6 �������ò˵�*/
				case 6:
					OLED_DISPLAY_CLEAR();//����
					INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //���ַ���ʾ3
					INVERSE_OLED_DISPLAY_8x16(0,1*8,0x2d); //�����ַ� -
					INVERSE_OLED_DISPLAY_8x16(0,2*8,3+0x30); //���ַ���ʾ3
					INVERSE_OLED_DISPLAY_8x16(0,3*8,0x20); //��ʾ����
					INVERSE_OLED_DISPLAY_16x16(0,2*16,13);
					INVERSE_OLED_DISPLAY_16x16(0,3*16,8);//������ʾ	 ��������
					INVERSE_OLED_DISPLAY_16x16(0,4*16,9);
					INVERSE_OLED_DISPLAY_16x16(0,5*16,6);
					INVERSE_OLED_DISPLAY_16x16(0,6*16,7);
					INVERSE_OLED_DISPLAY_16x16(0,7*16,13); //��ʾ����

					OLED_DISPLAY_16x16(4,0*16,8);//������ʾ	 ����
					OLED_DISPLAY_16x16(4,1*16,9);
					OLED_DISPLAY_8x16(4,4*8,10+0x30);//ð��
					OLED_DISPLAY_8x16(4,5*8,0x20);//�ո�
					INVERSE_OLED_DISPLAY_8x16(4, 6*8, light_value/1000+0x30);//��ɫ��ʾ����ǿ������ֵ
					INVERSE_OLED_DISPLAY_8x16(4, 7*8, light_value/100%10+0x30);
					INVERSE_OLED_DISPLAY_8x16(4, 8*8, light_value/10%10+0x30);
					INVERSE_OLED_DISPLAY_8x16(4, 9*8, light_value%10+0x30);
					
					MENU=61; //������յ����˵�
					break;
					
				/*�˵�7 ʪ�����ò˵�*/
				case 7:
					OLED_DISPLAY_CLEAR();//����
					INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //���ַ���ʾ3
					INVERSE_OLED_DISPLAY_8x16(0,1*8,0x2d); //�����ַ� -
					INVERSE_OLED_DISPLAY_8x16(0,2*8,4+0x30); //���ַ���ʾ4
					INVERSE_OLED_DISPLAY_8x16(0,3*8,0x20); //��ʾ����
					INVERSE_OLED_DISPLAY_16x16(0,2*16,13);
					INVERSE_OLED_DISPLAY_16x16(0,3*16,5);//������ʾ	 ʪ������
					INVERSE_OLED_DISPLAY_16x16(0,4*16,4);
					INVERSE_OLED_DISPLAY_16x16(0,5*16,6);
					INVERSE_OLED_DISPLAY_16x16(0,6*16,7);
					INVERSE_OLED_DISPLAY_16x16(0,7*16,13); //��ʾ����
					
					OLED_DISPLAY_16x16(4,0*16,5);//������ʾ	 ʪ��
					OLED_DISPLAY_16x16(4,1*16,4);
					OLED_DISPLAY_8x16(4,4*8,10+0x30);//ð��
					OLED_DISPLAY_8x16(4,5*8,0x20);//�ո�
					INVERSE_OLED_DISPLAY_8x16(4, 6*8, humidity_value/10%10+0x30);//��ɫ��ʾʪ������ֵ
					INVERSE_OLED_DISPLAY_8x16(4, 7*8, humidity_value%10+0x30);
					INVERSE_OLED_DISPLAY_8x16(4, 8*8, 0x25);
					
					MENU=71;//����ʪ�ȵ����˵�
					break;
				/*�˵�11 ��ȡ�¶ȣ� ʱ�䣬��ʾ���˵�1*/
				case 11:
					RTC_Get();//��ȡRTCʱ��
					LM75A_GetTemp(buffer);//��ȡ�¶�ֵ
					OLED_DISPLAY_8x16(6, 8*8, rhour/10+0x30);//��ʾСʱֵ
					OLED_DISPLAY_8x16(6, 9*8, rhour%10+0x30);
					OLED_DISPLAY_8x16(6, 10*8, 0x3a);//��ʾð��
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
					
					/*ˢ��ʪ��*/
					OLED_DISPLAY_8x16(4,9*8,dht11_data[0]/10+0x30);
					OLED_DISPLAY_8x16(4,10*8,dht11_data[0]%10+0x30);
					
					break;
				/*�˵�21 ��ʾģ��������ֵ*/
				case 21:
					Average_ADC1=0;//��λ����ֵ
					Average_ADC2=0;
					Average_ADC3=0;
					for(a=0;a<10;a++){
						arr1[a]=ADC_DMA_IN[0]; //��ȡ��1·ADCֵ
						arr2[a]=ADC_DMA_IN[1]; //��ȡ��2·ADCֵ
						arr3[a]=ADC_DMA_IN[2]; //��ȡ��3·ADCֵ
						while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ж�ADCת��������ʶλ
						delay_us(2);//��ʱ1ms�ȴ�ADC������д��ADC1_DR�� ����ʱ1ms���������
						ADC_ClearFlag(ADC1, ADC_FLAG_EOC);//�����־λ
					}
					
					/*���һ·ƽ��ֵ*/
					max=arr1[0];
					min=arr1[0];
					for(a=0; a<10; a++){
						if(max<arr1[a]) max=arr1[a];//�������ֵ
						if(min>arr1[a])	min=arr1[a];//������Сֵ
					}
					for(a=0; a<10; a++){
						Average_ADC1=Average_ADC1+arr1[a];//���
					}
					Average_ADC1=(Average_ADC1-max-min)/8;//��ƽ��ֵ
					
					/*��ڶ�·ƽ��ֵ*/
					max=arr2[0];
					min=arr2[0];
					for(a=0; a<10; a++){
						if(max<arr2[a]) max=arr2[a];//�������ֵ
						if(min>arr2[a])	min=arr2[a];//������Сֵ
					}
					for(a=0; a<10; a++){
						Average_ADC2=Average_ADC2+arr2[a];//���
					}
					Average_ADC2=(Average_ADC2-max-min)/8;//��ƽ��ֵ
					
					/*�����·ƽ��ֵ*/
					max=arr3[0];
					min=arr3[0];
					for(a=0; a<10; a++){
						if(max<arr3[a]) max=arr3[a];//�������ֵ
						if(min>arr3[a])	min=arr3[a];//������Сֵ
					}
					for(a=0; a<10; a++){
						Average_ADC3=Average_ADC3+arr3[a];//���
					}
					Average_ADC3=(Average_ADC3-max-min)/8;//��ƽ��ֵ
					/*��ʾ��һ·ƽ��ֵ*/
					OLED_DISPLAY_8x16(2,5*8,Average_ADC1/1000+0x30);
					OLED_DISPLAY_8x16(2,6*8,Average_ADC1%1000/100+0x30);                         
					OLED_DISPLAY_8x16(2,7*8,Average_ADC1%100/10+0x30);
					OLED_DISPLAY_8x16(2,8*8,Average_ADC1%10+0x30);
					/*��ʾ�ڶ�·ƽ��ֵ*/
					OLED_DISPLAY_8x16(4,5*8,Average_ADC2/1000+0x30);
					OLED_DISPLAY_8x16(4,6*8,Average_ADC2%1000/100+0x30);                         
					OLED_DISPLAY_8x16(4,7*8,Average_ADC2%100/10+0x30);
					OLED_DISPLAY_8x16(4,8*8,Average_ADC2%10+0x30);
					/*��ʾ����·ƽ��ֵ*/
					OLED_DISPLAY_8x16(6,5*8,Average_ADC3/1000+0x30);
					OLED_DISPLAY_8x16(6,6*8,Average_ADC3%1000/100+0x30);                         
					OLED_DISPLAY_8x16(6,7*8,Average_ADC3%100/10+0x30);
					OLED_DISPLAY_8x16(6,8*8,Average_ADC3%10+0x30);
					break;
				/*�˵�31 ���ò˵�*/
				case 31:
					delay_ms(10);
					break;
				/*�˵�31 ���ò˵�*/
				case 41:
					delay_ms(20);
					break;
				/*�˵�51 ���ò˵�*/
				case 51:
					delay_ms(20);
					break;
				/*�˵�61 ���ò˵�*/
				case 61:
					delay_ms(10);
					break;
				/*�˵�31 ���ò˵�*/
				case 71:
					delay_ms(10);
					break;
				default:
					MENU=1;//��ת����ʼ�˵�
					break;
			}
	
			
			/*���˵���ť�л�*/
			if(INT_MARK!=0 && MENU>=11 && MENU<=71){
				/*���˵�����ѭ���л�*/
				switch(MENU){
					case 11:
						if(INT_MARK==1){//��ת
							MENU=2;
						}else if(INT_MARK==2){//��ת
							MENU=3;
						}
						break;
					case 21:
						if(INT_MARK==1){//��ת
							MENU=3;
						}else if(INT_MARK==2){//��ת
							MENU=1;
						}
						break;
					case 31:
						if(INT_MARK==1){//��ת
							MENU=1;
						}else if(INT_MARK==2){//��ת
							MENU=2;
						}else if(INT_MARK==3){
							SUBMENU3=1;//�˵�3�Ӳ˵�ѡ��
							MENU=0;//���������˵�ҳ��
							BUZZER_BEEP3();//ȷ����ʾ��
						}
						break;
					/*�˵�41 ����������ֵ����*/
					case 41:
						if(INT_MARK==1){//��ת
							high_temp++;
							if(high_temp>99) high_temp=low_temp;
						}else if(INT_MARK==2){//��ת
							high_temp--;
							if(high_temp<low_temp) high_temp=99;
      				
						}else if(INT_MARK==3){//ȷ��
							MENU=0;
							SUBMENU3=1;
							BKP_WriteBackupRegister(BKP_DR3, high_temp);//д�뱸�üĴ�����
							BUZZER_BEEP4();//�˳���Ч
						}
						/*��ʾ���µ�����ֵ*/
						INVERSE_OLED_DISPLAY_8x16(2,5*8,0x20);//�ո�
						INVERSE_OLED_DISPLAY_8x16(2, 6*8, high_temp/10%10+0x30);//����ʮλ
						INVERSE_OLED_DISPLAY_8x16(2, 7*8, high_temp%10+0x30);//���¸�λ
						INVERSE_OLED_DISPLAY_16x16(2, 4*16, 16);//���϶�
						break;
					/*�˵�51 ����������ֵ����*/
					case 51:
						if(INT_MARK==1){//��ת
							low_temp++;
							if(low_temp>high_temp) low_temp=0;//�¶ȸ��ڸ���
						}else if(INT_MARK==2){//��ת
							low_temp--;
							if(low_temp<0) low_temp=high_temp;
						}else if(INT_MARK==3){//ȷ��
							MENU=0;
							SUBMENU3=2;
							BKP_WriteBackupRegister(BKP_DR4, low_temp);//д�뱸�üĴ�����
							BUZZER_BEEP4();//�˳���Ч
						}
						/*��ʾ���µ�����ֵ*/
						INVERSE_OLED_DISPLAY_8x16(4,5*8,0x20);//�ո�
						INVERSE_OLED_DISPLAY_8x16(4, 6*8, low_temp/10%10+0x30);//����ʮλ
						INVERSE_OLED_DISPLAY_8x16(4, 7*8, low_temp%10+0x30);//���¸�λ
						INVERSE_OLED_DISPLAY_16x16(4, 4*16, 16);//���϶�
						break;
					/*�˵�61 ����ǿ��������ֵ����*/
					case 61:
						if(INT_MARK==1){//��ת
							light_value++;
							if(light_value>4095) light_value=0;
						}else if(INT_MARK==2){//��ת
							if(light_value==0) light_value=4096;
							light_value--;
						}else if(INT_MARK==3){//ȷ��
							MENU=0;
							SUBMENU3=3;
							BKP_WriteBackupRegister(BKP_DR5, light_value);//д�뱸�üĴ�����
							BUZZER_BEEP4();//�˳���Ч        
						}
						/*��ʾ����ǿ�ȵ�����ֵ*/
						INVERSE_OLED_DISPLAY_8x16(4, 6*8, light_value/1000+0x30);//��ɫ��ʾ����ǿ������ֵ
						INVERSE_OLED_DISPLAY_8x16(4, 7*8, light_value/100%10+0x30);
						INVERSE_OLED_DISPLAY_8x16(4, 8*8, light_value/10%10+0x30);
						INVERSE_OLED_DISPLAY_8x16(4, 9*8, light_value%10+0x30);
						break;
					/*�˵�71 ʪ��������ֵ����*/
					case 71:
						if(INT_MARK==1){//��ת
							humidity_value++;
							if(humidity_value>90 || humidity_value<15) humidity_value=15;
						}else if(INT_MARK==2){//��ת
							if(humidity_value<=15) humidity_value=91;
							humidity_value--;
						}else if(INT_MARK==3){//ȷ��
							MENU=0;
							SUBMENU3=3;
							BKP_WriteBackupRegister(BKP_DR6, humidity_value);//д�뱸�üĴ�����
							BUZZER_BEEP4();//�˳���Ч
						}
						//*��ʾʪ�ȵ�����ֵ*/
						INVERSE_OLED_DISPLAY_8x16(4, 6*8, humidity_value/10%10+0x30);//��ɫ��ʾʪ������ֵ
						INVERSE_OLED_DISPLAY_8x16(4, 7*8, humidity_value%10+0x30);
						INVERSE_OLED_DISPLAY_8x16(4, 8*8, 0x25);
						break;
					default:
						MENU=1;
						break;
				}
				
				INT_MARK=0;//��־λ����
			}
		}else{//����˵�3�Ӳ˵�ѡ��
			
			switch(SUBMENU3){
				/*�˵�3�Ӳ˵��� ��������*/
				case 1:
					OLED_DISPLAY_CLEAR();//����
					INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //���ַ���ʾ3
					INVERSE_OLED_DISPLAY_8x16(0,1*8,0x20); //��ʾ����
					INVERSE_OLED_DISPLAY_16x16(0,1*16,13);
					INVERSE_OLED_DISPLAY_16x16(0,2*16,6);//������ʾ	 ���ò˵�
					INVERSE_OLED_DISPLAY_16x16(0,3*16,7);
					INVERSE_OLED_DISPLAY_16x16(0,4*16,1);
					INVERSE_OLED_DISPLAY_16x16(0,5*16,2);
					INVERSE_OLED_DISPLAY_16x16(0,6*16,13); //��ʾ����
					INVERSE_OLED_DISPLAY_16x16(0,7*16,13);
					
					INVERSE_OLED_DISPLAY_16x16(2,0*16,21); //���Ը�������
					INVERSE_OLED_DISPLAY_16x16(2,1*16,3);
					INVERSE_OLED_DISPLAY_16x16(2,2*16,6); 
					INVERSE_OLED_DISPLAY_16x16(2,3*16,7);
					INVERSE_OLED_DISPLAY_16x16(2,4*16,13); //��ʾ����
					INVERSE_OLED_DISPLAY_16x16(2,5*16,13);
					INVERSE_OLED_DISPLAY_16x16(2,6*16,13); //��ʾ����
					INVERSE_OLED_DISPLAY_16x16(2,7*16,13);
					
					OLED_DISPLAY_16x16(4,0*16,22);//������ʾ	 ��������
					OLED_DISPLAY_16x16(4,1*16,3);
					OLED_DISPLAY_16x16(4,2*16,6);
					OLED_DISPLAY_16x16(4,3*16,7);
					OLED_DISPLAY_16x16(4,4*16,13);//��ʾ�ո�
					OLED_DISPLAY_16x16(4,5*16,13);
					OLED_DISPLAY_16x16(4,6*16,13); 
					OLED_DISPLAY_16x16(4,7*16,13);
					
					OLED_DISPLAY_16x16(6,0*16,8);//������ʾ	 ��������
					OLED_DISPLAY_16x16(6,1*16,9);
					OLED_DISPLAY_16x16(6,2*16,6);
					OLED_DISPLAY_16x16(6,3*16,7);
					OLED_DISPLAY_16x16(6,4*16,13);//��ʾ�ո�
					OLED_DISPLAY_16x16(6,5*16,13);
					OLED_DISPLAY_16x16(6,6*16,13); 
					OLED_DISPLAY_16x16(6,7*16,13);
					SUBMENU3=11;//��ת��11�˵���������Ļһֱˢ�£����ϵͳ����
					break;
				/*�˵�3�Ӳ˵��� ��������*/
				case 2:
					OLED_DISPLAY_CLEAR();//����
					INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //���ַ���ʾ3
					INVERSE_OLED_DISPLAY_8x16(0,1*8,0x20); //��ʾ����
					INVERSE_OLED_DISPLAY_16x16(0,1*16,13);
					INVERSE_OLED_DISPLAY_16x16(0,2*16,6);//������ʾ	 ���ò˵�
					INVERSE_OLED_DISPLAY_16x16(0,3*16,7);
					INVERSE_OLED_DISPLAY_16x16(0,4*16,1);
					INVERSE_OLED_DISPLAY_16x16(0,5*16,2);
					INVERSE_OLED_DISPLAY_16x16(0,6*16,13); //��ʾ����
					INVERSE_OLED_DISPLAY_16x16(0,7*16,13);
					
					OLED_DISPLAY_16x16(2,0*16,21);//������ʾ	 ��������
					OLED_DISPLAY_16x16(2,1*16,3);
					OLED_DISPLAY_16x16(2,2*16,6);
					OLED_DISPLAY_16x16(2,3*16,7);
					OLED_DISPLAY_16x16(2,4*16,13);//��ʾ�ո�
					OLED_DISPLAY_16x16(2,5*16,13);
					OLED_DISPLAY_16x16(2,6*16,13); 
					OLED_DISPLAY_16x16(2,7*16,13);
					
					INVERSE_OLED_DISPLAY_16x16(4,0*16,22); //���Ե�������
					INVERSE_OLED_DISPLAY_16x16(4,1*16,3);
					INVERSE_OLED_DISPLAY_16x16(4,2*16,6); 
					INVERSE_OLED_DISPLAY_16x16(4,3*16,7);
					INVERSE_OLED_DISPLAY_16x16(4,4*16,13); //��ʾ����
					INVERSE_OLED_DISPLAY_16x16(4,5*16,13);
					INVERSE_OLED_DISPLAY_16x16(4,6*16,13); //��ʾ����
					INVERSE_OLED_DISPLAY_16x16(4,7*16,13);
					
					OLED_DISPLAY_16x16(6,0*16,8);//������ʾ	 ��������
					OLED_DISPLAY_16x16(6,1*16,9);
					OLED_DISPLAY_16x16(6,2*16,6);
					OLED_DISPLAY_16x16(6,3*16,7);
					OLED_DISPLAY_16x16(6,4*16,13);
					OLED_DISPLAY_16x16(6,5*16,13);
					OLED_DISPLAY_16x16(6,6*16,13); 
					OLED_DISPLAY_16x16(6,7*16,13);
					
					SUBMENU3=21;
					break;
				
				/*�˵�3�Ӳ˵��� ��������*/
				case 3:
					OLED_DISPLAY_CLEAR();//����
					INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //���ַ���ʾ3
					INVERSE_OLED_DISPLAY_8x16(0,1*8,0x20); //��ʾ����
					INVERSE_OLED_DISPLAY_16x16(0,1*16,13);
					INVERSE_OLED_DISPLAY_16x16(0,2*16,6);//������ʾ	 ���ò˵�
					INVERSE_OLED_DISPLAY_16x16(0,3*16,7);
					INVERSE_OLED_DISPLAY_16x16(0,4*16,1);
					INVERSE_OLED_DISPLAY_16x16(0,5*16,2);
					INVERSE_OLED_DISPLAY_16x16(0,6*16,13); //��ʾ����
					INVERSE_OLED_DISPLAY_16x16(0,7*16,13);
					
					OLED_DISPLAY_16x16(2,0*16,21);//������ʾ	 ��������
					OLED_DISPLAY_16x16(2,1*16,3);
					OLED_DISPLAY_16x16(2,2*16,6);
					OLED_DISPLAY_16x16(2,3*16,7);
					OLED_DISPLAY_16x16(2,4*16,13);//��ʾ�ո�
					OLED_DISPLAY_16x16(2,5*16,13);
					OLED_DISPLAY_16x16(2,6*16,13);//��ʾ�ո�
					OLED_DISPLAY_16x16(2,7*16,13);
					
					OLED_DISPLAY_16x16(4,0*16,22);//������ʾ	 ��������
					OLED_DISPLAY_16x16(4,1*16,3);
					OLED_DISPLAY_16x16(4,2*16,6);
					OLED_DISPLAY_16x16(4,3*16,7);
					OLED_DISPLAY_16x16(4,4*16,13);//��ʾ�ո�
					OLED_DISPLAY_16x16(4,5*16,13);
					OLED_DISPLAY_16x16(4,6*16,13); //��ʾ�ո�
					OLED_DISPLAY_16x16(4,7*16,13);
					
					INVERSE_OLED_DISPLAY_16x16(6,0*16,8); //���Թ�������
					INVERSE_OLED_DISPLAY_16x16(6,1*16,9);
					INVERSE_OLED_DISPLAY_16x16(6,2*16,6); 
					INVERSE_OLED_DISPLAY_16x16(6,3*16,7);
					INVERSE_OLED_DISPLAY_16x16(6,4*16,13); //��ʾ����
					INVERSE_OLED_DISPLAY_16x16(6,5*16,13);
					INVERSE_OLED_DISPLAY_16x16(6,6*16,13); //��ʾ����
					INVERSE_OLED_DISPLAY_16x16(6,7*16,13);
					
					SUBMENU3=31;
					break;
				
				/*�˵�3�Ӳ˵��� ʪ������*/
				case 4:
					OLED_DISPLAY_CLEAR();//����
					INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //���ַ���ʾ3
					INVERSE_OLED_DISPLAY_8x16(0,1*8,0x20); //��ʾ����
					INVERSE_OLED_DISPLAY_16x16(0,1*16,13);
					INVERSE_OLED_DISPLAY_16x16(0,2*16,6);//������ʾ	 ���ò˵�
					INVERSE_OLED_DISPLAY_16x16(0,3*16,7);
					INVERSE_OLED_DISPLAY_16x16(0,4*16,1);
					INVERSE_OLED_DISPLAY_16x16(0,5*16,2);
					INVERSE_OLED_DISPLAY_16x16(0,6*16,13); //��ʾ����
					INVERSE_OLED_DISPLAY_16x16(0,7*16,13);
					
					OLED_DISPLAY_16x16(2,0*16,22);//������ʾ	 ��������
					OLED_DISPLAY_16x16(2,1*16,3);
					OLED_DISPLAY_16x16(2,2*16,6);
					OLED_DISPLAY_16x16(2,3*16,7);
					OLED_DISPLAY_16x16(2,4*16,13);
					OLED_DISPLAY_16x16(2,5*16,13);
					OLED_DISPLAY_16x16(2,6*16,13); 
					OLED_DISPLAY_16x16(2,7*16,13);
					
					OLED_DISPLAY_16x16(4,0*16,8);//������ʾ	 ��������
					OLED_DISPLAY_16x16(4,1*16,9);
					OLED_DISPLAY_16x16(4,2*16,6);
					OLED_DISPLAY_16x16(4,3*16,7);
					OLED_DISPLAY_16x16(4,4*16,13);//��ʾ�ո�
					OLED_DISPLAY_16x16(4,5*16,13);
					OLED_DISPLAY_16x16(4,6*16,13); 
					OLED_DISPLAY_16x16(4,7*16,13);
					
					INVERSE_OLED_DISPLAY_16x16(6,0*16,5);//������ʾ	 ʪ������
					INVERSE_OLED_DISPLAY_16x16(6,1*16,4);
					INVERSE_OLED_DISPLAY_16x16(6,2*16,6);
					INVERSE_OLED_DISPLAY_16x16(6,3*16,7);
					INVERSE_OLED_DISPLAY_16x16(6,4*16,13); //��ʾ����
					INVERSE_OLED_DISPLAY_16x16(6,5*16,13);
					INVERSE_OLED_DISPLAY_16x16(6,6*16,13); //��ʾ����
					INVERSE_OLED_DISPLAY_16x16(6,7*16,13);
					SUBMENU3=41;
					break;
				
				/*�˵�3�Ӳ˵��� ʪ�����÷���*/
				case 5:
					OLED_DISPLAY_CLEAR();//����
				
					INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //���ַ���ʾ3
					INVERSE_OLED_DISPLAY_8x16(0,1*8,0x20); //��ʾ����
					INVERSE_OLED_DISPLAY_16x16(0,1*16,13);
					INVERSE_OLED_DISPLAY_16x16(0,2*16,6);//������ʾ	 ���ò˵�
					INVERSE_OLED_DISPLAY_16x16(0,3*16,7);
					INVERSE_OLED_DISPLAY_16x16(0,4*16,1);
					INVERSE_OLED_DISPLAY_16x16(0,5*16,2);
					INVERSE_OLED_DISPLAY_16x16(0,6*16,13); //��ʾ����
					INVERSE_OLED_DISPLAY_16x16(0,7*16,13);
					
					OLED_DISPLAY_16x16(2,0*16,8);//������ʾ	 ��������
					OLED_DISPLAY_16x16(2,1*16,9);
					OLED_DISPLAY_16x16(2,2*16,6);
					OLED_DISPLAY_16x16(2,3*16,7);
					OLED_DISPLAY_16x16(2,4*16,13);//��ʾ�ո�
					OLED_DISPLAY_16x16(2,5*16,13);
					OLED_DISPLAY_16x16(2,6*16,13); 
					OLED_DISPLAY_16x16(2,7*16,13);
					
					OLED_DISPLAY_16x16(4,0*16,5);//������ʾ	 ʪ������
					OLED_DISPLAY_16x16(4,1*16,4);
					OLED_DISPLAY_16x16(4,2*16,6);
					OLED_DISPLAY_16x16(4,3*16,7);
					OLED_DISPLAY_16x16(4,4*16,13); //��ʾ����
					OLED_DISPLAY_16x16(4,5*16,13);
					OLED_DISPLAY_16x16(4,6*16,13); //��ʾ����
					OLED_DISPLAY_16x16(4,7*16,13);
					
					INVERSE_OLED_DISPLAY_16x16(6,0*16,23);//������ʾ	 �˳�����
					INVERSE_OLED_DISPLAY_16x16(6,1*16,24);
					INVERSE_OLED_DISPLAY_16x16(6,2*16,6);
					INVERSE_OLED_DISPLAY_16x16(6,3*16,7);
					INVERSE_OLED_DISPLAY_16x16(6,4*16,13); //��ʾ����
					INVERSE_OLED_DISPLAY_16x16(6,5*16,13);
					INVERSE_OLED_DISPLAY_16x16(6,6*16,13); //��ʾ����
					INVERSE_OLED_DISPLAY_16x16(6,7*16,13);
					
					SUBMENU3=51;
					break;
				
				case 11:
					delay_ms(50);
					break;
				
				case 21:
					delay_ms(50);
					break;
				
				case 31:
					delay_ms(50);
					break;
				
				case 41:
					delay_ms(50);
					break;
				
				case 51:
					delay_ms(50);
					break;
				
				default:
					SUBMENU3=1;//Ĭ�ϻ��˵�����������ʾ
					break;
			}

			
			/*�˵�3�Ӳ˵���ť�л�*/
			if(INT_MARK!=0){
				/*���˵�����ѭ���л�*/
				switch(SUBMENU3){
					case 11:
						if(INT_MARK==1){//��ת
							SUBMENU3=2;
						}else if(INT_MARK==2){//��ת
							SUBMENU3=5;
						}else if(INT_MARK==3){
							SUBMENU3=0;
							MENU=4;
						}
						break;
					case 21:
						if(INT_MARK==1){//��ת
							SUBMENU3=3;
						}else if(INT_MARK==2){//��ת
							SUBMENU3=1;
						}else if(INT_MARK==3){
							SUBMENU3=0;
							MENU=5;
						}
						break;
					case 31:
						if(INT_MARK==1){//��ת
							SUBMENU3=4;
						}else if(INT_MARK==2){//��ת
							SUBMENU3=2;
						}else if(INT_MARK==3){
							SUBMENU3=0;
							MENU=6;
						}
						break;	
					case 41:
						if(INT_MARK==1){//��ת
							SUBMENU3=5;
						}else if(INT_MARK==2){//��ת
							SUBMENU3=3;
						}else if(INT_MARK==3){
							SUBMENU3=0;
							MENU=7;
						}
						break;
					case 51:
						if(INT_MARK==1){//��ת
							SUBMENU3=1;
						}else if(INT_MARK==2){//��ת
							SUBMENU3=4;
						}else if(INT_MARK==3){
							SUBMENU3=0;
							MENU=3;
						}
						break;
					default:
						MENU=3;
						SUBMENU3=0;
						break;
				}
				
				INT_MARK=0;//��־λ����
			}
		}
	}
}













