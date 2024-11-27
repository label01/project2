












#include "usart.h"


//ʹUSART���ڿ���printf��������
//��usart.h�ļ���ɸ���printf�����Ĵ��ں�
#if 1
#pragma import(__use_no_semihosting)
struct __FILE{
	int handle;
};

FILE __stdout;

//�ղ���
_sys_exit(int x){
	x = x;
}

//�ض���fpuc����
int fputc(int ch, FILE *f){
	while((USART_n->SR&0x40)==0);//ѭ�����ͣ�֪���������
	USART_n->DR = (u8) ch;
	return ch;
}




#endif

/*USART1 ������س���*/

#if EN_USART1 
u8 USART1_RX_BUF[USART1_REC_LEN];

u16 USART1_RX_STA=0;


/* ����1������ PA9 PA10 */
void USART1_Init(u32 bound){
	//������GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	//ʹ�ܻ�ʧ��APB2����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1, ENABLE);
	//USART1_RX PA9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//USART1_TX PA10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ; //��������
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//�����ж�
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; //����1�ж�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ��IRQͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�
	NVIC_Init(&NVIC_InitStructure);
	
	//USART1 ��ʼ������
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
	USART_InitStructure.USART_Parity = USART_Parity_No; //����żУ��λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //һ��ֹͣλ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE, ENABLE); //USART_IT_RXNE: Receive Data register not empty interrupt
	USART_Cmd(USART1, ENABLE); //ʹ�ܴ���
	
}



	//�������ַ������յ�USART_RX_BUF[]�ĳ���(USART_RX_STA&0x3FFF)�����ݵĳ��ȣ��������س���
	//��(USART_RX_STA&0xC000)Ϊ��ʱ��ʾ���ݽ�����ɣ��������ն��ﰴ�»س�����
	//����������д�ж�if(USART_RX_STA&0xC000)��Ȼ���USART_RX_BUF[]���飬����0x0d 0x0a���ǽ�����
	//ע���������������괮�����ݺ�Ҫ��USART_RX_STA��0
void USART1_IRQHandler(void){ //����1�жϷ�����򣨹̶��ĺ����������޸ģ�	
	uint8_t Res; //��һ���������ڽ�������
	if(USART_GetFlagStatus(USART1, USART_IT_RXNE) != RESET){//�жϴ��ڽ������ݱ�־λ�ǿ�
		Res = USART_ReceiveData(USART1); // ��ȡ���յ�����
		printf("%c", Res); //����������ݴ�ӡ�ڳ����ն��ϣ�Ҫ��û����ʾ
		//��ʼ�����������
		/*
		USART1_RX_STA ��һ���޷��ŵ�ʮ��λȫ�ֱ���
		bit15��	������ɱ�־
		bit14��	���յ�0x0d
		bit13~0��	���յ�����Ч�ֽ���Ŀ
		*/
		if((USART1_RX_STA&0x8000) == 0){ //����δ���
			if(USART1_RX_STA&0x4000){//���յ�0x0dҲ���ǻس��ַ�
				if(Res != 0x0a)USART1_RX_STA=0; // ���մ���0x0a�ǻ��з�������ڽ��յ��س���֮���ǻ��з��Ǿʹ����ˣ����¿�ʼ
				else USART1_RX_STA |= 0x8000; //�����λ��1����ʾ�������
			}else{//û���յ��س�
				if(Res==0x0d)USART1_RX_STA|=0x4000;//����־λ��1
				else{
					USART1_RX_BUF[USART1_RX_STA&0x3fff] = Res;
					USART1_RX_STA++;//���ݳ��ȼ�1
					if(USART1_RX_STA > (USART1_REC_LEN-1))USART1_RX_STA = 0;// ���ճ��ȴ��ڻ����������¿�ʼ����
				}
				
			}
		}
	
	}
} 
#endif	




















