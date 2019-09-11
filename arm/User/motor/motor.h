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
    int32_t raw_value;                // 编码器不经过处理的原始值
    int32_t last_raw_value;           // 上一次编码器的原始值
    int32_t ecd_value;                // 经过处理后连续的编码器值
    int32_t diff;                     // 两次编码器之间的差值
    int32_t temp_count;               // 用于计数
    uint8_t buf_count;                // 均值滤波器的buffer下标
    int32_t ecd_bias;                 // 初始编码器值
    int32_t ecd_raw_rate;             // 通过编码器计算得到的速度原始值
    int32_t rate_buf[RATE_BUF_SIZE];  // 用于均值滤波器
    int32_t round_cnt;                // 当前圈数
    float filter_rpm;                 // 速度
    float local_angle;                // 角度(0 ~ 360)
    float total_angle;                // 角度(-无穷 ~ +无穷)
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
