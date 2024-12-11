#include "adc.h"



vu16 ADC_DMA_IN[3]; //ADC数值存放的变量


void ADC_GPIO_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//使能DMA时钟， 用于ADC的数据传输
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);// 使能ADC1时钟
	GPIO_InitStructure.GPIO_Pin = ADC_CH4|ADC_CH5|ADC_CH1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	GPIO_Init(ADCPORT, &GPIO_InitStructure);
	
}
	
	


void ADC_DMA_Init(void){
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel1);//更好的管理DMA模块的初始化过程，确保其在正确的时机以正确的状态被激活。
	DMA_InitStructure.DMA_BufferSize = 3;//定义DMA缓冲区大小（根据ADC采集通道数量修改）
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//指定外设为源地址
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//禁止DMA通道存储器到存储器传输
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_DMA_IN;//定义DMA通道ADC数据存储
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord; //定义外设数据宽度16位
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;//当前存储器地址， disable不变， enable递增
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular; //DMA通道操作模式位环形缓冲模式
	DMA_InitStructure.DMA_PeripheralBaseAddr=ADC1_DR_Address;//dma通道外设基地址
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;//定义存储器数据宽度16位
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;//当前外设寄存器地址不变
	DMA_InitStructure.DMA_Priority=DMA_Priority_High;//DMA通道优先级高
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//初始化DMA通道
	DMA_Cmd(DMA1_Channel1,ENABLE);//使能DMA通道

}



void ADC_Configuration(void){
	ADC_InitTypeDef ADC_InitStructure;
	ADC_GPIO_Init();
	ADC_DMA_Init();
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;//ADC转换工作在连续模式
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;//转换数据右对齐
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//由软件控制转换
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;//ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_NbrOfChannel=3;//输入进行规则转换的ADC通道的数据（根据ADC采集通道数量修改）
	ADC_InitStructure.ADC_ScanConvMode=ENABLE;//使能扫描
	ADC_Init(ADC1, &ADC_InitStructure);//
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_13Cycles5);//ADC1选择信道x,采样顺序y,采样时间n个周期
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_13Cycles5);//ADC1选择信道x,采样顺序y,采样时间n个周期
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 3, ADC_SampleTime_13Cycles5);

	
	ADC_DMACmd(ADC1, ENABLE);//开启ADC的DMA支持（要实现DMA功能， 还需独立配置DMA通道等参数）
	ADC_Cmd(ADC1, ENABLE);//使能ADC1
	ADC_ResetCalibration(ADC1);//重置ADC1校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1));//等待ADC1校准完成
	ADC_StartCalibration(ADC1);//开启ADC1校准
	while(ADC_GetCalibrationStatus(ADC1));//等待ADC1校准完成
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//使能ADC1软件开始转换

}

































