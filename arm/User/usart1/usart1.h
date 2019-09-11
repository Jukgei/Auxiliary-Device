#ifndef USART1_H
#define USART1_H

#include "stm32f4xx.h"
#include "bool.h"
#include "LobotSerialServo.h"
#include "systick.h"

void USART1_Configuration(void);
bool isRxCompleted(void);
void uartWriteBuf(uint8_t *buf, uint8_t len);
void SendPostion(uint8_t ID);

#endif

