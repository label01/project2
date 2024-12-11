
#include "nvic.h"
#include "encoder.h"

u8 INT_MARK=0;//�жϱ�־λ
u8 encoder_key;//�洢��������ֵ

//ϵͳ��ʱ�����������ȷֻ��һ����ŵ�ֵ
void NVIC_Delay_Us(u16 time){
	u16 i=0;
	while(time--){
		i=10;//�Լ�����
		while(i--);
	}
}


//ϵͳ��ʱ�����������ȷֻ��һ����ŵ�ֵ
void NVIC_Delay_Ms(u16 time){
	u16 i=0;
	while(time--){
		i=12000;//�Լ�����
		while(i--);
	}
}


void KEYPAD4x4_INT_INIT(void){
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//����GPIOʱ�ӣ���Ҫ�ڸ���ʱ��һͬ����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //���ö˿��ж���Ҫ���ø���ʱ��
	
	//��һ���ж�
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;//�ж�ģʽΪ �ж�
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI4_IRQn; //�ж���
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2; //�����ȼ� 2
	NVIC_Init(&NVIC_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);
	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;//�ж�ģʽΪ �ж�
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn; //�ж���
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2; //�����ȼ� 2
	NVIC_Init(&NVIC_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
	EXTI_InitStructure.EXTI_Line=EXTI_Line6;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;//�ж�ģʽΪ �ж�
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn; //�ж���
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2; //�����ȼ� 2
	NVIC_Init(&NVIC_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7);
	EXTI_InitStructure.EXTI_Line=EXTI_Line7;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;//�ж�ģʽΪ �ж�
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn; //�ж���
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2; //�����ȼ� 2
	NVIC_Init(&NVIC_InitStructure);

}


//�������жϳ�ʼ��
void ENCODER_INT_INIT(void){
	NVIC_InitTypeDef NVIC_InitStructure;//����һ��NVIC_InitTypeDef�ṹ����������������жϿ������Ĳ���
	EXTI_InitTypeDef EXTI_InitStructure;//����һ��EXTI_InitTypeDef�ṹ����������������жϵĲ���
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//����GPIOʱ�ӣ���Ҫ�ڸ���ʱ��һͬ����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //���ö˿��ж���Ҫ���ø���ʱ��
	
	//��һ���ж�
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);//ѡ��Ҫ���õ��жϵĶ˿ں�����
	EXTI_InitStructure.EXTI_Line=EXTI_Line6;//�����ж���ΪEXTI_Line6�������ӦPA6
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;//ʹ���ж���
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;//�����ж�ģʽΪ�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//�����жϴ�����ʽΪ�½��ش�������PA6���ŵĵ�ƽ�Ӹߵ��ͱ仯ʱ���������ж�
	EXTI_Init(&EXTI_InitStructure);//����EXTI_Init����������EXTI_InitStructure�����ó�ʼ���ⲿ�ж�
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn;//�����ж���EXTI9_5_IRQn�����Ӧ�ⲿ�ж���9-5
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//ʹ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//������ռ���ȼ�����ռ���ȼ��������ܷ�����������ִ�е��жϷ������
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2; //���������ȼ� 2�������ȼ�����������ͬ��ռ���ȼ����ж��У��ĸ��жϻ��ȱ�ִ��
	NVIC_Init(&NVIC_InitStructure);//����NVIC_Init����������NVIC_InitStructure�����ó�ʼ���жϿ�����
	
		//�ڶ����ж�
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7);//ѡ��Ҫ���õ��жϵĶ˿ں�����
	EXTI_InitStructure.EXTI_Line=EXTI_Line7;//�����ж���ΪEXTI_Line7�������ӦPA7
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;//ʹ���ж���
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;//�����ж�ģʽΪ�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//�����жϴ�����ʽΪ�½��ش�������PA7���ŵĵ�ƽ�Ӹߵ��ͱ仯ʱ���������ж�
	EXTI_Init(&EXTI_InitStructure);//����EXTI_Init����������EXTI_InitStructure�����ó�ʼ���ⲿ�ж�
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn;//�����ж���EXTI9_5_IRQn�����Ӧ�ⲿ�ж���9-5
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//ʹ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//������ռ���ȼ�����ռ���ȼ��������ܷ�����������ִ�е��жϷ������
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2; //���������ȼ� 2�������ȼ�����������ͬ��ռ���ȼ����ж��У��ĸ��жϻ��ȱ�ִ��
	NVIC_Init(&NVIC_InitStructure);//����NVIC_Init����������NVIC_InitStructure�����ó�ʼ���жϿ�����
	
}




//void  EXTI4_IRQHandler(void){
//	if(EXTI_GetITStatus(EXTI_Line4)!=RESET){//�ж�ĳ�����ϵ��ж��Ƿ��� 
//		INT_MARK=1;//��־λ��1����ʾ�а����ж�
//		EXTI_ClearITPendingBit(EXTI_Line4);   //��� LINE �ϵ��жϱ�־λ
//	}     
//}

void  EXTI9_5_IRQHandler(void){    
	if(EXTI_GetITStatus(EXTI_Line6)!=RESET){//�ж�ĳ�����ϵ��ж��Ƿ��� 
		NVIC_Delay_Us(100);//��ʱ100΢��
		encoder_key=GPIO_ReadInputDataBit(ENCODER_PORT_B, ENCODER_R);//��ȡ�������Ҽ�ֵ
		NVIC_Delay_Ms(3);//��ʱ3����
		if(!GPIO_ReadInputDataBit(ENCODER_PORT_A, ENCODER_L)){
			if(encoder_key==0) INT_MARK=1;//��ת
			if(encoder_key==1) INT_MARK=2;//��ת
			u16 a=0;//�ж�������ʱ
			while(!GPIO_ReadInputDataBit(ENCODER_PORT_A, ENCODER_L) && a<60000){//�ȴ���ť�ſ���ͬʱ�ж��Ƿ�����
				a++;
				NVIC_Delay_Us(20);
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line6);   //��� LINE �ϵ��жϱ�־λ
	}     
	if(EXTI_GetITStatus(EXTI_Line7)!=RESET){//�ж�ĳ�����ϵ��ж��Ƿ��� 
		NVIC_Delay_Ms(20);//��ʱ20ms
		if(!GPIO_ReadInputDataBit(ENCODER_PORT_A, ENCODER_D)) INT_MARK=3;//�жϰ����Ƿ���
		EXTI_ClearITPendingBit(EXTI_Line7);   //��� LINE �ϵ��жϱ�־λ
	}     
}



























































