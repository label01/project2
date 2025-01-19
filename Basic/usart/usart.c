












#include "usart.h"
#include "buzzer.h"

//使USART串口可用printf函数发送
//在usart.h文件里可更换printf函数的串口号
#if 1
#pragma import(__use_no_semihosting)
struct __FILE{
	int handle;
};

FILE __stdout;

//空操作
int _sys_exit(int x){
	x = x;
}

//重定义fpuc函数
int fputc(int ch, FILE *f){
	while((USART_n->SR&0x40)==0);//循环发送，知道发送完毕
	USART_n->DR = (u8) ch;
	return ch;
}




#endif

/*USART1 串口相关程序*/

#if EN_USART1 
u8 USART1_RX_BUF[USART1_REC_LEN];

u16 USART1_RX_STA=0;


/* 串口1的引脚 PA9 PA10 */
void USART1_Init(u32 bound){
	//先配置GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	//使能或失能APB2外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1, ENABLE);
	//USART1_RX PA9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//USART1_TX PA10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ; //浮空输入
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//配置中断
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; //串口1中断
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能IRQ通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //子优先级
	NVIC_Init(&NVIC_InitStructure);
	
	//USART1 初始化设置
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
	USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶校验位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //一个停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长为8位数据格式
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE, ENABLE); //USART_IT_RXNE: Receive Data register not empty interrupt
	USART_Cmd(USART1, ENABLE); //使能串口
	
}



	//以下是字符串接收到USART_RX_BUF[]的程序，(USART_RX_STA&0x3FFF)是数据的长度（不包括回车）
	//当(USART_RX_STA&0xC000)为真时表示数据接收完成，即超级终端里按下回车键。
	//在主函数里写判断if(USART_RX_STA&0xC000)，然后读USART_RX_BUF[]数组，读到0x0d 0x0a即是结束。
	//注意在主函数处理完串口数据后，要将USART_RX_STA清0
void USART1_IRQHandler(void){ //串口1中断服务程序（固定的函数名不能修改）	
	uint8_t Res; //顶一个变量用于接收数据
	if(USART_GetFlagStatus(USART1, USART_IT_RXNE) != RESET){//判断串口接收数据标志位非空
		Res = USART_ReceiveData(USART1); // 读取接收的数据
		printf("%c", Res); //把输入的数据打印在超级终端上，要不没有显示
		//开始处理接收数据
		/*
		USART1_RX_STA 是一个无符号的十六位全局变量
		bit15，	接收完成标志
		bit14，	接收到0x0d
		bit13~0，	接收到的有效字节数目
		*/
		if((USART1_RX_STA&0x8000) == 0){ //接收未完成
			if(USART1_RX_STA&0x4000){//接收到0x0d也就是回车字符
				if(Res != 0x0a)USART1_RX_STA=0; // 接收错误，0x0a是换行符，如果在接收到回车符之后不是换行符那就错误了，重新开始
				else USART1_RX_STA |= 0x8000; //给最高位置1，表示接收完成
			}else{//没有收到回车
				if(Res==0x0d)USART1_RX_STA|=0x4000;//给标志位置1
				else{
					USART1_RX_BUF[USART1_RX_STA&0x3fff] = Res;
					USART1_RX_STA++;//数据长度加1
					if(USART1_RX_STA > (USART1_REC_LEN-1))USART1_RX_STA = 0;// 接收长度大于缓冲区，重新开始接收
				}
				
			}
		}
	
	}
} 
#endif	


/*
USART2串口相关程序
*/
#if EN_USART2   //USART2使用与屏蔽选择
u8 USART2_RX_BUF[USART2_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART2_RX_STA=0;       //接收状态标记	  

/*
USART2专用的printf函数
当同时开启2个以上串口时，printf函数只能用于其中之一，其他串口要自创独立的printf函数
调用方法：USART2_printf("123"); //向USART2发送字符123
*/
void USART2_printf (char *fmt, ...){ 
	char buffer[USART2_REC_LEN+1];  // 数据长度
	u8 i = 0;	
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, USART2_REC_LEN+1, fmt, arg_ptr);
	while ((i < USART2_REC_LEN) && (i < strlen(buffer))){
        USART_SendData(USART2, (u8) buffer[i++]);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); 
	}
	va_end(arg_ptr);
}


void USART2_Init(u32 bound){ //串口1初始化并启动
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
		 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); //使能UART2所在GPIOA的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //使能串口的RCC时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //设置USART2的RX接口是PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //设置USART2的TX接口是PA2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART2, &USART_InitStructure); //初始化串口
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启ENABLE/关闭DISABLE中断
    USART_Cmd(USART2, ENABLE);                    //使能串口 
   //Usart2 NVIC 配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器 
}

void USART2_IRQHandler(void){ //串口2中断服务程序（固定的函数名不能修改）	

} 
#endif	


#if EN_USART3   //如果使能了接收
u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART3_RX_STA=0;       //接收状态标记	  

/*
USART3专用的printf函数
当同时开启2个以上串口时，printf函数只能用于其中之一，其他串口要自创独立的printf函数
调用方法：USART3_printf("123"); //向USART3发送字符123
*/
void USART3_printf (char *fmt, ...){ 
	char buffer[USART3_REC_LEN+1];  // 数据长度
	u8 i = 0;	
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, USART3_REC_LEN+1, fmt, arg_ptr);
	while ((i < USART3_REC_LEN) && (i < strlen(buffer))){
        USART_SendData(USART3, (u8) buffer[i++]);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); //USART_FLAG_TC 发送完成标志位
	}
	va_end(arg_ptr);
}

void USART3_Init(u32 BaudRate){ //USART3初始化并启动
   GPIO_InitTypeDef GPIO_InitStructure;
   USART_InitTypeDef USART_InitStructure;
   NVIC_InitTypeDef NVIC_InitStructure; 

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); //使能UART3所在GPIOB的时钟
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //使能串口的RCC时钟

   //串口使用的GPIO口配置
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//设置USART3的RX接口是PB11
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//接口模式 浮空输入
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//设置USART3的TX接口是PB10
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输出速度50MHz
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//接口模式 复用推挽输出
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   //配置串口
   USART_InitStructure.USART_BaudRate = BaudRate;
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
   USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
   USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

		//串口中断配置
   NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//允许USART3中断
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//中断等级
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);

   USART_Init(USART3, &USART_InitStructure);//配置串口3
   USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//使能串口接收中断  
   //USART_ITConfig(USART3, USART_IT_TXE, ENABLE);//串口发送中断在发送数据时开启
   USART_Cmd(USART3, ENABLE);//使能串口3
}

//串口3中断服务程序（固定的函数名不能修改）
void USART3_IRQHandler(void){ 	
	u8 Res;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET){  //接收中断
				
        Res =USART_ReceiveData(USART3);//读取接收到的数据
        if(Res=='S'){//判断数据是否是STOP（省略读取S）            
            USART3_RX_STA=1;//如果是STOP则标志位为1      
        }else if(Res=='K'){//判断数据是否是OK（省略读取K）            
            USART3_RX_STA=2;//如果是OK则标志位为2      
        }
    }
}
#endif	

















