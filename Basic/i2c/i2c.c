
#include "i2c.h"





void I2C_GPIO_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE); //����I2C
	GPIO_InitStructure.GPIO_Pin = I2C_SCL|I2C_SDA;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//I2C���߹淶����� ���ӵ����ߵ����������������©����·�򼯵缫��·
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2CPORT, &GPIO_InitStructure);
}




/*i2c��ʼ��*/
void I2C_Configuration(void){
	I2C_InitTypeDef I2C_InitStructure;
	I2C_GPIO_Init();
	
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C; //����I2CΪI2Cģʽ
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;//ֻ��ʱ��Ƶ�ʸ���100khz��������
  I2C_InitStructure.I2C_OwnAddress1 = HostAddress;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;//����Ӧ��
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; //7λ��ַģʽ
	I2C_InitStructure.I2C_ClockSpeed = BusSpeed;
	I2C_Init(I2C1, &I2C_InitStructure);
	I2C_Cmd(I2C1, ENABLE);
}


/*i2c�������ݴ�*/
void I2C_SEND_BUFFER(u8 SlaveAddr, u8 WriteAddr, u8* pBuffer, u16 NumByteToWrite){
	I2C_GenerateSTART(I2C1,ENABLE);//������ʼλ
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); //���EV5
	I2C_Send7bitAddress(I2C1,SlaveAddr,I2C_Direction_Transmitter);//����������ַ
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//���EV6
	I2C_SendData(I2C1,WriteAddr); //�ڲ����ܵ�ַ
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));//��λ�Ĵ����ǿգ����ݼĴ����ѿգ�����EV8���������ݵ�DR��������¼�
	while(NumByteToWrite--){ //ѭ����������	
		I2C_SendData(I2C1,*pBuffer); //��������
		pBuffer++; //����ָ����λ
		while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));//���EV8
	}
	I2C_GenerateSTOP(I2C1,ENABLE);//����ֹͣ�ź�

}



/*�������ݴ�*/
void I2C_Send_Buffer(u8 SlaveAddr, u8 WriteAddr, u8 *pBuffer, u16 NumByteToWrite){
	I2C_GenerateSTART(I2C1,ENABLE); //���Ϳ�ʼ�ź�
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); //�ȴ����	
	I2C_Send7bitAddress(I2C1,SlaveAddr, I2C_Direction_Transmitter); //���ʹ�������ַ��״̬��д�룩
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); //�ȴ����	
	I2C_SendData(I2C1,WriteAddr); //���ʹ������ڲ��Ĵ�����ַ
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)); //�ȴ����	
	while(NumByteToWrite--){
		I2C_SendData(I2C1, *pBuffer);
		pBuffer++;
		while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)); //�ȴ����
	}	
	I2C_GenerateSTOP(I2C1,ENABLE); //���ͽ����ź�
}






/*i2c����һ���ֽ�*/
void I2C_SEND_BYTE(u8 SlaveAddr,u8 WriteAddr,u8 pBuffer){
	I2C_GenerateSTART(I2C1,ENABLE); //���Ϳ�ʼ�ź�
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); //�ȴ����	
	I2C_Send7bitAddress(I2C1,SlaveAddr, I2C_Direction_Transmitter); //���ʹ�������ַ��״̬��д�룩
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); //�ȴ����	
	I2C_SendData(I2C1,WriteAddr); //���ʹ������ڲ��Ĵ�����ַ
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)); //�ȴ����	
	I2C_SendData(I2C1,pBuffer); //����Ҫд�������
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)); //�ȴ����	
	I2C_GenerateSTOP(I2C1,ENABLE); //���ͽ����ź�

}


/*����һ���ֽ�*/
void I2C_Send_Byte(u8 SlaveAddr, u8 WriteAddr, u8 pBuffer){
	I2C_GenerateSTART(I2C1, ENABLE); //����I2Cx����START����
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)); //�ȴ��ӻ�Ӧ��I2C_EVENT_MASTER_MODE_SELECT   ѭ���ȴ����豸ģʽ��
	I2C_Send7bitAddress(I2C1, SlaveAddr, I2C_Direction_Transmitter);//��ָ���Ĵ�I2C�豸���͵�ַ��
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//�ȴ��ӻ�Ӧ��  I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED 
	I2C_SendData(I2C1, WriteAddr);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(I2C1, pBuffer);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTOP(I2C1, ENABLE);
}




/*i2c��ȡ���ݴ�*/
void I2C_READ_BUFFER(u8 SlaveAddr,u8 readAddr,u8* pBuffer,u16 NumByteToRead){
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2C1,ENABLE);//�����ź�
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));	//��� EV5
	I2C_Send7bitAddress(I2C1,SlaveAddr, I2C_Direction_Transmitter); //д��������ַ
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//��� EV6
	I2C_Cmd(I2C1,ENABLE);
	I2C_SendData(I2C1,readAddr); //���Ͷ��ĵ�ַ
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)); //��� EV8
	I2C_GenerateSTART(I2C1,ENABLE); //�����ź�
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); //��� EV5
	I2C_Send7bitAddress(I2C1,SlaveAddr,I2C_Direction_Receiver); //��������ַ����������Ϊ��
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)); //���EV6
	while(NumByteToRead){
		if(NumByteToRead == 1){ //ֻʣ�����һ������ʱ���� if ���
			I2C_AcknowledgeConfig(I2C1,DISABLE); //�����һ������ʱ�ر�Ӧ��λ
			I2C_GenerateSTOP(I2C1,ENABLE);	//���һ������ʱʹ��ֹͣλ
		}
		if(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)){ //��ȡ����
			*pBuffer = I2C_ReceiveData(I2C1);//���ÿ⺯��������ȡ���� pBuffer
			pBuffer++; //ָ����λ
			NumByteToRead--; //�ֽ����� 1 
		}
	}
	I2C_AcknowledgeConfig(I2C1,ENABLE);
}



void I2C_Read_Buffer(u8 SlaveAddr, u8 ReadAddr, u8 *pBuffer, u16 NumByteToRead){
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));//���ָ����I2C��־λ�������, ����æ��־
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1, SlaveAddr, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_Cmd(I2C1, ENABLE);//ʹ��I2C��ȷ�����账��ʹ��״̬
	I2C_SendData(I2C1, ReadAddr);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1, SlaveAddr, I2C_Direction_Receiver);//ȷ�Ϸ���󣨶���,���������ӻ�
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	while(NumByteToRead){
		if(NumByteToRead == 1){
			I2C_AcknowledgeConfig(I2C1, DISABLE);
			I2C_GenerateSTOP(I2C1, ENABLE);
		}
		if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)){
			*pBuffer = I2C_ReceiveData(I2C1);
			pBuffer++;
			NumByteToRead--;
		}
	
	}
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	
}






/*i2c��ȡһ���ֽ�*/
u8 I2C_READ_BYTE(u8 SlaveAddr,u8 readAddr){
	u8 a;
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2C1,ENABLE);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1,SlaveAddr, I2C_Direction_Transmitter); 
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_Cmd(I2C1,ENABLE);
	I2C_SendData(I2C1,readAddr);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTART(I2C1,ENABLE);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1,SlaveAddr, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	I2C_AcknowledgeConfig(I2C1,DISABLE); //�����һ������ʱ�ر�Ӧ��λ
	I2C_GenerateSTOP(I2C1,ENABLE);	//���һ������ʱʹ��ֹͣλ
	a = I2C_ReceiveData(I2C1);
	return a;
}



u8 I2C_Read_Byte(u8 SlaveAddr, u8 ReadAddr){
	u8 a;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));//���ָ����I2C��־λ�������, ����æ��־
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1, SlaveAddr, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_Cmd(I2C1, ENABLE);//ʹ��I2C��ȷ�����账��ʹ��״̬
	I2C_SendData(I2C1, ReadAddr);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1, SlaveAddr, I2C_Direction_Receiver);//ȷ�Ϸ���󣨶���,���������ӻ�
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	I2C_AcknowledgeConfig(I2C1,  DISABLE); //ʹ�ܻ�ʧ��ָ��I2C��Ӧ����
	I2C_GenerateSTOP(I2C1, ENABLE);
	a = I2C_ReceiveData(I2C1);
	return a;
	

}






