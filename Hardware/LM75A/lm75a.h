







#ifndef __LM75A_H
#define __LM75A_H


#include "sys.h"

#include "i2c.h"

#define LM75A_ADD 0x9e


void LM75A_GetTemp(u8 *TempBuffer);

void LM75A_PowerDown(void);










#endif
















































