
#include "nvic.h"
#include "encoder.h"

u8 INT_MARK=0;//中断标志位
u8 encoder_key;//存储编码器键值

//系统延时，这个并不精确只是一个大概的值
void NVIC_Delay_Us(u16 time){
	u16 i=0;
	while(time--){
		i=10;//自己定义
		while(i--);
	}
}


//系统延时，这个并不精确只是一个大概的值
void NVIC_Delay_Ms(u16 time){
	u16 i=0;
	while(time--){
		i=12000;//自己定义
		while(i--);
	}
}


void KEYPAD4x4_INT_INIT(void){
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//启动GPIO时钟，需要于复用时钟一同启动
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //配置端口中断需要启用复用时钟
	
	//第一个中断
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;//中断模式为 中断
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI4_IRQn; //中断线
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2; //子优先级 2
	NVIC_Init(&NVIC_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);
	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;//中断模式为 中断
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn; //中断线
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2; //子优先级 2
	NVIC_Init(&NVIC_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
	EXTI_InitStructure.EXTI_Line=EXTI_Line6;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;//中断模式为 中断
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn; //中断线
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2; //子优先级 2
	NVIC_Init(&NVIC_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7);
	EXTI_InitStructure.EXTI_Line=EXTI_Line7;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;//中断模式为 中断
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn; //中断线
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2; //子优先级 2
	NVIC_Init(&NVIC_InitStructure);

}


//编码器中断初始化
void ENCODER_INT_INIT(void){
	NVIC_InitTypeDef NVIC_InitStructure;//定义一个NVIC_InitTypeDef结构体变量，用于配置中断控制器的参数
	EXTI_InitTypeDef EXTI_InitStructure;//定义一个EXTI_InitTypeDef结构体变量，用于配置中断的参数
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//启动GPIO时钟，需要于复用时钟一同启动
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //配置端口中断需要启用复用时钟
	
	//第一个中断
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);//选择要配置的中断的端口和引脚
	EXTI_InitStructure.EXTI_Line=EXTI_Line6;//配置中断线为EXTI_Line6，这里对应PA6
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;//使能中断线
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;//设置中断模式为中断模式
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//设置中断触发方式为下降沿触发，当PA6引脚的电平从高到低变化时，将触发中断
	EXTI_Init(&EXTI_InitStructure);//调用EXTI_Init函数，根据EXTI_InitStructure的配置初始化外部中断
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn;//设置中断线EXTI9_5_IRQn，这对应外部中断线9-5
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//使能中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//设置抢占优先级，抢占优先级决定了能否打断其他正在执行的中断服务程序
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2; //设置子优先级 2，子优先级决定了在相同抢占优先级的中断中，哪个中断会先被执行
	NVIC_Init(&NVIC_InitStructure);//调用NVIC_Init函数，根据NVIC_InitStructure的配置初始化中断控制器
	
		//第二个中断
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7);//选择要配置的中断的端口和引脚
	EXTI_InitStructure.EXTI_Line=EXTI_Line7;//配置中断线为EXTI_Line7，这里对应PA7
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;//使能中断线
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;//设置中断模式为中断模式
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//设置中断触发方式为下降沿触发，当PA7引脚的电平从高到低变化时，将触发中断
	EXTI_Init(&EXTI_InitStructure);//调用EXTI_Init函数，根据EXTI_InitStructure的配置初始化外部中断
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn;//设置中断线EXTI9_5_IRQn，这对应外部中断线9-5
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//使能中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//设置抢占优先级，抢占优先级决定了能否打断其他正在执行的中断服务程序
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2; //设置子优先级 2，子优先级决定了在相同抢占优先级的中断中，哪个中断会先被执行
	NVIC_Init(&NVIC_InitStructure);//调用NVIC_Init函数，根据NVIC_InitStructure的配置初始化中断控制器
	
}




//void  EXTI4_IRQHandler(void){
//	if(EXTI_GetITStatus(EXTI_Line4)!=RESET){//判断某个线上的中断是否发生 
//		INT_MARK=1;//标志位置1，表示有按键中断
//		EXTI_ClearITPendingBit(EXTI_Line4);   //清除 LINE 上的中断标志位
//	}     
//}

void  EXTI9_5_IRQHandler(void){    
	if(EXTI_GetITStatus(EXTI_Line6)!=RESET){//判断某个线上的中断是否发生 
		NVIC_Delay_Us(100);//延时100微秒
		encoder_key=GPIO_ReadInputDataBit(ENCODER_PORT_B, ENCODER_R);//读取编码器右键值
		NVIC_Delay_Ms(3);//延时3毫秒
		if(!GPIO_ReadInputDataBit(ENCODER_PORT_A, ENCODER_L)){
			if(encoder_key==0) INT_MARK=1;//左转
			if(encoder_key==1) INT_MARK=2;//右转
			u16 a=0;//判断锁死计时
			while(!GPIO_ReadInputDataBit(ENCODER_PORT_A, ENCODER_L) && a<60000){//等待旋钮放开，同时判断是否锁死
				a++;
				NVIC_Delay_Us(20);
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line6);   //清除 LINE 上的中断标志位
	}     
	if(EXTI_GetITStatus(EXTI_Line7)!=RESET){//判断某个线上的中断是否发生 
		NVIC_Delay_Ms(20);//延时20ms
		if(!GPIO_ReadInputDataBit(ENCODER_PORT_A, ENCODER_D)) INT_MARK=3;//判断按键是否按下
		EXTI_ClearITPendingBit(EXTI_Line7);   //清除 LINE 上的中断标志位
	}     
}



























































