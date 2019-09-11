#ifndef _UART4_H
#define _UART4_H

#include "stm32f4xx.h"
#include "bool.h"
#include "systick.h"


void UART4_Configuration(void);
void Get_Gimbal_Data(void);
void Send_Gimbal(void);

#endif
