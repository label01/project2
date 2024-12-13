

#include "buzzer.h"
#include "delay.h"

/*��ʼ��*/
void BUZZER_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = BUZZER;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BUZZERPORT, &GPIO_InitStructure);
	
	GPIO_WriteBit(BUZZERPORT, BUZZER, (BitAction)(1)); //Ĭ������ߵ�ƽ����������·ʹ��PNP�������ܣ���·����ͨ��
}




/*����*/
void BUZZER_BEEP1(void){
	u16 i;
	for(i=0; i<200; i++){
		GPIO_WriteBit(BUZZERPORT, BUZZER, (BitAction)(0));
		delay_us(500);
		GPIO_WriteBit(BUZZERPORT, BUZZER, (BitAction)(1));
		delay_us(500);
	}
}

void BUZZER_BEEP2(void){ //��������һ������������
	u16 i;
	for(i=0;i<400;i++){
		GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(0)); //�������ӿ����0
		delay_us(800); //��ʱ		
		GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(1)); //�������ӿ�����ߵ�ƽ1
		delay_us(800); //��ʱ		
	}
}
void BUZZER_BEEP3(void){ //��������һ���������ʱ����
	u16 i;
	for(i=0;i<150;i++){
		GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(0)); //�������ӿ����0
		delay_us(200); //��ʱ		
		GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(1)); //�������ӿ�����ߵ�ƽ1
		delay_us(200); //��ʱ		
	}
	for(i=0;i<100;i++){
		GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(0)); //�������ӿ����0
		delay_us(300); //��ʱ		
		GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(1)); //�������ӿ�����ߵ�ƽ1
		delay_us(300); //��ʱ		
	}
	for(i=0;i<250;i++){
		GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(0)); //�������ӿ����0
		delay_us(100); //��ʱ		
		GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(1)); //�������ӿ�����ߵ�ƽ1
		delay_us(100); //��ʱ		
	}
}
void BUZZER_BEEP4(void){ //��������һ�����˳���ʱ����
	u16 i;
	for(i=0;i<450;i++){
		GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(0)); //�������ӿ����0
		delay_us(200); //��ʱ		
		GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(1)); //�������ӿ�����ߵ�ƽ1
		delay_us(200); //��ʱ		
	}
	for(i=0;i<250;i++){
		GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(0)); //�������ӿ����0
		delay_us(400); //��ʱ		
		GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(1)); //�������ӿ�����ߵ�ƽ1
		delay_us(400); //��ʱ		
	}
	for(i=0;i<450;i++){
		GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(0)); //�������ӿ����0
		delay_us(100); //��ʱ		
		GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(1)); //�������ӿ�����ߵ�ƽ1
		delay_us(100); //��ʱ		
	}
}


uc16 music1[78]={ //����1�����ݱ�������������ż���ǳ��ȣ� uc16 �޷��ŵ�ֻ��������ֻ�������洢��flash��
330,750,
440,375,
494,375,
523,750,
587,375,
659,375,
587,750,
494,375,
392,375,
440,1500,
330,750,
440,375,
494,375,
523,750,
587,375,
659,375,
587,750,
494,375,
392,375,
784,1500,
659,750,
698,375,
784,375,
880,750,
784,375,
698,375,
659,750,
587,750,
659,750,
523,375,
494,375,
440,750,
440,375,
494,375,
523,750,
523,750,
494,750,
392,750,
440,3000
};


void MIDI_PLAY(void){ //MIDI����
	u16 i,e;
	for(i=0;i<39;i++){
		for(e=0;e<music1[i*2]*music1[i*2+1]/1000;e++){
			GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(0)); //�������ӿ����0
			delay_us(500000/music1[i*2]); //��ʱ		
			GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(1)); //�������ӿ�����ߵ�ƽ1
			delay_us(500000/music1[i*2]); //��ʱ	
		}	
	}
}




