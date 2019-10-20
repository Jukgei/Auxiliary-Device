#include "usart3.h"
#include "stdio.h"
/*-----USART3_TX-----PB10-----*/
/*-----USART3_RX-----PB11-----*/

//static uint8_t _USART3_TX_BUFFER[USART3_DMA_TX_BUFFER_LEN];
//static uint8_t _USART3_RX_BUFFER[USART3_DMA_RX_BUFFER_LEN];
//static uint16_t receive_msg_size = 0;
//extern uint8_t usart_get;
extern uint32_t time_tick_1ms;
extern uint8_t usart_buff[BUFF_SIZE];
extern uint16_t distance;
extern uint16_t signal_strength;

uint8_t gripper_status = 0;	//手爪状态
//uint8_t feedback = 0;
uint16_t position[5] = {500, 500, 131, 141, 468};	//TX2传回的舵机位置
uint16_t time[5] = {3000, 3000, 3000, 3000, 3000};			//TX2传回的舵机运动时间
uint16_t gimbal[2] = {0};		//TX2传回的云台的控制量（PWM值）
extern uint8_t gimbal_data[10]; //云台的STM32传回来的yaw/pitch
//bool isfeedback = false;

void USART3_Configuration(void)
{
    GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;
    //DMA_InitTypeDef   dma;
    USART_InitTypeDef usart;
		
//		memset(desired_value, 0, 3);
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,  ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,   ENABLE);

    gpio.GPIO_Pin                           =   GPIO_Pin_10 | GPIO_Pin_11;
    gpio.GPIO_Mode                          =   GPIO_Mode_AF;
    gpio.GPIO_OType                         =   GPIO_OType_PP;
    gpio.GPIO_Speed                         =   GPIO_Speed_100MHz;
    gpio.GPIO_PuPd                          =   GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &gpio);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

    USART_DeInit(USART3);
    USART_StructInit(&usart);
    usart.USART_BaudRate                    =   115200;
    usart.USART_WordLength                  =   USART_WordLength_8b;
    usart.USART_StopBits                    =   USART_StopBits_1;
    usart.USART_Parity                      =   USART_Parity_No;
    usart.USART_Mode                        =   USART_Mode_Tx | USART_Mode_Rx;
    usart.USART_HardwareFlowControl         =   USART_HardwareFlowControl_None;
    USART_Init(USART3, &usart);


    // nvic
    nvic.NVIC_IRQChannel                    =   USART3_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority  =   0;
    nvic.NVIC_IRQChannelSubPriority         =   0;
    nvic.NVIC_IRQChannelCmd                 =   ENABLE; 
    NVIC_Init(&nvic);


		USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
    USART_Cmd(USART3, ENABLE); 
}


/***
 * 接受数据包，并存放到usart_buff
 */
void USART3_IRQHandler(void)
{
		uint8_t ucTemp;
		static uint8_t i = 0;
		static uint32_t time_recoder = 0;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
			if(abs(time_tick_1ms-time_recoder)>10)
				i = 0;
			time_recoder = time_tick_1ms;

			ucTemp = USART_ReceiveData(USART3);
			usart_buff[i] = ucTemp;
			i = i + 1;
			if(i == 25) 
			{
				i = 0;
//				isfeedback = true;
			}
    }
}

//解从TX2发过来的数据包
void decoder(uint8_t * buff)
{
	if(buff[0] == 0xDD && buff[1] == 0xDD && (buff[23] == 0x22) && buff[24] == 0x22)
	{
		int i = 0, j = 0;
		for(i = 0, j = 2; i < 5; i++, j+=4)
			position[i] = BYTE_TO_HW(buff[j+1],buff[j]);
		for(i = 0, j = 4; i < 5; i++, j+=4)
			time[i] = BYTE_TO_HW(buff[j+1],buff[j]);
		gripper_status = buff[22];
//		gimbal[0] = BYTE_TO_HW(buff[24],buff[23]);
//		gimbal[1] = BYTE_TO_HW(buff[26],buff[25]);
//		if(isfeedback)
//		{
//			feedback = buff[27];
//		}
	}
}
//向TX-2发送雷达测距
void SendDistance(void)
{
	//如果信号可信，则发距离，先发第八位再发高八位
	if(signal_strength > 100 && signal_strength < 65535)
	{
		USART_SendData(USART3,0xEE);
		Delay_us(10);
		USART_SendData(USART3,0xEE);
		Delay_us(10);
		USART_SendData(USART3,(uint8_t)(distance));
		Delay_us(10);
		USART_SendData(USART3,(uint8_t)((distance) >> 8));
		Delay_us(10);
	}
	else//信号不可信，发0xffff
	{
		USART_SendData(USART3,0xEE);
		Delay_us(10);
		USART_SendData(USART3,0xEE);
		Delay_us(10);
		USART_SendData(USART3,0xff);
		Delay_us(10);
		USART_SendData(USART3,0xff);
		Delay_us(10);
	}
}


//void signal_feedback(void)
//{
//	if((feedback & 0x01) == 0x01)	//第0位为1返回高度
//	{
//		SendDistance();	
//		feedback &= 0xFE;
//	}
//	if((feedback & 0x02) == 0x02) //第1位为1返回1号舵机位置
//	{
//		SendPostion(1);
//		feedback &= 0xFD;
//	}
//	if((feedback & 0x04) == 0x04) //第2位为1返回2号舵机位置
//	{
//		SendPostion(2);
//		feedback &= 0xFB;
//	}
//	if((feedback & 0x08) == 0x08) //第3位为1返回3号舵机位置
//	{
//		SendPostion(3);
//		feedback &= 0xF7;
//	}
//	if((feedback & 0x10) == 0x10) //第4位为1返回4号舵机位置
//	{
//		SendPostion(4);
//		feedback &= 0xEF;
//	}
//	if((feedback & 0x20) == 0x20) //第5位为1返回5号舵机位置
//	{
//		SendPostion(5);
//		feedback &= 0xDF;
//	}
//	if((feedback & 0x40) == 0x40) //第6位为1返回yaw角和pitch角度，由于无法修改云台程序、故删去。应该继续使用
//	{
//		Send_Gimbal();
//		feedback &= 0xBF;
//	}
//	isfeedback = false;
//}

void signal_feedback(void)
{
	uint8_t i;
	for(i = 1; i <= 5 ; i++)
		SendPostion(i);
	SendDistance();
}

int fputc(int ch, FILE *f)
{
    while (USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);
    USART_SendData(USART3, (uint8_t)ch);
    return ch;
}



