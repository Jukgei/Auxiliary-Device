#ifndef ULTRANSONIC_H
#define ULTRANSONIC_H


#include "stm32f4xx.h"
#include "bool.h"


void ultransonic_init(void);
void ultransonic_start(void);
void ultransonic_delay(void);

#define Trig(a) if(a) \
					GPIO_SetBits(GPIOE,GPIO_Pin_1);\
					else \
					GPIO_ResetBits(GPIOE,GPIO_Pin_1)	


#endif
