#include "imu_out.h"
#include "stm32f4xx_hal.h"
#include "i2c.h"
#include "mpu6500_reg.h"

volatile float Gyro_Yaw_out,Gyro_Pitch_out,Gyro_Roll_out;
volatile float yaw_angle_out,pitch_angle_out,roll_angle_out;

/**
  * @brief  串口接收外置imu数据处理
  * @param  None
  * @retval None
  */
//串口线朝后，放炮管背面
void IMU_task(uint8_t *pData)
{
	if(pData == NULL)
    {
        return;
    }
	Gyro_Yaw_out   = ((int16_t)(pData[2]<<8 |pData[5]))/16;//32768.0*2000.0;
	Gyro_Roll_out  = -((int16_t)(pData[6]<<8 |pData[7]))/16;///32768.0*2000.0;
	Gyro_Pitch_out    = ((int16_t)(pData[8]<<8 |pData[9]))/16;///32768.0*2000.0;
	
	yaw_angle_out  = ((int16_t)(pData[10]<<8 |pData[11]))/100;//32768.0*180.0;
	roll_angle_out = -((int16_t)(pData[12]<<8 |pData[13]))/100;///32768.0*180.0;
	pitch_angle_out   = -((int16_t)(pData[14]<<8 |pData[15]))/100;///32768.0*180.0;
 
}


