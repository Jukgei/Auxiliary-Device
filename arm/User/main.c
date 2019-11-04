#include "main.h"


uint8_t usart_buff[BUFF_SIZE];	//数据包buff
uint32_t time_tick_1ms;					//时基
uint32_t ultrasonic = 0;				//超声波计数、非高度

extern uint8_t gripper_status;	//手爪状态
extern uint16_t position[5];	//TX2传回的舵机位置
extern uint16_t time[5];			//TX2传回的舵机运动时间
//extern uint16_t gimbal[2];		//TX2传回的云台的控制量（PWM值） yaw\pitch

int main()
{
	memset(usart_buff, 0, BUFF_SIZE);
	SysTick_Init();
	time_tick_1ms = 0;
	TIM6_Configureation();						//system time base						
	//USART6_Configuration();						//imu of gimbal
	USART3_Configuration();  					//TX2
	//USART2_Configuration();						//ladar
	USART1_Configuration();						//servo arm
	gripper_init();										//
	TIM7_Configureation();						//ultransonic
	//TIM3_Configurate(20000-1,84-1);		//pwm: gimbal yaw and pitch; gripper;
	ultransonic_init();								//ultransonic
	Servo_Init();
	//UART4_Configuration();
	while(1)
	{
		if(time_tick_1ms%10 == 0) //100Hz,解码，控制云台，控制云台是否要放在100Hz？
		{
			decoder(usart_buff);								//解数据包
			//gimbal_control(gimbal[1],gimbal[0]);//控制摄像机云台
			
		}
		if(time_tick_1ms % 20 == 0) //50Hz 控制舵机、反馈信息。
		{
			ArmControl();
			ultransonic_start();
			signal_feedback();
			//SendDistance();
		}
//		if(time_tick_1ms % 50 == 0)	//20Hz 反馈高度信息给无人机。
//		{
//			
//			//SendPostion();
//		}
		if(time_tick_1ms % 100 == 0)//100Hz 控制爪子
		{
			gripper_control();
		}
	}
}

