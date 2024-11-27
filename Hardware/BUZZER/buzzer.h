#ifndef __BUZZER_H
#define __BUZZER_H


#include "sys.h"

#define BUZZERPORT	GPIOB //定义蜂鸣器端口
#define BUZZER	GPIO_Pin_5 //PB5

void BUZZER_Init(void);//初始化
void BUZZER_BEEP1(void);//蜂鸣
void MIDI_PLAY(void);


#endif
