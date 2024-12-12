
#include "dht11.h"






void DHT11_IO_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Pin=DHT11_IO;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(DHT11PORT, &GPIO_InitStruct);
}
void DHT11_IO_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Pin=DHT11_IO;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(DHT11PORT, &GPIO_InitStruct);
}
void DHT11_RST(void)
{
	DHT11_IO_OUT();//IO���״̬
	GPIO_ResetBits(DHT11PORT, DHT11_IO);
	delay_ms(20);//��������Ҫ����18ms
	GPIO_SetBits(DHT11PORT, DHT11_IO);
	delay_us(40);
}

u8 DHT11_Check(void){
	u8 tim=0;
	DHT11_IO_IN();//�л�δ����
	while(GPIO_ReadInputDataBit(DHT11PORT, DHT11_IO)&&tim<80){
		tim++;
		delay_us(1);
	}
	if (tim<80){
		tim=0;
	}
	else{
		return 1;
	}
	
	while(!GPIO_ReadInputDataBit(DHT11PORT, DHT11_IO) && tim<100){
		tim++;
		delay_us(1);
	}
	
	if(tim>100){
		return 1;
	}else{
		return 0;
	}
	
	
}



u8 DHT11_Init(void){ //DHT11��ʼ��
	DHT11_RST();
	return DHT11_Check();
}


u8 DHT11_ReadBit(void){
	u8 tim=0;
	while(GPIO_ReadInputDataBit(DHT11PORT, DHT11_IO)&&tim<100){
		tim++;
		delay_us(1);
	}
	tim=0;
	while(!GPIO_ReadInputDataBit(DHT11PORT, DHT11_IO)&&tim<100){
		tim++;
		delay_us(1);
	}
	delay_us(40);
	if(GPIO_ReadInputDataBit(DHT11PORT, DHT11_IO)){
		return 1;
	}else{
		return 0;
	}
}

u8 DHT11_ReadByte(void){
	u8 i, dat;
	for(i=0; i<8; i++){
		dat<<=1;
		dat|=DHT11_ReadBit();
	}
	return dat;
}

u8 DHT11_ReadData(u8 *h){
	u8 buf[5];
	u8 i;
	DHT11_RST();//������ʼ�ź�
	if(DHT11_Check()==0){//��鴫�����Ƿ���Ӧ
		for(i=0;i<5;i++){//һ�����������ݴ���Ϊ40bit, С�����������Ժ���չ���ֶ���Ϊ0
			buf[i]=DHT11_ReadByte();//���ݸ�ʽ��8bitʪ����������+8bitʪ��С������+8bit�¶���������+8bit�¶�С������+8bitУ���
		}
		if(buf[0]+buf[1]+buf[2]+buf[3]==buf[4]){//У��͵���8bitʪ����������+8bitʪ��С������+8bit�¶���������+8bit�¶�С���������ý����ĩ��λ
			*h=buf[0];
			h++;
			*h=buf[2];
		}
	}else {
		return 1;
	}
	return 0;
	

}







