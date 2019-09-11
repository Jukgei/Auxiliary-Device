#include "can.h"
#include "motor.h"

extern attitude_t attitude;

void CAN1_Configuration(void)
{
    GPIO_InitTypeDef       gpio;
    NVIC_InitTypeDef       nvic;
    CAN_InitTypeDef        can;
    CAN_FilterInitTypeDef  can_filter;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,  ENABLE);

    gpio.GPIO_Pin                           =   GPIO_Pin_0 | GPIO_Pin_1;
    gpio.GPIO_Mode                          =   GPIO_Mode_AF;
    GPIO_Init(GPIOD, &gpio);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);
    
    nvic.NVIC_IRQChannel                    =   CAN1_RX0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority  =   1;
    nvic.NVIC_IRQChannelSubPriority         =   3;
    nvic.NVIC_IRQChannelCmd                 =   ENABLE;
    NVIC_Init(&nvic);
    
    nvic.NVIC_IRQChannel                    =   CAN1_TX_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority  =   1;
    nvic.NVIC_IRQChannelSubPriority         =   3;
    nvic.NVIC_IRQChannelCmd                 =   ENABLE;
    NVIC_Init(&nvic);    
    
    CAN_DeInit(CAN1);
    CAN_StructInit(&can);
        
    can.CAN_TTCM                            =   DISABLE;
    can.CAN_ABOM                            =   ENABLE;
    can.CAN_AWUM                            =   ENABLE;
    can.CAN_NART                            =   DISABLE;
    can.CAN_RFLM                            =   DISABLE;
    can.CAN_TXFP                            =   DISABLE;
    can.CAN_Mode                            =   CAN_Mode_Normal;
    can.CAN_SJW                             =   CAN_SJW_1tq;
    can.CAN_BS1                             =   CAN_BS1_9tq;
    can.CAN_BS2                             =   CAN_BS2_4tq;
    can.CAN_Prescaler                       =   3;   //CAN BaudRate 42/(1+9+4)/3=1Mbps
    CAN_Init(CAN1, &can);    

    can_filter.CAN_FilterNumber             =   0;
    can_filter.CAN_FilterMode               =   CAN_FilterMode_IdMask;
    can_filter.CAN_FilterScale              =   CAN_FilterScale_32bit;
    can_filter.CAN_FilterIdHigh             =   0x0000;
    can_filter.CAN_FilterIdLow              =   0x0000;
    can_filter.CAN_FilterMaskIdHigh         =   0x0000;
    can_filter.CAN_FilterMaskIdLow          =   0x0000;
    can_filter.CAN_FilterFIFOAssignment     =   0;
    can_filter.CAN_FilterActivation         =   ENABLE;
    CAN_FilterInit(&can_filter);
    
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
    CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE); 
}

void CAN_Output(int16_t roll,int16_t pitch, int16_t yaw)
{
	CanTxMsg tx_message;
	
	tx_message.StdId = 0x1FF;
	
	tx_message.IDE = CAN_Id_Standard;
	
	tx_message.RTR = CAN_RTR_Data;
	
	tx_message.DLC = 0x08;
	
	//roll 205
	tx_message.Data[0] = (uint8_t)(roll >> 8);
	tx_message.Data[1] = (uint8_t)roll;
	
	//pitch 206
	tx_message.Data[2] = (uint8_t)(pitch >> 8);
	tx_message.Data[3] = (uint8_t)pitch;
	
	//yaw 207
	tx_message.Data[4] = (uint8_t)(yaw >> 8);
	tx_message.Data[5] = (uint8_t)yaw;
	
	tx_message.Data[6] = 0;
	tx_message.Data[7] = 0;
	
	CAN_Transmit(CAN1, &tx_message);
	
}
void CAN1_TX_IRQHandler(void)
{
    if (CAN_GetITStatus(CAN1, CAN_IT_TME) != RESET) 
    {
        CAN_ClearITPendingBit(CAN1, CAN_IT_TME);
    }
}

void CAN1_RX0_IRQHandler(void)
{   
		CanRxMsg rx_message;
    if (CAN_GetITStatus(CAN1, CAN_IT_FMP0)!= RESET)
    {
      CAN_ClearITPendingBit(CAN1, CAN_IT_FF0);
			CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
			attitude = can_receive_msg_process(&rx_message);
    }
}
