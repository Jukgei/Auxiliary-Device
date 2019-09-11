#include "pid.h"

pid_t pid[3] = {{YAW_P, YAW_I, YAW_D, DT, 0, 0, 0, 0, 0, 0, 0},
								{PITCH_P, PITCH_I, PITCH_D, DT, 0, 0, 0, 0, 0, 0, 0},
								{ROLL_P, ROLL_I, ROLL_D, DT, 0, 0, 0, 0, 0, 0, 0}
};

float cal_pid(float des_val,float cur_val,uint8_t motor_num)
{
	
	pid[motor_num].err_past = pid[motor_num].err;
	
	//cal p_out
//	if(des_val-cur_val>180)
//		pid[motor_num].err = 50;
//	else if(des_val - cur_val<-180)
//		pid[motor_num].err = -50;
//	else
	pid[motor_num].err = ang_diff(des_val, cur_val);
	
	pid[motor_num].p_out = pid[motor_num].p * pid[motor_num].err;
	
	//cal i_out
	pid[motor_num].err_integral = pid[motor_num].err_integral +pid[motor_num].err;
	//integral limited
	if(pid[motor_num].err_integral > MAX_INTEGRAL)			pid[motor_num].err_integral = MAX_INTEGRAL;
	else if(pid[motor_num].err_integral < MIN_INTEGRAL)	pid[motor_num].err_integral = MIN_INTEGRAL;																							
		
	pid[motor_num].i_out = pid[motor_num].i * pid[motor_num].err_integral * pid[motor_num].dt;
	
	
	//cal d_out
	pid[motor_num].d_out = pid[motor_num].d * ( pid[motor_num].err - pid[motor_num].err_past) / pid[motor_num].dt;
	
	//total out
	pid[motor_num].pid_out = pid[motor_num].p_out + pid[motor_num].i_out + pid[motor_num].d_out;
	
	if(motor_num==0)
	{
		if(pid[motor_num].pid_out > MAX_2006_MOTOR)
			pid[motor_num].pid_out = MAX_2006_MOTOR;
		else if(pid[motor_num].pid_out < MIN_2006_MOTOR)
			pid[motor_num].pid_out = MIN_2006_MOTOR;
	}
	else if(motor_num == 1 || motor_num == 2)
	{
		if(pid[motor_num].pid_out > MAX_3510_MOTOR)
			pid[motor_num].pid_out = MAX_3510_MOTOR;
		else if(pid[motor_num].pid_out < MIN_3510_MOTOR)
			pid[motor_num].pid_out = MIN_3510_MOTOR;
	}
	
	return pid[motor_num].pid_out;
}

float f_mod(float a, float b)
{
	float c;
	int d;
	c = a/b;
	if(c<0)
		d = (int)(c-1);
	else
		d = (int)c;
	return (a - d * b);
}

float ang_diff(float ang1,float ang2)
{
	float diff;
	diff = ang1 - ang2;
	diff = f_mod(diff+180,360.0)-180;
	return diff;
}
