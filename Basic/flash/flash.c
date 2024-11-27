#include "flash.h"




void FLASH_W(u32 addr, u16 data){
		FLASH_Unlock();
		FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//Clears all flags
		FLASH_ErasePage(addr);
		FLASH_ProgramHalfWord(addr, data); //��ָ����FLASHѡ���ֽڵ�ַ��д�����  ��Ϊд��������� u16�� �����u32�����FLASH_ProgramWord
		FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//Clears all flags
		FLASH_Lock();
}

u16 FLASH_R(u32 addr){
		u16 a;
		a = *(u16*)(addr);
		return a;

}

