

#include "encoder.h"


u8 KUP; //旋钮锁死标志位
u16 cou; //用于锁死计数



/*接口初始化*/
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
	if(GPIO_ReadInputDataBit(ENCODER_PORT_A, ENCODER_L))KUP=0; //判断旋钮是否接触锁死
	if(!GPIO_ReadInputDataBit(ENCODER_PORT_A, ENCODER_L)&&KUP==0){//判断是否旋转旋钮，同时判断是否有旋钮锁死
		delay_us(100);
		kt=GPIO_ReadInputDataBit(ENCODER_PORT_B, ENCODER_R); //读取另一端旋钮状态
		delay_ms(3); //延时去抖
		if(!GPIO_ReadInputDataBit(ENCODER_PORT_A, ENCODER_L)){
			if(kt==0) a=1; //右转
			else a=2; //左转
			cou = 0;
			while(!(GPIO_ReadInputDataBit(ENCODER_PORT_A, ENCODER_L))&&cou<60000){
				cou++;
				KUP=1;
				delay_us(20);
			}
		}
	}
	if(!GPIO_ReadInputDataBit(ENCODER_PORT_A, ENCODER_D)&&KUP==0){
		delay_ms(20);//去抖
		if(!GPIO_ReadInputDataBit(ENCODER_PORT_A,ENCODER_D)){
			a=3;
		}
	}
	return a;
}

























