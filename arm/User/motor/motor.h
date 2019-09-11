#ifndef _MOTOR_H
#define _MOTOR_H

#include "stm32f4xx.h"
#include "systick.h"

#define YAW_MOTOR							0x207
#define ROLL_MOTOR						0x205
#define PITCH_MOTOR						0x206

#define RATE_BUF_SIZE 6
#define DOF 3


#define ENCODER_RPM_COEFFICIENT                      7.32f // = 1000.f * 60.f / 8192.f

#define PC2(a) if(a) \
					GPIO_SetBits(GPIOC,GPIO_Pin_2);\
					else \
					GPIO_ResetBits(GPIOC,GPIO_Pin_2)	

#define PC3(a) if(a) \
					GPIO_SetBits(GPIOC,GPIO_Pin_3); \
					else 	\
					GPIO_ResetBits(GPIOC,GPIO_Pin_3)
					
typedef struct{
    int32_t raw_value;                // �����������������ԭʼֵ
    int32_t last_raw_value;           // ��һ�α�������ԭʼֵ
    int32_t ecd_value;                // ��������������ı�����ֵ
    int32_t diff;                     // ���α�����֮��Ĳ�ֵ
    int32_t temp_count;               // ���ڼ���
    uint8_t buf_count;                // ��ֵ�˲�����buffer�±�
    int32_t ecd_bias;                 // ��ʼ������ֵ
    int32_t ecd_raw_rate;             // ͨ������������õ����ٶ�ԭʼֵ
    int32_t rate_buf[RATE_BUF_SIZE];  // ���ھ�ֵ�˲���
    int32_t round_cnt;                // ��ǰȦ��
    float filter_rpm;                 // �ٶ�
    float local_angle;                // �Ƕ�(0 ~ 360)
    float total_angle;                // �Ƕ�(-���� ~ +����)
}encoder_t;

typedef struct{
	float roll;
	float pitch;
	float yaw;
}attitude_t;

attitude_t can_receive_msg_process(CanRxMsg *msg);
void Motor3510_Rx_Msg_Process(volatile encoder_t *v, CanRxMsg *msg);
void Motor2006_Rx_Msg_Process(volatile encoder_t *v, CanRxMsg *msg);
void get_encoder_bias(volatile encoder_t *v, CanRxMsg * msg);
void gripper_init(void);
void grasp(void);
void loose(void);
void gripper_stop(void);
void gripper_control(void);

#endif
