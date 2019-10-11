#include "uart4.h"

/*-----UART4_TX-----PC10-----*/
/*-----UART4_RX-----PC11-----*/

//uint8_t gimbal_data[10] = {0};
//uint8_t gimbal[10] = {0};
//extern uint8_t gimbal_send[10];
void UART4_Configuration(void)
{
	
		GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;
    USART_InitTypeDef usart;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

    gpio.GPIO_Pin                           =   GPIO_Pin_10 | GPIO_Pin_11;
    gpio.GPIO_Mode                          =   GPIO_Mode_AF;
    gpio.GPIO_OType                         =   GPIO_OType_PP;
    gpio.GPIO_Speed                         =   GPIO_Speed_100MHz;
    gpio.GPIO_PuPd                          =   GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &gpio);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);
    
    usart.USART_BaudRate                    =   115200;
    usart.USART_WordLength                  =   USART_WordLength_8b;
    usart.USART_StopBits                    =   USART_StopBits_1;
    usart.USART_Parity                      =   USART_Parity_No;
    usart.USART_Mode                        =   USART_Mode_Rx|USART_Mode_Tx;
    usart.USART_HardwareFlowControl         =   USART_HardwareFlowControl_None;
    USART_Init(UART4, &usart);

    nvic.NVIC_IRQChannel                    =   UART4_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority  =   1;
    nvic.NVIC_IRQChannelSubPriority         =   4;
    nvic.NVIC_IRQChannelCmd                 =   ENABLE; 
    NVIC_Init(&nvic);

    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
		
    USART_Cmd(UART4, ENABLE);
}

//void Get_Gimbal_Data(void)
//{
//	//memcpy(gimbal_data,0,10);		//先把数据清空
//	USART_SendData(UART4,0xAA);	//从USART4发出，发给云台的stm32，发帧头
//	Delay_us(10);
//	USART_SendData(UART4,0xAA);	//从USART4发出，发给云台的stm32，发帧头
//	Delay_us(100);
//}

//void Send_Gimbal(void)
//{
//	uint8_t i = 0;
//	for(i = 0; i < 10 ; i++)
//	{
//		USART_SendData(USART3,gimbal_send[i]);
//		Delay_us(10);
//	}
//}

void UART4_IRQHandler(void)
{
//	uint8_t Res;
//	static bool isGotFrameHeader = false;
//	static uint8_t frameHeaderCount = 0;
	//static uint8_t dataLength = 2;
//	static uint8_t dataCount = 0;
//	static uint8_t gimbal[10] = {0};
	if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //判断接收中断
	{
//		Res = USART_ReceiveData(UART4);//(USART1->DR);	//取出接收寄存器数据
//		if (!isGotFrameHeader) //判断帧头
//		{  
//			if (Res == 0xAA) 
//			{
//				frameHeaderCount++;
//				if (frameHeaderCount == 2)
//				{
//					frameHeaderCount = 0;
//					isGotFrameHeader = true;
//					dataCount = 1;
//				}
//			} 
//			else 
//			{
//				isGotFrameHeader = false;
//				dataCount = 2;
//				frameHeaderCount = 0;
//			}
//		}
//		if(isGotFrameHeader)
//		{
//			gimbal[dataCount] = Res;
//			dataCount = (dataCount+1)%10;
//		}
//		if(dataCount == 0 && isGotFrameHeader)
//		{
//			isGotFrameHeader = false;
//			gimbal[0] = 0xAA;
//			gimbal[1] = 0xAA;
//			//memcpy(gimbal_data,gimbal,10);
//		}
	}
}
