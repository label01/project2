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
	u8 high_temp=0;//������ֵ
	u8 low_temp=0;//������ֵ
	u16 light_value=0;//����ֵ
	u8 humidity_value=0;//ʪ��ֵ
	
	
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
	BUZZER_Init();
	OLED_DISPLAY_LIT(155);//������Ļ����
	delay_ms(1000);
	MENU=BKP_ReadBackupRegister(BKP_DR1);
	if(MENU<1||MENU>3)MENU=1; //���menu�ı���ֵ����1~3�Ĳ˵��б��Ĭ�����¸�ֵmenuΪ1
	
	
//	high_temp=BKP_ReadBackupRegister(BKP_DR2);//�ӱ��üĴ����ж�ȡ���µ�����ֵ
//	low_temp=BKP_ReadBackupRegister(BKP_DR3);//�ӱ��üĴ����ж�ȡ���µ�����ֵ
//	light_value=BKP_ReadBackupRegister(BKP_DR4);//�ӱ��üĴ����ж�ȡ����ǿ�ȵ�����ֵ
//	humidity_value=BKP_ReadBackupRegister(BKP_DR5);//�ӱ��üĴ����ж�ȡʪ�ȵ�����ֵ
//	
//	//�״ζ�ȡ�������ֵ�쳣�����¸�ֵ
//	if(high_temp<0 || high_temp>99) high_temp=0;
//	if(low_temp<0 || low_temp>99) low_temp=0;
//	if(light_value<0 || light_value>4096) light_value=0;
//	if(humidity_value<0 || humidity_value>99) humidity_value=0;
	
	
	while (1) {
		/*����ѭ������*/
		
		/*�˵�1  ��ʾ��ʪ�ȣ� ����ʱ��*/
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
			OLED_DISPLAY_8x16(4,11*8,0x25);//��ʾ�ٷֺ�
			OLED_DISPLAY_16x16(6,0*16,17);//������ʾ ����ʱ��	 
			OLED_DISPLAY_16x16(6,1*16,18);
			OLED_DISPLAY_16x16(6,2*16,19);	
			OLED_DISPLAY_16x16(6,3*16,20);
			
			
			BKP_WriteBackupRegister(BKP_DR1, MENU);//���ݲ˵�ֵ,���籣���ڱ��ݼĴ���
			
			MENU=11;//�Զ���ת���˵�11
		}
		
		/*�˵�2 ģ�����˵�*/
		if(MENU==2){
			
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

			OLED_DISPLAY_8x16_BUFFER(2,"ADC1:"); //��ʾ�ַ���
			OLED_DISPLAY_8x16_BUFFER(4,"ADC2:"); //��ʾ�ַ���
			OLED_DISPLAY_8x16_BUFFER(6,"ADC3: "); //��ʾ�ַ���
			
			
			BKP_WriteBackupRegister(BKP_DR1, MENU);//���ݲ˵�ֵ,���籣���ڱ��ݼĴ���
			
			MENU=21;//�Զ���ת���˵�21���������ò˵���ʾ����ֵ
		}
		
		/*�˵�3  ���ò˵�*/
		if(MENU==3){
			
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
			
			BKP_WriteBackupRegister(BKP_DR1, MENU);//���ݲ˵�ֵ,���籣���ڱ��ݼĴ���
			
			MENU=31;//�Զ���ת�� �˵�31 ��ʾ�ߵ��� ����
		}
		
		/*�˵�4 �������ò˵�*/
		if(MENU==4){
			
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
			INVERSE_OLED_DISPLAY_8x16(2, 6*8, high_temp/10+0x30);//����ʮλ
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
		}
		
		/*�˵�5 �������ò˵�*/
		if(MENU==5){
			
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
			OLED_DISPLAY_8x16(2, 6*8, high_temp/10+0x30);//����ʮλ
			OLED_DISPLAY_8x16(2, 7*8, high_temp%10+0x30);//���¸�λ
			OLED_DISPLAY_16x16(2, 4*16, 16);//���϶�
			OLED_DISPLAY_16x16(4,0*16,22);//������ʾ	 ����
			OLED_DISPLAY_16x16(4,1*16,3);
			OLED_DISPLAY_8x16(4,4*8,10+0x30);//ð��
			INVERSE_OLED_DISPLAY_8x16(4,5*8,0x20);//�ո�
			INVERSE_OLED_DISPLAY_8x16(4, 6*8, low_temp/10+0x30);//����ʮλ
			INVERSE_OLED_DISPLAY_8x16(4, 7*8, low_temp%10+0x30);//���¸�λ
			INVERSE_OLED_DISPLAY_16x16(4, 4*16, 16);//���϶�
			
			MENU=51;//������µ����˵�
			
		}
		
		/*�˵�6 �������ò˵�*/
		if(MENU==6){
			
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
		}
		
		/*�˵�7 ʪ�����ò˵�*/
		if(MENU==7){
			
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
			INVERSE_OLED_DISPLAY_8x16(4, 6*8, humidity_value/10+0x30);//��ɫ��ʾʪ������ֵ
			INVERSE_OLED_DISPLAY_8x16(4, 7*8, humidity_value%10+0x30);
			INVERSE_OLED_DISPLAY_8x16(4, 8*8, 0x25);
			
			MENU=71;//����ʪ�ȵ����˵�
			
		}
		
		
		/*�˵�11 ��ȡ�¶ȣ� ʱ�䣬��ʾ���˵�1*/
		if(MENU==11){//�˵��л�
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
			
			/*��ȡʪ��ֵ*/
			OLED_DISPLAY_8x16(4,9*8,2+0x30);
			OLED_DISPLAY_8x16(4,10*8,5+0x30);
			
			/*ˢ��ʪ��*/
			
		}
		
		
		/*�˵�21 ��ʾģ��������ֵ*/
		if(MENU==21){
			Average_ADC1=0;//��λ����ֵ
			Average_ADC2=0;
			Average_ADC3=0;
			
			for(a=0;a<10;a++){
				arr1[a]=ADC_DMA_IN[0]; //��ȡ��1·ADCֵ
				arr2[a]=ADC_DMA_IN[1]; //��ȡ��2·ADCֵ
				arr3[a]=ADC_DMA_IN[2]; //��ȡ��3·ADCֵ
				while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ж�ADCת��������ʶλ
				delay_us(1);//��ʱ1ms�ȴ�ADC������д��ADC1_DR�� ����ʱ1ms���������
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
		}
		
		/*�˵�31 ���ò˵�*/
		if(MENU==31){
			delay_ms(50);
		}
		
		/*���˵���ť�л�*/
		if(INT_MARK!=0 && MENU>=11 && MENU<=31){
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
						INT_MARK=0;//��־λ����
						SUBMENU3=1;//�˵�3�Ӳ˵�ѡ��
						MENU=0;//���������˵�ҳ��
						BUZZER_BEEP3();//ȷ����ʾ��
					}
					break;
				default:
					MENU=1;
					break;
			}
			
			INT_MARK=0;//��־λ����
		}
		
		
		
//		/*�˵�3�Ӳ˵��ж��л�*/
//		if(INT_MARK!=0 && MENU>=32 && MENU<=35){
//			/*��������*/
//			if(MENU==32 && INT_MARK==1) 
//			{
//				MENU=33;//��ת�������������
//				INT_MARK=0;//��־λ����
//				OLED_DISPLAY_CLEAR();//����
//				//BUZZER_BEEP1();//��ʾ��
//			}
//			if(MENU==32 && INT_MARK==2) 
//			{
//				MENU=35;//��ת��ʪ������
//				INT_MARK=0;//��־λ����
//				OLED_DISPLAY_CLEAR();//����
//				//BUZZER_BEEP1();//��ʾ��
//			}
//			/*��������*/
//			if(MENU==33 && INT_MARK==1) 
//			{
//				MENU=34;//��ת����������
//				INT_MARK=0;//��־λ����
//				OLED_DISPLAY_CLEAR();//����
//				//BUZZER_BEEP1();//��ʾ��
//			}
//			if(MENU==33 && INT_MARK==2)
//			{
//				MENU=32;//��ת����������
//				INT_MARK=0;//��־λ����
//				OLED_DISPLAY_CLEAR();//����
//				//BUZZER_BEEP1();//��ʾ��
//			}
//			/*��������*/
//			if(MENU==34 && INT_MARK==1)
//			{
//				MENU=35;//��ת��ʪ������
//				INT_MARK=0;//��־λ����
//				OLED_DISPLAY_CLEAR();//����
//				//BUZZER_BEEP1();//��ʾ��
//			}
//			if(MENU==34 && INT_MARK==2) 
//			{
//				MENU=33;//��ת����������
//				INT_MARK=0;//��־λ����
//				OLED_DISPLAY_CLEAR();//����
//				//BUZZER_BEEP1();//��ʾ��
//			}	
//			/*ʪ������*/
//			if(MENU==35 && INT_MARK==1) 
//			{
//				MENU=32;//��ת����������
//				INT_MARK=0;//��־λ����
//				OLED_DISPLAY_CLEAR();//����
//				//BUZZER_BEEP1();//��ʾ��
//			}	
//			if(MENU==35 && INT_MARK==2) 
//			{
//				MENU=34;//��ת����������
//				INT_MARK=0;//��־λ����
//				OLED_DISPLAY_CLEAR();//����
//				//BUZZER_BEEP1();//��ʾ��
//			}
//			/*��ť���£�����˵�3�Ӳ˵�����*/
//			if(INT_MARK==3)
//			{
//				MENU=MENU%10+2;
//				INT_MARK=0;
//				BUZZER_BEEP3();//ȷ����ʾ��
//			}
//		}
		
		
		/*�˵�32 �˵�3�Ӳ˵��� �������÷���*/
//		if(MENU==100){
//			delay_ms(100);
//			INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //���ַ���ʾ3
//			INVERSE_OLED_DISPLAY_8x16(0,1*8,0x20); //��ʾ����
//			INVERSE_OLED_DISPLAY_16x16(0,1*16,13);
//			INVERSE_OLED_DISPLAY_16x16(0,2*16,6);//������ʾ	 ���ò˵�
//			INVERSE_OLED_DISPLAY_16x16(0,3*16,7);
//			INVERSE_OLED_DISPLAY_16x16(0,4*16,1);
//			INVERSE_OLED_DISPLAY_16x16(0,5*16,2);
//			INVERSE_OLED_DISPLAY_16x16(0,6*16,13); //��ʾ����
//			INVERSE_OLED_DISPLAY_16x16(0,7*16,13);
//			
//			INVERSE_OLED_DISPLAY_16x16(2,0*16,21); //���Ը�������
//			INVERSE_OLED_DISPLAY_16x16(2,1*16,3);
//			INVERSE_OLED_DISPLAY_16x16(2,2*16,6); 
//			INVERSE_OLED_DISPLAY_16x16(2,3*16,7);
//			INVERSE_OLED_DISPLAY_16x16(2,4*16,13); //��ʾ����
//			INVERSE_OLED_DISPLAY_16x16(2,5*16,13);
//			INVERSE_OLED_DISPLAY_16x16(2,6*16,13); //��ʾ����
//			INVERSE_OLED_DISPLAY_16x16(2,7*16,13);
//			
//			OLED_DISPLAY_16x16(4,0*16,22);//������ʾ	 ��������
//			OLED_DISPLAY_16x16(4,1*16,3);
//			OLED_DISPLAY_16x16(4,2*16,6);
//			OLED_DISPLAY_16x16(4,3*16,7);
//			OLED_DISPLAY_16x16(4,4*16,13);//��ʾ�ո�
//			OLED_DISPLAY_16x16(4,5*16,13);
//			OLED_DISPLAY_16x16(4,6*16,13); 
//			OLED_DISPLAY_16x16(4,7*16,13);
//			
//			OLED_DISPLAY_16x16(6,0*16,8);//������ʾ	 ��������
//			OLED_DISPLAY_16x16(6,1*16,9);
//			OLED_DISPLAY_16x16(6,2*16,6);
//			OLED_DISPLAY_16x16(6,3*16,7);
//			OLED_DISPLAY_16x16(6,4*16,13);//��ʾ�ո�
//			OLED_DISPLAY_16x16(6,5*16,13);
//			OLED_DISPLAY_16x16(6,6*16,13); 
//			OLED_DISPLAY_16x16(6,7*16,13);
//		}
//	
//		/*�˵�33 �˵�3�Ӳ˵��� �������÷���*/
//		if(MENU==200){
//			delay_ms(100);
//			INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //���ַ���ʾ3
//			INVERSE_OLED_DISPLAY_8x16(0,1*8,0x20); //��ʾ����
//			INVERSE_OLED_DISPLAY_16x16(0,1*16,13);
//			INVERSE_OLED_DISPLAY_16x16(0,2*16,6);//������ʾ	 ���ò˵�
//			INVERSE_OLED_DISPLAY_16x16(0,3*16,7);
//			INVERSE_OLED_DISPLAY_16x16(0,4*16,1);
//			INVERSE_OLED_DISPLAY_16x16(0,5*16,2);
//			INVERSE_OLED_DISPLAY_16x16(0,6*16,13); //��ʾ����
//			INVERSE_OLED_DISPLAY_16x16(0,7*16,13);
//			
//			OLED_DISPLAY_16x16(2,0*16,21);//������ʾ	 ��������
//			OLED_DISPLAY_16x16(2,1*16,3);
//			OLED_DISPLAY_16x16(2,2*16,6);
//			OLED_DISPLAY_16x16(2,3*16,7);
//			OLED_DISPLAY_16x16(2,4*16,13);//��ʾ�ո�
//			OLED_DISPLAY_16x16(2,5*16,13);
//			OLED_DISPLAY_16x16(2,6*16,13); 
//			OLED_DISPLAY_16x16(2,7*16,13);
//			
//			INVERSE_OLED_DISPLAY_16x16(4,0*16,22); //��������
//			INVERSE_OLED_DISPLAY_16x16(4,1*16,3);
//			INVERSE_OLED_DISPLAY_16x16(4,2*16,6); 
//			INVERSE_OLED_DISPLAY_16x16(4,3*16,7);
//			INVERSE_OLED_DISPLAY_16x16(4,4*16,13); //��ʾ����
//			INVERSE_OLED_DISPLAY_16x16(4,5*16,13);
//			INVERSE_OLED_DISPLAY_16x16(4,6*16,13); //��ʾ����
//			INVERSE_OLED_DISPLAY_16x16(4,7*16,13);
//			
//			OLED_DISPLAY_16x16(6,0*16,8);//������ʾ	 ��������
//			OLED_DISPLAY_16x16(6,1*16,9);
//			OLED_DISPLAY_16x16(6,2*16,6);
//			OLED_DISPLAY_16x16(6,3*16,7);
//			OLED_DISPLAY_16x16(6,4*16,13);
//			OLED_DISPLAY_16x16(6,5*16,13);
//			OLED_DISPLAY_16x16(6,6*16,13); 
//			OLED_DISPLAY_16x16(6,7*16,13);
//		}
//		
//		/*�˵�34 �˵�3�Ӳ˵��� �������÷���*/
//		if(MENU==250){
//			delay_ms(100);
//			INVERSE_OLED_DISPLAY_8x16(0,0*8,3+0x30); //���ַ���ʾ3
//			INVERSE_OLED_DISPLAY_8x16(0,1*8,0x20); //��ʾ����
//			INVERSE_OLED_DISPLAY_16x16(0,1*16,13);
//			INVERSE_OLED_DISPLAY_16x16(0,2*16,6);//������ʾ	 ���ò˵�
//			INVERSE_OLED_DISPLAY_16x16(0,3*16,7);
//			INVERSE_OLED_DISPLAY_16x16(0,4*16,1);
//			INVERSE_OLED_DISPLAY_16x16(0,5*16,2);
//			INVERSE_OLED_DISPLAY_16x16(0,6*16,13); //��ʾ����
//			INVERSE_OLED_DISPLAY_16x16(0,7*16,13);
//			
//			OLED_DISPLAY_16x16(2,0*16,21);//������ʾ	 ��������
//			OLED_DISPLAY_16x16(2,1*16,3);
//			OLED_DISPLAY_16x16(2,2*16,6);
//			OLED_DISPLAY_16x16(2,3*16,7);
//			OLED_DISPLAY_16x16(2,4*16,13);//��ʾ�ո�
//			OLED_DISPLAY_16x16(2,5*16,13);
//			OLED_DISPLAY_16x16(2,6*16,13);//��ʾ�ո�
//			OLED_DISPLAY_16x16(2,7*16,13);
//			
//			OLED_DISPLAY_16x16(4,0*16,22);//������ʾ	 ��������
//			OLED_DISPLAY_16x16(4,1*16,3);
//			OLED_DISPLAY_16x16(4,2*16,6);
//			OLED_DISPLAY_16x16(4,3*16,7);
//			OLED_DISPLAY_16x16(4,4*16,13);//��ʾ�ո�
//			OLED_DISPLAY_16x16(4,5*16,13);
//			OLED_DISPLAY_16x16(4,6*16,13); //��ʾ�ո�
//			OLED_DISPLAY_16x16(4,7*16,13);
//			
//			INVERSE_OLED_DISPLAY_16x16(6,0*16,8); //���Թ�������
//			INVERSE_OLED_DISPLAY_16x16(6,1*16,9);
//			INVERSE_OLED_DISPLAY_16x16(6,2*16,6); 
//			INVERSE_OLED_DISPLAY_16x16(6,3*16,7);
//			INVERSE_OLED_DISPLAY_16x16(6,4*16,13); //��ʾ����
//			INVERSE_OLED_DISPLAY_16x16(6,5*16,13);
//			INVERSE_OLED_DISPLAY_16x16(6,6*16,13); //��ʾ����
//			INVERSE_OLED_DISPLAY_16x16(6,7*16,13);
//		}
//		
//		
//		/*�˵�35 �˵�3�Ӳ˵��� ʪ�����÷���*/
//		if(MENU==230){
//			OLED_DISPLAY_16x16(2,0*16,22);//������ʾ	 ��������
//			OLED_DISPLAY_16x16(2,1*16,3);
//			OLED_DISPLAY_16x16(2,2*16,6);
//			OLED_DISPLAY_16x16(2,3*16,7);
//			OLED_DISPLAY_16x16(2,4*16,13);
//			OLED_DISPLAY_16x16(2,5*16,13);
//			OLED_DISPLAY_16x16(2,6*16,13); 
//			OLED_DISPLAY_16x16(2,7*16,13);
//			
//			OLED_DISPLAY_16x16(4,0*16,8);//������ʾ	 ��������
//			OLED_DISPLAY_16x16(4,1*16,9);
//			OLED_DISPLAY_16x16(4,2*16,6);
//			OLED_DISPLAY_16x16(4,3*16,7);
//			OLED_DISPLAY_16x16(4,4*16,13);//��ʾ�ո�
//			OLED_DISPLAY_16x16(4,5*16,13);
//			OLED_DISPLAY_16x16(4,6*16,13); 
//			OLED_DISPLAY_16x16(4,7*16,13);
//			
//			INVERSE_OLED_DISPLAY_16x16(6,0*16,5);//������ʾ	 ʪ������
//			INVERSE_OLED_DISPLAY_16x16(6,1*16,4);
//			INVERSE_OLED_DISPLAY_16x16(6,2*16,6);
//			INVERSE_OLED_DISPLAY_16x16(6,3*16,7);
//			INVERSE_OLED_DISPLAY_16x16(6,4*16,13); //��ʾ����
//			INVERSE_OLED_DISPLAY_16x16(6,5*16,13);
//			INVERSE_OLED_DISPLAY_16x16(6,6*16,13); //��ʾ����
//			INVERSE_OLED_DISPLAY_16x16(6,7*16,13);
//		}


//		
//		/*�˵�41 ����������ֵ����*/
//		if(MENU==41 && INT_MARK!=0){
//			if(INT_MARK==1){
//				//BUZZER_BEEP1();//��ʾ��
//				INT_MARK=0;//��λ�ж�ֵ
//				high_temp++;
//				if(high_temp>99) high_temp=low_temp;
//			}
//			if(INT_MARK==2){
//				//BUZZER_BEEP1();//��ʾ��
//				INT_MARK=0;//��λ�ж�ֵ
//				if(high_temp==low_temp) high_temp=100;
//				high_temp--;
//			}
//			if(INT_MARK==3){//������ť��ȷ�ϱ�������ֵ�����ز˵�3
//				INT_MARK=0;//��λ�ж�ֵ
//				MENU=3;//���ز˵�3
//				BUZZER_BEEP4();//�˳���Ч
//			}
//			
//			/*��ʾ���µ�����ֵ*/
//			INVERSE_OLED_DISPLAY_8x16(2,5*8,0x20);//�ո�
//			INVERSE_OLED_DISPLAY_8x16(2, 6*8, high_temp/10+0x30);//����ʮλ
//			INVERSE_OLED_DISPLAY_8x16(2, 7*8, high_temp%10+0x30);//���¸�λ
//			INVERSE_OLED_DISPLAY_16x16(2, 4*16, 16);//���϶�
//		}
//		
//		/*�˵�51 ����������ֵ����*/
//		if(MENU==51 && INT_MARK!=0){
//			if(INT_MARK==1){
//				//BUZZER_BEEP1();//��ʾ��
//				INT_MARK=0;//��λ�ж�ֵ
//				low_temp++;
//				if(low_temp>high_temp) low_temp=0;//�¶ȸ��ڸ��£���������Ϊ0��
//			}
//			if(INT_MARK==2){
//				//BUZZER_BEEP1();//��ʾ��
//				INT_MARK=0;//��λ�ж�ֵ
//				if(low_temp==0) low_temp=high_temp;
//				low_temp--;
//			}
//			if(INT_MARK==3){//������ť��ȷ�ϱ�������ֵ�����ز˵�3
//				INT_MARK=0;//��λ�ж�ֵ
//				MENU=3;//���ز˵�3
//				BUZZER_BEEP4();//�˳���Ч
//			}
//			
//			/*��ʾ���µ�����ֵ*/
//			INVERSE_OLED_DISPLAY_8x16(4,5*8,0x20);//�ո�
//			INVERSE_OLED_DISPLAY_8x16(4, 6*8, low_temp/10+0x30);//����ʮλ
//			INVERSE_OLED_DISPLAY_8x16(4, 7*8, low_temp%10+0x30);//���¸�λ
//			INVERSE_OLED_DISPLAY_16x16(4, 4*16, 16);//���϶�
//		}
//		
//		/*����ǿ��������ֵ����*/
//		if(MENU==61 && INT_MARK!=0){
//			if(INT_MARK==1){
//				//BUZZER_BEEP1();//��ʾ��
//				INT_MARK=0;//��λ�ж�ֵ
//				light_value++;
//				if(light_value>4095) light_value=0;
//				
//			}
//			if(INT_MARK==2){
//				//BUZZER_BEEP1();//��ʾ��
//				INT_MARK=0;//��λ�ж�ֵ
//				if(light_value==0) light_value=4096;
//				light_value--;
//			}
//			if(INT_MARK==3){//������ť��ȷ�ϱ�������ֵ�����ز˵�3
//				INT_MARK=0;//��λ�ж�ֵ
//				MENU=3;//���ز˵�3
//				BUZZER_BEEP4();//�˳���Ч
//			}
//			
//			/*��ʾ����ǿ�ȵ�����ֵ*/
//			INVERSE_OLED_DISPLAY_8x16(4, 6*8, light_value/1000+0x30);//��ɫ��ʾ����ǿ������ֵ
//			INVERSE_OLED_DISPLAY_8x16(4, 7*8, light_value/100%10+0x30);
//			INVERSE_OLED_DISPLAY_8x16(4, 8*8, light_value/10%10+0x30);
//			INVERSE_OLED_DISPLAY_8x16(4, 9*8, light_value%10+0x30);
//		}
//		
//		/*ʪ��������ֵ����*/
//		if(MENU==71 && INT_MARK!=0){
//			if(INT_MARK==1){
//				//BUZZER_BEEP1();//��ʾ��
//				INT_MARK=0;//��λ�ж�ֵ
//				humidity_value++;
//				if(humidity_value>90) humidity_value=0;
//				
//			}
//			if(INT_MARK==2){
//				//BUZZER_BEEP1();//��ʾ��
//				INT_MARK=0;//��λ�ж�ֵ
//				if(humidity_value==0) humidity_value=90;
//				humidity_value--;
//			}
//			if(INT_MARK==3){//������ť��ȷ�ϱ�������ֵ�����ز˵�3
//				INT_MARK=0;//��λ�ж�ֵ
//				MENU=3;//���ز˵�3
//				BUZZER_BEEP4();//�˳���Ч
//			}
//			/*��ʾʪ�ȵ�����ֵ*/
//			INVERSE_OLED_DISPLAY_8x16(4, 6*8, humidity_value/10+0x30);//��ɫ��ʾʪ������ֵ
//			INVERSE_OLED_DISPLAY_8x16(4, 7*8, humidity_value%10+0x30);
//			INVERSE_OLED_DISPLAY_8x16(4, 8*8, 0x25);
//		}
		
		
		
		
		
	}
}
