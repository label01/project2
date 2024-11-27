#ifndef __ENCODER_H
#define __ENCODER_H


#include "sys.h"
#include "delay.h"


#define ENCODER_PORT_A GPIOA
#define ENCODER_L GPIO_Pin_6
#define ENCODER_D GPIO_Pin_7

#define ENCODER_PORT_B GPIOB
#define ENCODER_R GPIO_Pin_2

void ENCODER_Init(void);
u8 ENCODER_READ(void);






#endif

