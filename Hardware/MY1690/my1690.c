

#include "my1690.h"
#include "delay.h"


void MY1690_Init(void)//��ʼ��
{
	USART3_Init(9600); //оƬ�ֲ�Ҫ������Ϊ9600
	MY1690_CMD2(0x35, 1);//�豸�л�
	delay_ms(25);
	MY1690_CMD2(0x33, 0);//ѭ����ʽ
	delay_ms(25);
	MY1690_CMD2(0x32, 1);//����EQ
	delay_ms(25);
	MY1690_CMD2(0x31, 10);//��������
	delay_ms(25);
	MY1690_STOP();
}

void MY1690_STOP(void){
	USART3_printf("\x7e\x03\x1E\x1D\xef");
}

void MY1690_PLAY(void){ //����
	USART3_printf("\x7e\x03\x11\x12\xef"); //���� \x ���ʮ����������
}

void MY1690_PAUSE(void){ //��ͣ
	//USART3_printf("\x7e\x03\x12\x11\xef");
	//USART3_printf("\x7e\x03\x1E\x1D\xef");
	USART3_printf("\x7e\x03\x1c\x1f\xef");
}

void MY1690_PREV(void){ //��һ��
	USART3_printf("\x7e\x03\x14\x17\xef");
}

void MY1690_NEXT(void){ //��һ��
	USART3_printf("\x7e\x03\x13\x10\xef");
}

void MY1690_VUP(void){ //������1
	USART3_printf("\x7e\x03\x15\x16\xef");
}

void MY1690_VDOWN(void){ //������1
	USART3_printf("\x7e\x03\x16\x15\xef");
}



void MY1690_CMD1(u8 a){	//�޲�����ָ���  a������
	u8 i;
	i=3^a;//ȡУ���루���
	USART_SendData(USART3 , 0x7e);	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //��鷢���жϱ�־λ
	USART_SendData(USART3 , 0x03);	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //��鷢���жϱ�־λ
	USART_SendData(USART3 , a);		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //��鷢���жϱ�־λ
	USART_SendData(USART3 , i);		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //��鷢���жϱ�־λ
	USART_SendData(USART3 , 0xef);	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //��鷢���жϱ�־λ
}
void MY1690_CMD2(u8 a,u8 b){ //�в�����ָ��� a������ b����
	u8 i;
	i=4^a;//ȡУ���루���
	i=i^b;//ȡУ���루���
	USART_SendData(USART3 , 0x7e);	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //��鷢���жϱ�־λ
	USART_SendData(USART3 , 0x04);	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //��鷢���жϱ�־λ
	USART_SendData(USART3 , a);		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //��鷢���жϱ�־λ
	USART_SendData(USART3 , b);		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //��鷢���жϱ�־λ
	USART_SendData(USART3 , i);		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //��鷢���жϱ�־λ
	USART_SendData(USART3 , 0xef);	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //��鷢���жϱ�־λ
}
void MY1690_CMD3(u8 a,u16 b){ //�в�����ָ��� a������ b������16λ��
	u8 i,c,d;
	c=b/0x100; //��16λ�����ֳ�2��8λ����
	d=b%0x100;
	i=5^a;//ȡУ���루���
	i=i^c;//ȡУ���루���
	i=i^d;//ȡУ���루���
	USART_SendData(USART3 , 0x7e);	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //��鷢���жϱ�־λ
	USART_SendData(USART3 , 0x05);	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //��鷢���жϱ�־λ
	USART_SendData(USART3 , a);		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //��鷢���жϱ�־λ
	USART_SendData(USART3 , c);		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //��鷢���жϱ�־λ
	USART_SendData(USART3 , d);		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //��鷢���жϱ�־λ
	USART_SendData(USART3 , i);		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //��鷢���жϱ�־λ
	USART_SendData(USART3 , 0xef);	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); //��鷢���жϱ�־λ
}

/*
* MY1690�豸�л�
*0-U�� 1-SD��
*/
void MY1690_SetDevice(u8 Data){
	MY1690_CMD2(0x35, Data);//�豸�л�
}

/*
* MY1690����ѭ����ʽ
*0-ȫ�� 1-�ļ��� 2-���� 3-��� 4-��ѭ��
*/
void MY1690_SetLoop(u8 Data){
	MY1690_CMD2(0x33, Data);//ѭ����ʽ
}

/*
*MY1690 ����EQ
*0-NO 1-POP 2-ROCK 3-JAZZ 4-CLASSIC 5-BASS
*/
void MY1690_SetEQ(u8 Data){
	MY1690_CMD2(0x32, Data);//����EQ
}

/*
*MY1690 ����EQ
*0-NO 1-POP 2-ROCK 3-JAZZ 4-CLASSIC 5-BASS
*/
void MY1690_SetVOL(u8 Data){
	MY1690_CMD2(0x31, Data);//��������
}






















