#include "timer.h"

extern uint32_t time_tick_1ms;
extern uint32_t ultrasonic;

void TIM6_Configureation(void) 			//ÓÃÓÚÏµÍ³Ê±»ù1ms
{
	  TIM_TimeBaseInitTypeDef  tim;
    NVIC_InitTypeDef         nvic;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
    
    tim.TIM_Prescaler                       =   84 - 1;
    tim.TIM_Period                          =   1000;
    tim.TIM_CounterMode                     =   TIM_CounterMode_Up;
    tim.TIM_ClockDivision                   =   TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM6, &tim);
    
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    nvic.NVIC_IRQChannel                    =   TIM6_DAC_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority  =   1;
    nvic.NVIC_IRQChannelSubPriority         =   1;
    nvic.NVIC_IRQChannelCmd                 =   ENABLE;
    NVIC_Init(&nvic);

    TIM_Cmd(TIM6, ENABLE);
    
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);
}

void TIM7_Configureation(void)		//ÓÃÓÚ³¬Éù²¨ 100us
{
	  TIM_TimeBaseInitTypeDef  tim;
    NVIC_InitTypeDef         nvic;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
    
    tim.TIM_Prescaler                       =   84 - 1;
    tim.TIM_Period                          =   100;
    tim.TIM_CounterMode                     =   TIM_CounterMode_Up;
    tim.TIM_ClockDivision                   =   TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM7, &tim);
    
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    nvic.NVIC_IRQChannel                    =   TIM7_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority  =   1;
    nvic.NVIC_IRQChannelSubPriority         =   1;
    nvic.NVIC_IRQChannelCmd                 =   ENABLE;
    NVIC_Init(&nvic);

    TIM_Cmd(TIM7, DISABLE);		//ÏÈ²»´ò¿ª
    
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
    TIM_ClearFlag(TIM7, TIM_FLAG_Update);
}

void TIM7_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET) 
    {
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
        TIM_ClearFlag(TIM7, TIM_FLAG_Update);
				//è¶…å£°æ³¢è®¡æ•°
				ultrasonic++;

    }
}

void TIM6_DAC_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) 
    {
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
        TIM_ClearFlag(TIM6, TIM_FLAG_Update);
				time_tick_1ms++ ;
				if(time_tick_1ms == 2000)
					time_tick_1ms = 0;
				
    }
}



