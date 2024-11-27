

#include "encoder.h"


u8 KUP; //��ť������־λ
u16 cou; //������������



/*�ӿڳ�ʼ��*/
void ENCODER_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = ENCODER_D|ENCODER_L;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(ENCODER_PORT_A, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ENCODER_R;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(ENCODER_PORT_B, &GPIO_InitStructure);

}


u8 ENCODER_READ(void){
	u8 a;
	u8 kt;
	a=0;
	if(GPIO_ReadInputDataBit(ENCODER_PORT_A, ENCODER_L))KUP=0; //�ж���ť�Ƿ�Ӵ�����
	if(!GPIO_ReadInputDataBit(ENCODER_PORT_A, ENCODER_L)&&KUP==0){//�ж��Ƿ���ת��ť��ͬʱ�ж��Ƿ�����ť����
		delay_us(100);
		kt=GPIO_ReadInputDataBit(ENCODER_PORT_B, ENCODER_R); //��ȡ��һ����ť״̬
		delay_ms(3); //��ʱȥ��
		if(!GPIO_ReadInputDataBit(ENCODER_PORT_A, ENCODER_L)){
			if(kt==0) a=1; //��ת
			else a=2; //��ת
			cou = 0;
			while(!(GPIO_ReadInputDataBit(ENCODER_PORT_A, ENCODER_L))&&cou<60000){
				cou++;
				KUP=1;
				delay_us(20);
			}
		}
	}
	if(!GPIO_ReadInputDataBit(ENCODER_PORT_A, ENCODER_D)&&KUP==0){
		delay_ms(20);//ȥ��
		if(!GPIO_ReadInputDataBit(ENCODER_PORT_A,ENCODER_D)){
			a=3;
		}
	}
	return a;
}

























