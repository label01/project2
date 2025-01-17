#ifndef __SPI_H
#define __SPI_H

#include "sys.h"


#define SPI2PORT  GPIOB
#define SPI2_NSS  GPIO_Pin_12
#define SPI2_SCK  GPIO_Pin_13
#define SPI2_MISO GPIO_Pin_14
#define SPI2_MOSI GPIO_Pin_15




void SPI2_Init(void);
u8 SPI2_SendByte(u8 Byte);









#endif







































