

#include "spi.h"



void SPI2_Init(void){

	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//使能SPI2时钟
	
	
	GPIO_InitStructure.GPIO_Pin = SPI2_MISO; //spi2输入引脚 浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SPI2PORT, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = SPI2_MOSI|SPI2_SCK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//这里如果是推挽输出，ch376初始化不成功
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI2PORT, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = SPI2_NSS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//其实更推荐用复用推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(SPI2PORT, &GPIO_InitStructure);
	
	
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//时钟的预分频值
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;//时钟相位， 1表示在SCK的奇数沿边采样， 2表示偶数沿边采样
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_High; //空闲状态时SCK的状态， High为高电平， low为低电平
	SPI_InitStructure.SPI_CRCPolynomial=7; //CRC校验和的多项式， 根据数据格式，八位数据帧就采用CR6； 16位数据帧采用CRC16；
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex; //双线输入输出全双工模式；
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB; //高位在前，参考CH376数据手册说明，建议CPOL=CPHA=1  || CPOL=CPHA=0 ， MSB，高位在前
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;//因为我们只接一个spi从设备，所以只可以交给软件自动控制，如果是多个从设备，就得手动控制
	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE);
	
}




u8 SPI2_SendByte(u8 Byte){
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPI2, Byte);//往发送寄存器里写入要发送的数据
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET);//如果接收寄存器没有收到数据， 循环等待
	return SPI_I2S_ReceiveData(SPI2);
}






























