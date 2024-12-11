

#include "oled0561.h"
#include "ASCII_8x16.h"

#include "CHS_16x16.h" //���뺺������ 
#include "PIC1.h" //����ͼƬ

void OLED_DISPLAY_OFF(void){
	u8 buf[2]={
	0xae,//����ʾ
	0x8d//VCC��Դ
	};
	I2C_Send_Buffer(OLED0561_ADD, COM, buf, 2);
}



void OLED_DISPLAY_CLEAR(void){
	u8 j, t;
	for(t=0xB0; t<0xB8; t++){ //ҳ��ַ��ʼΪ0xb0, һ����8ҳ��ÿҳ�ɿ���һ��
		I2C_Send_Byte(OLED0561_ADD, COM, t);//����ҳ��ַ
		I2C_Send_Byte(OLED0561_ADD, COM, 0x10);//������ʼ�и���λ
		I2C_Send_Byte(OLED0561_ADD, COM, 0x00);//������ʼ�е���λ
		for(j=0;j<132;j++){	//��ҳ�������
 			I2C_Send_Byte(OLED0561_ADD,DAT,0x00);
 		}
		
	}


}

void OLED_DISPLAY_ON (void){//OLED����ʼֵ���ò�����ʾ
	u8 buf[28]={
	0xae,//0xae:����ʾ��0xaf:����ʾ
    0x00,0x10,//��ʼ��ַ��˫�ֽڣ�       
	0xd5,0x80,//��ʾʱ��Ƶ�ʣ�
	0xa8,0x3f,//�����ʣ�
	0xd3,0x00,//��ʾƫ�ƣ�
	0XB0,//д��ҳλ�ã�0xB0~7��
	0x40,//��ʾ��ʼ��
	0x8d,0x14,//VCC��Դ
	0xa1,//���ö�����ӳ�䣿
	0xc8,//COM�����ʽ��
	0xda,0x12,//COM�����ʽ��
	0x81,0xff,//�Աȶȣ�ָ�0x81�����ݣ�0~255��255��ߣ�
	0xd9,0xf1,//������ڣ�
	0xdb,0x30,//VCC��ѹ���
	0x20,0x00,//ˮƽѰַ����
	0xa4,//0xa4:������ʾ��0xa5:�������
	0xa6,//0xa6:������ʾ��0xa7:��ɫ��ʾ
	0xaf//0xae:����ʾ��0xaf:����ʾ
	}; //
	I2C_Send_Buffer(OLED0561_ADD,COM,buf,28);
}




void OLED0561_Init(void){
	OLED_DISPLAY_OFF();
	OLED_DISPLAY_CLEAR();
	OLED_DISPLAY_ON();

}


void OLED_DISPLAY_LIT (u8 x){//OLED���������ã�0~255��
	I2C_Send_Byte(OLED0561_ADD,COM,0x81);
	I2C_Send_Byte(OLED0561_ADD,COM,x);//����ֵ
}



//��ʾӢ��������8*16��ASCII��
//ȡģ��СΪ16*16��ȡģ��ʽΪ�������Ҵ��ϵ��¡�������8���¸�λ��
void OLED_DISPLAY_8x16(u8 x, //��ʾӢ�ĵ�ҳ���꣨��0��7�����˴������޸ģ�
						u8 y, //��ʾӢ�ĵ������꣨��0��128��
						u16 w){ //Ҫ��ʾӢ�ĵı��
	u8 j,t,c=0;
	y=y+2; //��OLED������������оƬ�Ǵ�0x02����Ϊ��������һ�У�����Ҫ����ƫ����
	for(t=0;t<2;t++){
		I2C_Send_Byte(OLED0561_ADD,COM,0xb0+x); //ҳ��ַ����0xB0��0xB7��
		I2C_Send_Byte(OLED0561_ADD,COM,y/16+0x10); //��ʼ�е�ַ�ĸ�4λ ����λ��һ�й̶�Ϊ0x10�� ����λ�̶�Ϊ0x00 
		I2C_Send_Byte(OLED0561_ADD,COM,y%16);	//��ʼ�е�ַ�ĵ�4λ
		for(j=0;j<8;j++){ //��ҳ�������
 			I2C_Send_Byte(OLED0561_ADD,DAT,ASCII_8x16[(w*16)+c-512]);//Ϊ�˺�ASII���ӦҪ��512   ���������Ը�һ�ĸ�����⣺ ��w-32��*16 +c ,asii���һ���ַ���ʮ���Ʊ�ʾ����32 ��ˢ��һ����16λ
			c++;}
		x++; //ҳ��ַ��1
	}
}
//��LCM����һ���ַ���,����64�ַ�֮�ڡ�
//Ӧ�ã�OLED_DISPLAY_8_16_BUFFER(0," DoYoung Studio"); 
void OLED_DISPLAY_8x16_BUFFER(u8 row,u8 *str){
	u8 r=0;
	while(*str != '\0'){
		OLED_DISPLAY_8x16(row,r*8,*str++);
		r++;
    }	
}


//��ɫ��ʾӢ���ַ�
void INVERSE_OLED_DISPLAY_8x16(u8 x, //��ʾӢ��ҳ���꣨��0��7��
																u8 y,//��ʾӢ�ĵ������꣨��0~128��
																u16 w){//��ʾ��Ӣ�ı���
	u8 j, t, c =0;//��������
	y=y+2; //��OLED������������оƬ�Ǵ�0x02����Ϊ��������һ�У�����Ҫ����ƫ����
	for(t=0; t<2; t++){
		I2C_Send_Byte(OLED0561_ADD,COM,0xb0+x); //ҳ��ַ����0xB0��0xB7��
		I2C_Send_Byte(OLED0561_ADD,COM,y/16+0x10); //��ʼ�е�ַ�ĸ�4λ ����λ��һ�й̶�Ϊ0x10�� ����λ�̶�Ϊ0x00 
		I2C_Send_Byte(OLED0561_ADD,COM,y%16);	//��ʼ�е�ַ�ĵ�4λ
		for(j=0;j<8;j++){ //��ҳ�������
 			I2C_Send_Byte(OLED0561_ADD,DAT,~ASCII_8x16[(w*16)+c-512]);//Ϊ�˺�ASII���ӦҪ��512�� ��λȡ�����Ϳ���ʵ�ַ�ɫЧ��
			c++;//��¼ˢ����
		}
		x++; //ҳ��ַ��1
	}


}







/******************************������ʾ����***********************************************/

//��ʾ����16*16
//ȡģ��СΪ16*16��ȡģ��ʽΪ�������Ҵ��ϵ��¡�������8���¸�λ��
void OLED_DISPLAY_16x16(u8 x, //��ʾ���ֵ�ҳ���꣨��0xB0��0xB7��
			u8 y, //��ʾ���ֵ������꣨��0��128��
			u16 w){ //Ҫ��ʾ���ֵı��
	u8 j,t,c=0;
	y=y+2;
	for(t=0;t<2;t++){
		I2C_Send_Byte(OLED0561_ADD,COM,0xb0+x); //ҳ��ַ����0xB0��0xB7��
		I2C_Send_Byte(OLED0561_ADD,COM,y/16+0x10); //��ʼ�е�ַ�ĸ�4λ ������λ��һ�й̶�Ϊ0x10�� ����λ�̶�Ϊ0x00 
		I2C_Send_Byte(OLED0561_ADD,COM,y%16);	//��ʼ�е�ַ�ĵ�4λ
		for(j=0;j<16;j++){ //��ҳ�������
 			I2C_Send_Byte(OLED0561_ADD,DAT,GB_16[(w*32)+c]);
			c++;}x++; //ҳ��ַ��1
	}
	I2C_Send_Byte(OLED0561_ADD,COM,0xAF); //����ʾ 
}

//���� ��ɫ��ʾ
void INVERSE_OLED_DISPLAY_16x16(u8 x, //��ʾ���ֵ�ҳ���꣨��0xB0��0xB7��
			u8 y, //��ʾ���ֵ������꣨��0��128��
			u16 w){ //Ҫ��ʾ���ֵı��
	u8 j,t,c=0;//��������
				y=y+2;
	for(t=0;t<2;t++){
		I2C_Send_Byte(OLED0561_ADD,COM,0xb0+x); //ҳ��ַ����0xB0��0xB7��
		I2C_Send_Byte(OLED0561_ADD,COM,y/16+0x10); //��ʼ�е�ַ�ĸ�4λ ������λ��һ�й̶�Ϊ0x10�� ����λ�̶�Ϊ0x00 
		I2C_Send_Byte(OLED0561_ADD,COM,y%16);	//��ʼ�е�ַ�ĵ�4λ
		for(j=0;j<16;j++){ //��ҳ�������
 			I2C_Send_Byte(OLED0561_ADD,DAT,~GB_16[(w*32)+c]);
			c++;//��¼ˢ�µ��ڼ���
		}
		x++; //ҳ��ַ��1
	}
	I2C_Send_Byte(OLED0561_ADD,COM,0xAF); //����ʾ 
}



void OLED_DISPLAY_PIC1(void){ //��ʾȫ��ͼƬ
	u8 m,i;
	for(m=0;m<8;m++){//
		I2C_Send_Byte(OLED0561_ADD,COM,0xb0+m);
		I2C_Send_Byte(OLED0561_ADD,COM,0x10); //��ʼ�е�ַ�ĸ�4λ
		I2C_Send_Byte(OLED0561_ADD,COM,0x00);	//��ʼ�е�ַ�ĵ�4λ
		for(i=0;i<128;i++){//����128��ͼƬ��ʾ����
			I2C_Send_Byte(OLED0561_ADD,DAT,PIC1[i+m*128]);}
	}
}









