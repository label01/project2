#ifndef __ADC_H
#define __ADC_H



#include "sys.h"



#define ADC1_DR_Address ((uint32_t)0x4001244C)


#define ADCPORT	GPIOA

#define ADC_CH5	GPIO_Pin_5 //����ADC�ӿڣ���������
#define ADC_CH4 GPIO_Pin_4 //����ADC�ӿڵ�λ��
#define ADC_CH6 GPIO_Pin_6 //����ADC�ӿ� ҡ��X
#define ADC_CH7 GPIO_Pin_7 //����ADC�ӿ� ҡ��Y
#define ADC_CH1 GPIO_Pin_1 //����ADC�ӿ� �Զ���ӿ�





void ADC_DMA_Init(void);
void ADC_GPIO_Init(void);
void ADC_Configuration(void);



















#endif
