#include "usart1.h"

/*-----USART1_TX-----PA9-----*/
/*-----USART1_RX-----PA10-----*/

u8 UART_RX_BUF[16];
bool isUartRxCompleted = false;

void USART1_Configuration(void)
{
    GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;
    //DMA_InitTypeDef   dma;
    USART_InitTypeDef usart;
		
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,  ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    gpio.GPIO_Pin                           =   GPIO_Pin_10 | GPIO_Pin_9;
    gpio.GPIO_Mode                          =   GPIO_Mode_AF;
    gpio.GPIO_OType                         =   GPIO_OType_PP;
    gpio.GPIO_Speed                         =   GPIO_Speed_100MHz;
    gpio.GPIO_PuPd                          =   GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &gpio);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

    USART_DeInit(USART1);
    USART_StructInit(&usart);
    usart.USART_BaudRate                    =   115200;
    usart.USART_WordLength                  =   USART_WordLength_8b;
    usart.USART_StopBits                    =   USART_StopBits_1;
    usart.USART_Parity                      =   USART_Parity_No;
    usart.USART_Mode                        =   USART_Mode_Tx | USART_Mode_Rx;
    usart.USART_HardwareFlowControl         =   USART_HardwareFlowControl_None;
    USART_Init(USART1, &usart);


    // nvic
    nvic.NVIC_IRQChannel                    =   USART1_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority  =   1;
    nvic.NVIC_IRQChannelSubPriority         =   3;
    nvic.NVIC_IRQChannelCmd                 =   ENABLE; 
    NVIC_Init(&nvic);


		USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
    USART_Cmd(USART1, ENABLE); 
}


void uartWriteBuf(uint8_t *buf, uint8_t len)
{
	TX(1);
	RX(0);
	Delay_us(10);
	while (len--) {
		while ((USART1->SR & 0x40) == 0);
		USART_SendData(USART1,*buf++);
	}
	Delay_us(10);
	TX(0);
	RX(1);
}

bool isRxCompleted(void)
{
	if(isUartRxCompleted == true){
		isUartRxCompleted = false;
		return true;
	}else{
		return false;
	}
}

void SendPostion(uint8_t ID)
{
	
	int temp = 0;
	uint16_t position = 0;
	uint16_t header = 0xffff-ID;
	temp = LobotSerialServoReadPosition(ID);
	position = temp;
	USART_SendData(USART3,(uint8_t)header);
	Delay_us(10);
	USART_SendData(USART3,(uint8_t)(header>>8));
	Delay_us(10);
	USART_SendData(USART3,(uint8_t)position);
	Delay_us(10);
	USART_SendData(USART3,(uint8_t)(position>>8));
}

void USART1_IRQHandler(void)
{
	uint8_t Res;
	static bool isGotFrameHeader = false;
	static uint8_t frameHeaderCount = 0;
	static uint8_t dataLength = 2;
	static uint8_t dataCount = 0;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) { //判断接收中断
		Res = USART_ReceiveData(USART1);//(USART1->DR);	//取出接收寄存器数据
		if (!isGotFrameHeader) {  //判断帧头
			if (Res == 0x55) {
				frameHeaderCount++;
				if (frameHeaderCount == 2) {
					frameHeaderCount = 0;
					isGotFrameHeader = true;
					dataCount = 1;
				}
			} else {
				isGotFrameHeader = false;
				dataCount = 0;
				frameHeaderCount = 0;
			}
		}
		if (isGotFrameHeader) { //接收接收数据部分
			UART_RX_BUF[dataCount] = Res;
			if (dataCount == 3) {
				dataLength = UART_RX_BUF[dataCount];
				if (dataLength < 3 || dataLength > 7) {
					dataLength = 3;
					isGotFrameHeader = false;
				}
			}
			dataCount++;
			if (dataCount == dataLength + 3) {
				if (isUartRxCompleted == false) {
					isUartRxCompleted = true;
					memcpy(LobotRxBuf, UART_RX_BUF, dataCount+2);	
				}
				isGotFrameHeader = false;
			}
		}
	}
}
