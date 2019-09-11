#include "pwm.h"


void TIM3_Configurate(uint32_t arr, uint16_t psc)
{
	TIM_TimeBaseInitTypeDef tim3;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);   //TIM3
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3); //GPIOB¸
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM3); //GPIOB¸
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_TIM3); //GPIOB¸´
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_TIM3); //GPIOB¸

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;  //GPIOB
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //
	GPIO_Init(GPIOB,&GPIO_InitStructure);               //
	 
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  																	//
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_Period=arr;                       
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);                  


	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OCInitStructure.TIM_Pulse = 1500;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure); 
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);		//PB4

	TIM_OCInitStructure.TIM_Pulse = 1500;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure); 
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); 	//PB3

	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC3Init(TIM3, &TIM_OCInitStructure); 
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //PB0

	TIM_OCInitStructure.TIM_Pulse = 1500;
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //PB1
 
  TIM_ARRPreloadConfig(TIM3,ENABLE);

	TIM_Cmd(TIM3, ENABLE);  //ä½¿èƒ½TIM3
}

void gimbal_control(uint16_t pitch, uint16_t yaw)
{
	TIM_SetCompare1(TIM3,yaw);
	TIM_SetCompare2(TIM3,pitch);
}

void pwm_output(uint8_t channal,uint32_t pulse)
{
	
	if(pulse > MAX_PULSE)
		pulse  = MAX_PULSE;
	if(pulse < MIN_PULSE)
		pulse	 = MIN_PULSE;
	
}
