








#include "rtc.h"
#include "usart.h"

//����2��ȫ�ֱ���--����RTCʱ��Ķ�ȡ
u16 ryear; //4λ��
u8 rmon,rday,rhour,rmin,rsec,rweek;//2λ����ʱ������



void RTC_First_Config(void){ //�״�����RTC������
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); //����PWR��BKP��ʱ��
	PWR_BackupAccessCmd(ENABLE);
	BKP_DeInit();
	RCC_LSEConfig(RCC_LSE_ON); //�ⲿ32.768Khz��������
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET); // ���RCC�ı�־λ�Ƿ�����RCC_FLAG_LSERDY��LSE�����ⲿʱ��Դ
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); // RTCʱ�����óɵ����ⲿʱ��ԴLSE
	RCC_RTCCLKCmd(ENABLE); //����RTC
	RTC_WaitForSynchro(); //��������Ҫ�ȴ�APB1ʱ����RTCʱ��ͬ���� ���ܶ�д�Ĵ���
	RTC_WaitForLastTask(); //��д�Ĵ���ǰ��Ҫȷ����һ�������Ѿ�����
	RTC_SetPrescaler(32767); //����RTC��Ƶ����ʹRTCʱ��Ϊ1Hz
	RTC_WaitForLastTask();// �ȴ��Ĵ���д�����
}



/*ʵʱʱ�ӳ�ʼ��*/
void RTC_Config(void){
	if(BKP_ReadBackupRegister(BKP_DR1) != 0xa5a5){
		RTC_First_Config(); //��������rtc
		BKP_WriteBackupRegister(BKP_DR1, 0xa5a5);
	}else{
		/*�󱸼Ĵ���û�е��磬 ������������RTC�� ��ʼ�жϱ��θ�λ����*/
		if(RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET){
			//�ϵ縴λ
		}
		else if(RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET){
			//�����ϵ縴λ
		}
		RCC_ClearFlag();
		RCC_RTCCLKCmd(ENABLE);
		RTC_WaitForSynchro();
	}

}

/*
* �ж��Ƿ������꺯��
* �·� 1		2		3		4		5		6		7		8		9		10		11		12
* ���� 31		29	31	30	31	30	31	31	30	31		30		31
*������31		28	31	30	31	30	31	31	30	31		30		31
*����:���
*���:������ǲ������꣬ 1����. 0�� ����	
*/
u8 Is_Leap_Year(u16 year){
	if(year%4==0){
		if(year%100==0){
			if(year%400==0)return 1;
			else return 0;
		}else return 1;
	}else return 0;
}



u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�




/*
*�������ռ������ڣ�ֻ����1970-2099��
*/
u8 RTC_Get_Week(u16 year,u8 month,u8 day){
	if(month<1||month>12){
		//�·���Ч�����ش����룬����255
		return 255;
	}
	u16 temp2;
	u8 yearH, yearL;
	yearH = year/100;
	yearL = year%100;
	if(yearH>19)yearL+=100;
	temp2 = yearL+yearL/4;
	temp2 = temp2%7;
	temp2= temp2+day+table_week[month-1];
	if(yearL%4==0&&month<3)temp2--;
	return(temp2%7);
}


/*
*���չ�ʽ����������
*/
u8 RTC_Zeller_Week(u16 year, u8 month, u8 day){
	if(month<1||month>12){
		//�·���Ч�����ش����룬����255
		return 255;
	}
	if(month==1||month==2) month+=12, --year;
	return ((day+2*month+3*(month+1)/5+year+year/4-year/100+year/400+1)%7+7)%7;
}





const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};//ƽ����·����ڱ�

/**
  *����ʱ��
  *����ֵ��0�� �ɹ�
	*�������������
**/
u8 RTC_Get(void){
	static u16 daycnt=0;
	u32 timecount=0;
	u32 temp=0;
	u16 temp1=0;
	timecount=RTC_GetCounter();
	temp=timecount/86400; //�õ���������������Ӧ��
	if(daycnt!=temp){
		daycnt=temp;
		temp1=1970;
		while(temp>=365){
			if(Is_Leap_Year(temp1)){//������
				if(temp>=366)temp-=366;//���������
				else{temp1++;break;}
			}
			else temp-=365;
			temp1++;
		}
		ryear=temp1;
		temp1=0;
		while(temp>=28){//����һ����
			if(Is_Leap_Year(ryear)&&temp1==1){
				if(temp>=29)temp-=29;
				else break;
			}else{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];
				else break;
			}
			temp1++;
		}
		rmon=temp1+1;
		rday=temp+1;
	}
	temp=timecount%86400;
	rhour=temp/3600;
	rmin=(temp%3600)/60;
	rsec=(temp%3600)%60;
	//rweek=RTC_Get_Week(ryear, rmon, rday);
	rweek=RTC_Zeller_Week(ryear, rmon, rday);
	return 0;

}


/*
*д��ʱ��
*
*/
u8 RTC_Set(u16 syear, u8 smon, u8 sday, u8 hour, u8 min, u8 sec){
	u16 t;
	u32 seccount=0;
	if(syear<2000||syear>2099)return 1;
	for(t=1970;t<syear;t++){
		if(Is_Leap_Year(t))seccount+=31622400;
		else seccount+=31536000;
	}
	smon-=1;
	for(t=0;t<smon;t++){
		seccount+=(u32)mon_table[t]*86400;
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;
	}
	seccount+=(u32)(sday-1)*86400;
	seccount+=(u32)hour*3600;
	seccount+=(u32)min*60;
	seccount+=sec;
	RTC_First_Config();
	BKP_WriteBackupRegister(BKP_DR1, 0xa5a5);
	RTC_SetCounter(seccount);
	RTC_WaitForLastTask();
	return 0;
}









