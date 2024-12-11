#include "adc.h"



vu16 ADC_DMA_IN[3]; //ADC��ֵ��ŵı���


void ADC_GPIO_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//ʹ��DMAʱ�ӣ� ����ADC�����ݴ���
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);// ʹ��ADC1ʱ��
	GPIO_InitStructure.GPIO_Pin = ADC_CH4|ADC_CH5|ADC_CH1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//ģ������
	GPIO_Init(ADCPORT, &GPIO_InitStructure);
	
}
	
	


void ADC_DMA_Init(void){
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel1);//���õĹ���DMAģ��ĳ�ʼ�����̣�ȷ��������ȷ��ʱ������ȷ��״̬�����
	DMA_InitStructure.DMA_BufferSize = 3;//����DMA��������С������ADC�ɼ�ͨ�������޸ģ�
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//ָ������ΪԴ��ַ
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//��ֹDMAͨ���洢�����洢������
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_DMA_IN;//����DMAͨ��ADC���ݴ洢
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord; //�����������ݿ��16λ
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;//��ǰ�洢����ַ�� disable���䣬 enable����
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular; //DMAͨ������ģʽλ���λ���ģʽ
	DMA_InitStructure.DMA_PeripheralBaseAddr=ADC1_DR_Address;//dmaͨ���������ַ
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;//����洢�����ݿ��16λ
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;//��ǰ����Ĵ�����ַ����
	DMA_InitStructure.DMA_Priority=DMA_Priority_High;//DMAͨ�����ȼ���
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//��ʼ��DMAͨ��
	DMA_Cmd(DMA1_Channel1,ENABLE);//ʹ��DMAͨ��

}



void ADC_Configuration(void){
	ADC_InitTypeDef ADC_InitStructure;
	ADC_GPIO_Init();
	ADC_DMA_Init();
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;//ADCת������������ģʽ
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;//ת�������Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//���������ת��
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;//ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_NbrOfChannel=3;//������й���ת����ADCͨ�������ݣ�����ADC�ɼ�ͨ�������޸ģ�
	ADC_InitStructure.ADC_ScanConvMode=ENABLE;//ʹ��ɨ��
	ADC_Init(ADC1, &ADC_InitStructure);//
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_13Cycles5);//ADC1ѡ���ŵ�x,����˳��y,����ʱ��n������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_13Cycles5);//ADC1ѡ���ŵ�x,����˳��y,����ʱ��n������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 3, ADC_SampleTime_13Cycles5);

	
	ADC_DMACmd(ADC1, ENABLE);//����ADC��DMA֧�֣�Ҫʵ��DMA���ܣ� �����������DMAͨ���Ȳ�����
	ADC_Cmd(ADC1, ENABLE);//ʹ��ADC1
	ADC_ResetCalibration(ADC1);//����ADC1У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));//�ȴ�ADC1У׼���
	ADC_StartCalibration(ADC1);//����ADC1У׼
	while(ADC_GetCalibrationStatus(ADC1));//�ȴ�ADC1У׼���
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//ʹ��ADC1�����ʼת��

}

































