#include "imu.h"

volatile imu_t imu;

static uint16_t _1L = IMU_MSG_LENGTH;
static uint16_t _2L = IMU_MSG_LENGTH_2;

//float gimbal_pitch = 0;
//float gimbal_yaw = 0;

//uint8_t gimbal_send[10] = {0};

void imu_download_msg_process(uint8_t * buffer)
{
    // È·ÈÏÖ¡Í·
    if(buffer[0] == IMU_MSG_SOF)
    {
        imu_msg_unpack_handler(buffer);			
    }
}

/**
  * @brief     MPU6050 ½â°üº¯Êý
  * @param[in] buffer: ÍêÕûµÄÈýÖ¡
  * @retval    null
  */
void imu_msg_unpack_handler(uint8_t * buffer)
{
	int i = 0;
	//uint8_t * point;
	if (buffer[IMU_CMD_ID_SEGMENT_OFFSET] == IMU_ACCEL_CMD_ID)
    {
        // IMU_ACCEL: // 0.005f ˜ 1.f / 32768.f * 16.f * 9.8f
        imu.acc.x =  ((short)(buffer[3]<<8 | buffer[2])) * 0.005f;    // g
        imu.acc.y =  ((short)(buffer[5]<<8 | buffer[4])) * 0.005f;    // g
        imu.acc.z =  ((short)(buffer[7]<<8 | buffer[6])) * 0.005f;    // g
    }
    
    if (buffer[IMU_CMD_ID_SEGMENT_OFFSET + _1L] == IMU_GYRO_CMD_ID)
    {
        // IMU_GYRO:  // 0.061f ˜ 1.f / 32768.f * 2000.f
        imu.gyro.x = ((short)(buffer[_1L + 3]<<8 | buffer[_1L + 2])) * 0.061f;   // deg/sec
        imu.gyro.y = ((short)(buffer[_1L + 5]<<8 | buffer[_1L + 4])) * 0.061f;   // deg/sec
        imu.gyro.z = ((short)(buffer[_1L + 7]<<8 | buffer[_1L + 6])) * 0.061f;   // deg/sec
    }
    
    if (buffer[IMU_CMD_ID_SEGMENT_OFFSET + _2L] == IMU_MAG_CMD_ID)
    {
        // IMU_MAG:   // 0.005f ˜ 1.f / 32768.f * 180.f
        imu.mag.x =  ((short)(buffer[_2L + 3]<<8 | buffer[_2L + 2])) * 0.0054931f;    // deg
        imu.mag.y =  ((short)(buffer[_2L + 5]<<8 | buffer[_2L + 4])) * 0.0054931f;    // deg
        imu.mag.z =  ((short)(buffer[_2L + 7]<<8 | buffer[_2L + 6])) * 0.0054931f;    // deg
//				gimbal_pitch = imu.mag.y;	//pitchÊÇÓÉ¾ø¶Ô×ø±êÏµµÄ
//				gimbal_yaw = imu.mag.z;		//yawÊÇ³õÊ¼Î»ÖÃ¾ö¶¨µÄ
//				gimbal_send[0] = 0xAA;
//				gimbal_send[1] = 0xAA;
//				point = (uint8_t *)(&imu.mag.z);
//				for(i = 2; i <= 5; i++)
//				{
//					gimbal_send[i] = *point;
//					point++;
//				}
//				point = (uint8_t *)(&imu.mag.y);
//				for(i = 6; i <= 9; i++)
//				{
//					gimbal_send[i] = *point;
//					point++;
//				}
			//gimbal_send[2] = (uint8_t)gimbal_yaw>>;
				
    }
}

