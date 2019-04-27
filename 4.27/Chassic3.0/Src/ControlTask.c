#include "ControlTask.h"
#include "Driver_Chassis.h"
#include "Driver_Gimbal.h"
#include "StatusMachine.h"
#include "imu.h"
#include "Driver_Beep.h"
#include "SuperviseTask.h"
#include "Driver_FriMotor.h"
#include "IOTask.h"
#include "CanBusTask.h"
#include "Ramp.h"
#include "Driver_Judge.h"
#include "Driver_CurrentMeasure.h"
#include "pid.h"

//extern AppParam_t gAppParamStruct;					//������Ϣ,���ﱣ�������µ�У׼ֵ��������Flash�е�����ͬ��
extern RampGen_t GMPitchRamp;
extern RampGen_t GMYawRamp;
extern RampGen_t FriMotorRamp;

uint32_t time_task_ms=0;
void ControlLoopTaskInit(void)
{
	time_task_ms=0;
	AppParamInit();
	Sensor_Offset_Param_Init(&gAppParamStruct);  
	
	GMPitchEncoder.round_cnt = 0;
	GMPitchEncoder.ecd_angle = 0;
	GMYawEncoder.round_cnt = 0;
	GMYawEncoder.ecd_angle = 0;
	//���ù���ģʽ
	SetWorkState(PREPARE_STATE);
	//б�³�ʼ��
	GMPitchRamp.SetScale(&GMPitchRamp, PITCH_PREPARE_TIME_TICK_MS);
	GMYawRamp.SetScale(&GMYawRamp, YAW_PREPARE_TIME_TICK_MS);
	GMPitchRamp.ResetCounter(&GMPitchRamp);
	GMYawRamp.ResetCounter(&GMYawRamp);
  FriMotorRamp.SetScale(&FriMotorRamp,FRIMOTOR_PREPARE_TIME_TICK_MS);
	FriMotorRamp.ResetCounter(&FriMotorRamp);
	
	//��̨�����Ƕȳ�ʼ��
	GimbalRef.Pitch = 0.0f;
	GimbalRef.Yaw = 0.0f;
	//Yaw��Ƕȳ�ʼ��
	yaw_angle = 0;
	//yaw_angle_out=0;
	turn_flag=1;
  StatusMachine_Init();
	
//		//PID��ʼ��
//	ShootMotorSpeedPID.Reset(&ShootMotorSpeedPID);
//	ShootMotorPositionPID.Reset(&ShootMotorPositionPID);
//	GMPPositionPID.Reset(&GMPPositionPID);
//	GMPSpeedPID.Reset(&GMPSpeedPID);
//	GMYPositionPID.Reset(&GMYPositionPID);
//	GMYSpeedPID.Reset(&GMYSpeedPID);
//	CMRotatePID.Reset(&CMRotatePID);
//	CM1SpeedPID.Reset(&CM1SpeedPID);
//	CM2SpeedPID.Reset(&CM2SpeedPID);
//	CM3SpeedPID.Reset(&CM3SpeedPID);
//	CM4SpeedPID.Reset(&CM4SpeedPID);

}

void Control_Task(void)
{
			time_task_ms++;
	    CaliLoop();
	  	StatusMachine_Update();
     	Task_Monitor();
	    BEEP_Task();
    	IMU_Task();
			//ADC_StartMeasure();				//��ȡ���̵���������
    	GMControlLoop();
	    FriMotor_Control();
	    Turntable_Control();
			if(time_task_ms%4 == 0)         
			{	
				 Judge_Task();		
				 LASER_Task();
				 CMControlLoop();
				 
			}
		//	CAN_Send();
			
}