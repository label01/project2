#ifndef __OLED0561_H
#define __OLED0561_H

#include "sys.h"
#include "i2c.h"




#define OLED0561_ADD	0x78 //д�������ֲ���ģ� ���ʮ��Ž�VDD �ӵ�ַ�ͱ�Ϊ 0111101b,Ҳ����0x7A
#define COM		0x00
#define DAT		0x40



void OLED0561_Init(void);//��ʼ��
void OLED_DISPLAY_ON (void);//OLED������ʾ
void OLED_DISPLAY_OFF (void);//OLED������ʾ
void OLED_DISPLAY_LIT (u8 x);//OLED���������ã�0~255��
void OLED_DISPLAY_CLEAR(void);//��������
void OLED_DISPLAY_8x16(u8 x,u8 y,u16 w);//��ʾ8x16�ĵ����ַ� 
void INVERSE_OLED_DISPLAY_8x16(u8 x, u8 y, u16 w);//��ɫ��ʾӢ���ַ�
void OLED_DISPLAY_8x16_BUFFER(u8 row,u8 *str);//��ʾ8x16���ַ���

void OLED_DISPLAY_16x16(u8 x, u8 y, u16 w); //������ʾ
void INVERSE_OLED_DISPLAY_16x16(u8 x, u8 y, u16 w); //���� ��ɫ��ʾ

void OLED_DISPLAY_PIC1(void);//��ʾͼƬ






#endif


