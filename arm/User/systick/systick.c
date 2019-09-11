#include "systick.h"


static __IO u32 TimingDelay;


//启动系统滴答定时器Systick
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms中断
	 * SystemFrequency / 100000	 10us
	 * SystemFrequency / 1000000 1us
	 */
	if (SysTick_Config(SystemCoreClock / 100000))
	{ 
		/* Capture error */ 
		while (1);
	}
}

//微秒延时函数
void Delay_us(__IO u32 nTime)
{ 
	TimingDelay = nTime;	

	while(TimingDelay != 0);
}

//获取节拍函数
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}

void SysTick_Handler(void)
{
	TimingDelay_Decrement();
}
