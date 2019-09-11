#include "usart2.h"

/*-----USART2_TX-----PA2-----*/
/*-----USART2_RX-----PA3-----*/

uint16_t distance = 0;
uint16_t signal_strength = 0;
uint8_t radar[7];
//float tempeture; //原来用于返回雷达温度

void USART2_Configuration(void)
{
    GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;
    //DMA_InitTypeDef   dma;
    USART_InitTypeDef usart;
		
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,  ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 ,ENABLE);
	
    gpio.GPIO_Pin                           =   GPIO_Pin_2 | GPIO_Pin_3;
    gpio.GPIO_Mode                          =   GPIO_Mode_AF;
    gpio.GPIO_OType                         =   GPIO_OType_PP;
    gpio.GPIO_Speed                         =   GPIO_Speed_100MHz;
    gpio.GPIO_PuPd                          =   GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &gpio);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

    USART_DeInit(USART2);
    USART_StructInit(&usart);
    usart.USART_BaudRate                    =   115200;
    usart.USART_WordLength                  =   USART_WordLength_8b;
    usart.USART_StopBits                    =   USART_StopBits_1;
    usart.USART_Parity                      =   USART_Parity_No;
    usart.USART_Mode                        =   USART_Mode_Tx | USART_Mode_Rx;
    usart.USART_HardwareFlowControl         =   USART_HardwareFlowControl_None;
    USART_Init(USART2, &usart);


    // nvic
    nvic.NVIC_IRQChannel                    =   USART2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority  =   1;
    nvic.NVIC_IRQChannelSubPriority         =   2;
    nvic.NVIC_IRQChannelCmd                 =   ENABLE; 
    NVIC_Init(&nvic);


		USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
    USART_Cmd(USART2, ENABLE); 
}


void USART2_IRQHandler(void)
{
		uint8_t temp;
		static bool isGotFrameHeader = false;
		static uint8_t frameHeaderCount = 0;
		static uint8_t dataCount = 0;
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
			temp = USART_ReceiveData(USART2);
			
			if (!isGotFrameHeader) //判断帧头
			{  
				if (temp == 0x59) 
				{
					frameHeaderCount++;
					if (frameHeaderCount == 2) 
					{
						frameHeaderCount = 0;
						isGotFrameHeader = true;
						dataCount = 0;
						return ;
					}
				} 	
				else 
				{
					isGotFrameHeader = false;
					dataCount = 0;
					frameHeaderCount = 0;
				}
			}
			if(isGotFrameHeader)
			{
				radar[dataCount] = temp;
				dataCount = (dataCount+1)%8;
			}
			
			if(dataCount == 0 && isGotFrameHeader)
			{
				isGotFrameHeader = false;
				distance = ((((uint16_t)(radar[1])) << 8) | (uint8_t)(radar[0]));
				signal_strength = ((((uint16_t)(radar[3])) << 8) | (uint8_t)(radar[2]));
				//tempeture = ((float)((((uint16_t)(radar[5])) << 8) | (uint8_t)(radar[4]))/8.0)-256;  //温度，好像没啥用，不算了
			}
    }
}
