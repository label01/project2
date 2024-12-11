#ifndef __ADC_H
#define __ADC_H



#include "sys.h"



#define ADC1_DR_Address ((uint32_t)0x4001244C)


#define ADCPORT	GPIOA

#define ADC_CH5	GPIO_Pin_5 //定义ADC接口，光敏电阻
#define ADC_CH4 GPIO_Pin_4 //定义ADC接口电位器
#define ADC_CH6 GPIO_Pin_6 //定义ADC接口 摇杆X
#define ADC_CH7 GPIO_Pin_7 //定义ADC接口 摇杆Y
#define ADC_CH1 GPIO_Pin_1 //定义ADC接口 自定义接口





void ADC_DMA_Init(void);
void ADC_GPIO_Init(void);
void ADC_Configuration(void);



















#endif
