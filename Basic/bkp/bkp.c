#include "bkp.h"







//���ݽӿ�����
void BKP_Configuration(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);//ʹ�� PWR����Դ�ӿڣ�  �� BKP�����ݽӿڣ���ʱ��
	PWR_BackupAccessCmd(ENABLE);//ʹ�ܶԱ��ݼĴ����ķ���
}



