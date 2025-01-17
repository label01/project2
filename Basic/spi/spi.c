

#include "spi.h"



void SPI2_Init(void){

	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//ʹ��SPI2ʱ��
	
	
	GPIO_InitStructure.GPIO_Pin = SPI2_MISO; //spi2�������� ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SPI2PORT, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = SPI2_MOSI|SPI2_SCK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//������������������ch376��ʼ�����ɹ�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI2PORT, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = SPI2_NSS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//��ʵ���Ƽ��ø����������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(SPI2PORT, &GPIO_InitStructure);
	
	
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//ʱ�ӵ�Ԥ��Ƶֵ
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;//ʱ����λ�� 1��ʾ��SCK�������ر߲����� 2��ʾż���ر߲���
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_High; //����״̬ʱSCK��״̬�� HighΪ�ߵ�ƽ�� lowΪ�͵�ƽ
	SPI_InitStructure.SPI_CRCPolynomial=7; //CRCУ��͵Ķ���ʽ�� �������ݸ�ʽ����λ����֡�Ͳ���CR6�� 16λ����֡����CRC16��
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex; //˫���������ȫ˫��ģʽ��
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB; //��λ��ǰ���ο�CH376�����ֲ�˵��������CPOL=CPHA=1  || CPOL=CPHA=0 �� MSB����λ��ǰ
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;//��Ϊ����ֻ��һ��spi���豸������ֻ���Խ�������Զ����ƣ�����Ƕ�����豸���͵��ֶ�����
	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE);
	
}




u8 SPI2_SendByte(u8 Byte){
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPI2, Byte);//�����ͼĴ�����д��Ҫ���͵�����
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET);//������ռĴ���û���յ����ݣ� ѭ���ȴ�
	return SPI_I2S_ReceiveData(SPI2);
}






























