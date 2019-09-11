#include "LobotSerialServo.h"
#include <stdarg.h>
#include <string.h>
#include "usart1.h"

#define LobotSerialWrite  uartWriteBuf


extern uint16_t position[5];	//TX2传回的舵机位置
extern uint16_t time[5];			//TX2传回的舵机运动时间
uint8_t LobotRxBuf[16];
	
	
void Servo_Init(void){
	GPIO_InitTypeDef gpio;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_Speed = GPIO_High_Speed;
	gpio.GPIO_OType = GPIO_OType_PP;
	
	gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Init(GPIOB,&gpio);
	
	GPIO_Init(GPIOB,&gpio);

	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);
}
	
uint8_t LobotCheckSum(uint8_t buf[])
{
  uint8_t i;
  uint16_t temp = 0;
  for (i = 2; i < buf[3] + 2; i++) {
    temp += buf[i];
  }
  temp = ~temp;
  i = (uint8_t)temp;
  return i;
}

void LobotSerialServoSetID(uint8_t oldID, uint8_t newID)
{
	uint8_t buf[7];
  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = oldID;
  buf[3] = 4;
  buf[4] = LOBOT_SERVO_ID_WRITE;
  buf[5] = newID;
  buf[6] = LobotCheckSum(buf);
  LobotSerialWrite(buf, 7);
}

void LobotSerialServoMove(uint8_t id, int16_t position, uint16_t time)
{
  uint8_t buf[10];
  if(position < 0)
    position = 0;
  if(position > 1000)
	position = 1000;
  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = id;
  buf[3] = 7;
  buf[4] = LOBOT_SERVO_MOVE_TIME_WRITE;
  buf[5] = GET_LOW_BYTE(position);
  buf[6] = GET_HIGH_BYTE(position);
  buf[7] = GET_LOW_BYTE(time);
  buf[8] = GET_HIGH_BYTE(time);
  buf[9] = LobotCheckSum(buf);
  LobotSerialWrite(buf, 10);
}

void LobotSerialServoUnload(uint8_t id)
{
  uint8_t buf[7];
  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = id;
  buf[3] = 4;
  buf[4] = LOBOT_SERVO_LOAD_OR_UNLOAD_WRITE;
  buf[5] = 0;
  buf[6] = LobotCheckSum(buf);
  LobotSerialWrite(buf, 7);
}

void LobotSerialServoLoad(uint8_t id)
{
  uint8_t buf[7];
  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = id;
  buf[3] = 4;
  buf[4] = LOBOT_SERVO_LOAD_OR_UNLOAD_WRITE;
  buf[5] = 1;
  buf[6] = LobotCheckSum(buf);
  LobotSerialWrite(buf, 7);
}

int LobotSerialServoReadPosition(uint8_t id)
{
  int ret;
  uint8_t buf[6];

  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = id;
  buf[3] = 3;
  buf[4] = LOBOT_SERVO_POS_READ;
  buf[5] = LobotCheckSum(buf);
	
	LobotSerialWrite(buf, 6);
	
	ret = LobotSerialMsgHandle();
  return ret;
}

int LobotSerialMsgHandle(void)
{
	int count = 50000;
	uint8_t cmd;
	int ret;
	
	while(!isRxCompleted())
	{
		count--;
		if(count < 0)
			return -2048;
	}
	
	if(LobotCheckSum(LobotRxBuf) != LobotRxBuf[LobotRxBuf[3]+2])
	{
		return -2049;
	}
	
	cmd = LobotRxBuf[4];
	switch(cmd)
	{
		case LOBOT_SERVO_POS_READ:
			ret = (int)BYTE_TO_HW(LobotRxBuf[6], LobotRxBuf[5]);
			return ret;
		default:
			break;
	}
	return 0;
}



void ArmControl(void)
{
	for(int i = 0; i < 5; i++)
		LobotSerialServoMove(i+1,position[i],time[i]);
}


