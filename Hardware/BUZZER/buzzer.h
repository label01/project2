#ifndef __BUZZER_H
#define __BUZZER_H


#include "sys.h"

#define BUZZERPORT	GPIOB //����������˿�
#define BUZZER	GPIO_Pin_5 //PB5

void BUZZER_Init(void);//��ʼ��
void BUZZER_BEEP1(void);//����
void BUZZER_BEEP2(void);//������
void BUZZER_BEEP3(void);//�����ʱ������ʾ��
void BUZZER_BEEP4(void);//�˳���ʱ������ʾ��
void MIDI_PLAY(void);


#endif
