#include "ultransonic.h"

extern uint32_t ultrasonic;
extern uint16_t distance;
float height = 0;
bool test_high = false;

void ultransonic_init()
{
	GPIO_InitTypeDef       gpio;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	gpio.GPIO_Pin                           =   GPIO_Pin_1;
  gpio.GPIO_Mode                          =   GPIO_Mode_OUT;
	gpio.GPIO_Speed													=   GPIO_High_Speed;
	gpio.GPIO_OType													=		GPIO_OType_PP;
	//gpio.GPIO_PuPd													=		GPIO_PuPd_NOPULL;										
	GPIO_Init(GPIOE, &gpio);																								//PE1 -> Trig
	
	gpio.GPIO_Pin                           =   GPIO_Pin_0;
  gpio.GPIO_Mode                          =   GPIO_Mode_IN;
	gpio.GPIO_Speed													=   GPIO_High_Speed;
	//gpio.GPIO_PuPd													=		GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOE, &gpio);																								//PE0 -> Echo
	
	

	EXTI_InitTypeDef EXTI_InitStructure;
 
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource0);
	EXTI_ClearITPendingBit(EXTI_Line0);
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	
	//EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
 

	/* Configures the nested vectored interrupt controller. */
	NVIC_InitTypeDef NVIC_InitStructure;
 
	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	Trig(0);
	
}
void ultransonic_delay()
{
	int i = 50;
	while(i--);
}
void ultransonic_start()
{
	Trig(0);
	ultransonic_delay();
	Trig(1);
	//delay_10us();
	ultransonic_delay();
	Trig(0);
	test_high = true;
	//wait EXTI0_IRQHandler
}
void EXTI0_IRQHandler(void)	
{
	
	EXTI_InitTypeDef EXTI_InitStructure;
	if( EXTI_GetITStatus(EXTI_Line0) != RESET )
	{
		
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0))
		{
			EXTI_ClearITPendingBit(EXTI_Line0);
			TIM_Cmd(TIM7, ENABLE);
			

			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource0);
			EXTI_ClearITPendingBit(EXTI_Line0);
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	
			//EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
			EXTI_InitStructure.EXTI_Line = EXTI_Line0;
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;
			EXTI_Init(&EXTI_InitStructure);
		}
		else
		{
			TIM_Cmd(TIM7, DISABLE);
			
			height = ((float)ultrasonic*0.0001 * 340.0)/2.0;					//100us = 0.0001s
			distance = (uint16_t)(height*1000);
			//printf("height is :%f\n",height);			
			ultrasonic = 0;
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource0);
			EXTI_ClearITPendingBit(EXTI_Line0);
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	
			//EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
			EXTI_InitStructure.EXTI_Line = EXTI_Line0;
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;
			EXTI_Init(&EXTI_InitStructure);
			
		}
	
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}


