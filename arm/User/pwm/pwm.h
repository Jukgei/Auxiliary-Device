#ifndef _PWM_H
#define _PWM_H

#include "stm32f4xx.h"


#define MAX_PULSE 2000
#define MIN_PULSE 1000

void TIM3_Configurate(uint32_t arr, uint16_t psc);
//void gimbal_control(uint16_t pitch, uint16_t yaw);



#endif
