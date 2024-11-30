#include "bkp.h"







//备份接口配置
void BKP_Configuration(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);//使能 PWR（电源接口）  和 BKP（备份接口）的时钟
	PWR_BackupAccessCmd(ENABLE);//使能对备份寄存器的访问
}



