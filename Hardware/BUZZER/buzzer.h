#ifndef __BUZZER_H
#define __BUZZER_H


#include "sys.h"

#define BUZZERPORT	GPIOB //����������˿�
#define BUZZER	GPIO_Pin_5 //PB5

void BUZZER_Init(void);//��ʼ��
void BUZZER_BEEP1(void);//����
void MIDI_PLAY(void);


#endif
