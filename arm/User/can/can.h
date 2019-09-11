#ifndef _CAN_H
#define _CAN_H

#include "stm32f4xx.h"


void CAN1_Configuration(void);
void CAN_Output(int16_t roll,int16_t pitch, int16_t yaw);
#endif 
