#include "motor.h"
extern uint8_t gripper_status;	//手爪状态

void gripper_init()
{
	GPIO_InitTypeDef gpio;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_Speed = GPIO_High_Speed;
	gpio.GPIO_OType = GPIO_OType_PP;
	
	gpio.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Init(GPIOC,&gpio);
	
	GPIO_Init(GPIOC,&gpio);
	
	//GPIO_SetBits(GPIOC,GPIO_Pin_2);
	//GPIO_SetBits(GPIOC,GPIO_Pin_3);			//1
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);
	GPIO_ResetBits(GPIOC,GPIO_Pin_3);
}

void grasp(void)
{
	PC2(1);
	PC3(0);
	TIM_SetCompare3(TIM3,20000);
	Delay_ms(500);
	gripper_stop();
}

void loose(void)
{
	PC2(0);
	PC3(1);
	TIM_SetCompare3(TIM3,20000);
	Delay_ms(500);
	gripper_stop();
}

void gripper_stop(void)
{
	PC2(0);
	PC3(0);
	TIM_SetCompare3(TIM3,0);
}

void gripper_control(void)
{
	if(gripper_status == 0x11)
		grasp();
	else if(gripper_status == 0x22)
		loose();
	else
		gripper_stop();
}
