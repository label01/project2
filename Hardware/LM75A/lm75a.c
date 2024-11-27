


#include "lm75a.h"




/*
����LM75A���¶�ֵ��-55~125���϶ȣ�
�¶�������0��1�������¶������� �¶�С�������λ�� ���η���*TempBuffer(ʮ����)
*/
void LM75A_GetTemp(u8 *TempBuffer){
	u8 buf[2]; //�洢�¶�ֵ
	u8 t=0, a=0;
	//I2C_READ_BUFFER(LM75A_ADD, 0x00, buf, 2);
	I2C_Read_Buffer(LM75A_ADD, 0x00, buf, 2);
	t=buf[0];
	*TempBuffer = 0; //�ж��¶�ֵ��0Ϊ��
	if(t&0x80){
		*TempBuffer = 1;//�¶�Ϊ��
		t=~t;//ȡ��
		t++;//��1 �õ�����
	}
	if(t&0x01){a=a+1;}
	if(t&0x02){a=a+2;}
	if(t&0x04){a=a+4;}
	if(t&0x08){a=a+8;}
	if(t&0x10){a=a+16;}
	if(t&0x20){a=a+32;}
	if(t&0x40){a=a+64;}
	TempBuffer++;
	*TempBuffer=a;
	//��ʼ����������������С������
	a=0;
	t=buf[1];
	if(t&0x20) {a=a+12;}
	if(t&0x40) {a=a+25;}
	if(t&0x80) {a=a+50;}
	TempBuffer++;
	*TempBuffer=a;
	
	
}




void LM75A_PowerDown(void){
	I2C_SEND_BYTE(LM75A_ADD, 0x01, 1);

}




















































