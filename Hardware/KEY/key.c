#include "key.h"



/*΢�����س�ʼ��*/
void KEY_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure; //����GPIO�ĳ�ʼ��ö�ٽṹ
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ�ܻ���ʧ�� APB2 ����ʱ��
	GPIO_InitStructure.GPIO_Pin = KEY1 | KEY2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //ѡ��IO�ӿڹ�����ʽ�� ��������
	GPIO_Init(KEYPORT, &GPIO_InitStructure);//���� GPIO_InitStruct ��ָ���Ĳ�����ʼ������ GPIOx �Ĵ���
}
