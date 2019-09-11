#ifndef _PID_H
#define _PID_H

#include "stm32f4xx.h"

#define MAX_INTEGRAL	 5000
#define MIN_INTEGRAL  -5000

#define MAX_2006_MOTOR 3000
#define MIN_2006_MOTOR -3000

#define MAX_3510_MOTOR 	7000
#define MIN_3510_MOTOR -7000

#define DT						0.1

#define YAW_P					0
#define YAW_I					0
#define YAW_D					0

#define PITCH_P					30
#define PITCH_I					0.1
#define PITCH_D					0

//#define PITCH_P					0
//#define PITCH_I					0
//#define PITCH_D					0

#define ROLL_P					0
#define ROLL_I					0
#define ROLL_D					0


typedef struct{
	float p;
	float i;
	float d;
	float dt;
	float err;	
	float err_past;
	float err_integral;
	float p_out;
	float i_out;
	float d_out;
	float pid_out;
}pid_t;

float cal_pid(float des_val,float cur_val,uint8_t motor_num);
float ang_diff(float ang1,float ang2);
float f_mod(float a, float b);

#endif 
