#ifndef _USART3_H
#define _USART3_H

#include "stm32f4xx.h"
#include "motor.h"
#include "LobotSerialServo.h"
#include "usart1.h"
#include "uart4.h"

#define USART3_DMA_TX_BUFFER_LEN 150u
#define USART3_DMA_RX_BUFFER_LEN 50u

#define BUFF_SIZE					25

void USART3_Configuration(void);
void decoder(uint8_t * buff);
void SendDistance(void);
//void display(float data1,float data2);
void signal_feedback(void);

#endif



