#include "Driver_Chassis.h"
#include "Remote.h"
#include "pid.h"
#include "can.h"
#include "CanBusTask.h"
#include "StatusMachine.h"
#include "Ramp.h"
#include "IOTask.h"
#include "Driver_Gimbal.h"
#include "Driver_Judge.h"
#include "SuperviseTask.h"


ChassisDataTypeDef ChassisData;

PID_Regulator_t CMRotatePID = CHASSIS_MOTOR_ROTATE_PID_DEFAULT; 
PID_Regulator_t CM1SpeedPID = CHASSIS_MOTOR_SPEED_PID_DEFAULT;
PID_Regulator_t CM2SpeedPID = CHASSIS_MOTOR_SPEED_PID_DEFAULT;
PID_Regulator_t CM3SpeedPID = CHASSIS_MOTOR_SPEED_PID_DEFAULT;
PID_Regulator_t CM4SpeedPID = CHASSIS_MOTOR_SPEED_PID_DEFAULT;

RampGen_t LRSpeedRamp = RAMP_GEN_DAFAULT;   //mouse�����ƶ�б��
RampGen_t FBSpeedRamp = RAMP_GEN_DAFAULT;   //mouseǰ���ƶ�б��
RampGen_t MBSpeedRamp = RAMP_GEN_DAFAULT;   //mouseè���ƶ�б�� 

static void CalculateWheelSpeed(float vx, float vy, float omega, float radian, uint16_t maxspeed);
void Key2Speed(int16_t FB, int16_t LR)
{
	int16_t tmp_FB = 0;
	int16_t tmp_LR = 0;

	tmp_FB = FB ;
	tmp_LR = LR ;


	if(Remote_CheckJumpKey(KEY_W) == 1)
	{
		ChassisData.ChassisSpeedRef.Y = tmp_FB*FBSpeedRamp.Calc(&FBSpeedRamp);
	}
	else if(Remote_CheckJumpKey(KEY_S) == 1)
	{
		ChassisData.ChassisSpeedRef.Y = -tmp_FB*FBSpeedRamp.Calc(&FBSpeedRamp);
	}
	else
	{
		FBSpeedRamp.ResetCounter(&FBSpeedRamp);
		ChassisData.ChassisSpeedRef.Y = 0;
	}

	if(Remote_CheckJumpKey(KEY_D) == 1)
	{
		ChassisData.ChassisSpeedRef.X = tmp_LR*LRSpeedRamp.Calc(&LRSpeedRamp);
	}
	else if(Remote_CheckJumpKey(KEY_A) == 1)
	{
		ChassisData.ChassisSpeedRef.X = -tmp_LR*LRSpeedRamp.Calc(&LRSpeedRamp);
	}
	else
	{
		LRSpeedRamp.ResetCounter(&LRSpeedRamp);
		ChassisData.ChassisSpeedRef.X = 0;
	}
}

/**
  * @brief  �����˶�����
  * @param  None
  * @retval None
  */
static void UnderpanMoveControl(void)
{	
	switch(GetChassis_MoveMode())
	{	
		case Chassis_Stop:
		{
			ChassisData.ChassisSpeedRef.Y = 0;
			ChassisData.ChassisSpeedRef.X = 0;                                                       
			ChassisData.ChassisSpeedRef.Omega  = 0;
			ChassisData.ChassisAngle = 0;//��ֵ ǰ��������ʱ��ת
		}break;
		
		case Chassis_RC_Mode:                                    
		{
			
				ChassisData.ChassisSpeedRef.Y		= RC_CtrlData.rc.ch1*RC2CAR;
				ChassisData.ChassisSpeedRef.X   	= RC_CtrlData.rc.ch0*RC2CAR; 
				ChassisData.ChassisSpeedRef.Omega	= PID_Task(&CMRotatePID, 0, -GMYawEncoder.ecd_angle); 	//9015����
				ChassisData.ChassisAngle 			= GMYawEncoder.ecd_angle;
	
		}break; 
		
		case Chassis_Mouse_Mode:
		{ 
			switch(Get_AttackMode())
			{
			  case Attack_Normal:
				{
					if(Remote_CheckJumpKey(KEY_SHIFT) == 1)//Shift ����ģʽ
					{
						Key2Speed(HIGH_FORWARD_BACK_SPEED, HIGH_LEFT_RIGHT_SPEED);
					}
					else if(Remote_CheckJumpKey(KEY_CTRL) == 1)//Ctrl����ȡ�� ����ģʽ
					{
						Key2Speed(LOW_FORWARD_BACK_SPEED, LOW_LEFT_RIGHT_SPEED);
					}				
					else
					{
						Key2Speed(NORMAL_FORWARD_BACK_SPEED, NORMAL_LEFT_RIGHT_SPEED);
					}
					
					//��̨����ģʽ
					if(GetGM_CM_Mode() == GM_CM_Notlock)
					{
						ChassisData.ChassisAngle = GMYawEncoder.ecd_angle;//��̨����ʱ �����˶�������̨
						
						if(fabs(GMYawEncoder.ecd_angle) > YAW_MAX)//�����޶��� ���̸���ȥ
						{
							if(GMYawEncoder.ecd_angle >0)
								ChassisData.ChassisSpeedRef.Omega = PID_Task(&CMRotatePID, YAW_MAX, -GMYawEncoder.ecd_angle);
							else
								ChassisData.ChassisSpeedRef.Omega = PID_Task(&CMRotatePID, -YAW_MAX, -GMYawEncoder.ecd_angle);							
						}
					}
					//���̸���
					else					
					{
						ChassisData.ChassisAngle = 0;//�����ʱ�򳯵��̷���
						ChassisData.ChassisSpeedRef.Omega = PID_Task(&CMRotatePID, 0, -GMYawEncoder.ecd_angle);
					}
				}break;
				case Attack_Lop:
				{
					
				}break;
				case Attack_CatWalk:
				{
					static uint32_t time_re = 0;
					time_re++;
					ChassisData.ChassisSpeedRef.Omega =PID_Task(&CMRotatePID, 35*sin(time_re/360.0f *2*M_PI), -GMYawEncoder.ecd_angle);
					
					ChassisData.ChassisAngle = GMYawEncoder.ecd_angle;
					
					//Shift ����ģʽ
					if(Remote_CheckJumpKey(KEY_SHIFT) == 1)
					{
						Key2Speed(HIGH_FORWARD_BACK_SPEED, HIGH_LEFT_RIGHT_SPEED);
					}
					else
					{
						Key2Speed(NORMAL_FORWARD_BACK_SPEED, NORMAL_LEFT_RIGHT_SPEED);
					}

				}break;
				case Attack_Slow:
				{
					Key2Speed(LOW_FORWARD_BACK_SPEED, LOW_LEFT_RIGHT_SPEED);
										//��̨����ģʽ
					if(GetGM_CM_Mode() == GM_CM_Notlock)
					{
						ChassisData.ChassisAngle = GMYawEncoder.ecd_angle;//��̨����ʱ �����˶�������̨
						
						if(fabs(GMYawEncoder.ecd_angle) > YAW_MAX)//�����޶��� ���̸���ȥ
						{
							if(GMYawEncoder.ecd_angle >0)
								ChassisData.ChassisSpeedRef.Omega = PID_Task(&CMRotatePID, YAW_MAX, -GMYawEncoder.ecd_angle);
							else
								ChassisData.ChassisSpeedRef.Omega = PID_Task(&CMRotatePID, -YAW_MAX, -GMYawEncoder.ecd_angle);							
						}
					}
					//���̸���
					else					
					{
						ChassisData.ChassisAngle = 0;//�����ʱ�򳯵��̷���
						ChassisData.ChassisSpeedRef.Omega = PID_Task(&CMRotatePID, 0, -GMYawEncoder.ecd_angle);
					}

				}break;
				case Attack_45Degree:
				{
					
				}break;
				case Attack_None:
				{
					
				}break;
			}
		}break;	
		case Chassis_Test:
		{
			switch(Get_AttackMode())
			{
			  case Attack_Normal:
				{
						ChassisData.ChassisSpeedRef.Y		= RC_CtrlData.rc.ch1*RC2CAR;
						ChassisData.ChassisSpeedRef.X   	= RC_CtrlData.rc.ch0*RC2CAR; 
						ChassisData.ChassisSpeedRef.Omega	= PID_Task(&CMRotatePID, 0, -GMYawEncoder.ecd_angle); 	//9015����
						ChassisData.ChassisAngle 			= GMYawEncoder.ecd_angle;
				}break;
				case Attack_CatWalk:
				{
					static uint32_t time_re = 0;
					time_re++;
					ChassisData.ChassisSpeedRef.Y		= RC_CtrlData.rc.ch1*RC2CAR;
					ChassisData.ChassisSpeedRef.X   	= RC_CtrlData.rc.ch0*RC2CAR; 
					ChassisData.ChassisSpeedRef.Omega =PID_Task(&CMRotatePID, 35*sin(time_re/360.0f *2*M_PI), -GMYawEncoder.ecd_angle);
					ChassisData.ChassisAngle = GMYawEncoder.ecd_angle;

				}break;
			}
		}break;
		default:
			break;
	}
	
CalculateWheelSpeed(ChassisData.ChassisSpeedRef.X, \
						ChassisData.ChassisSpeedRef.Y, \
						ChassisData.ChassisSpeedRef.Omega, \
						Ang2Rad(ChassisData.ChassisAngle), \
						660);
}
/**
  * @brief  ����3���ٶ�ֵ�������ӵ��ٶ�
  * @param  float vx X�����ϵ��ٶ�
  * @param  float vy Y�����ϵ��ٶ�
  * @param  float omega ��ת���ٶ�
  * @param  float radian ��ʱ���������ĽǶ�(����)��˳ʱ��Ϊ����
  * @param  int16_t maxspped  ����ٶ�
  * @retval None
  */
static void CalculateWheelSpeed(float vx, float vy, float omega, float radian, uint16_t maxspeed)
{
	float   fWheelSpd[5];
	float	Chassis_forward_back_ref;		//��ս������ϵ�Ĳο��ٶ�
	float	Chassis_left_right_ref;
	float 	fMaxSpd = 0;
	int16_t s16_WheelSpd[5];
	Chassis_forward_back_ref = vy*cos(radian)+vx*sin(radian);		
	Chassis_left_right_ref   = vx*cos(radian)-vy*sin(radian);
	//���ֽ���
	fWheelSpd[0] = -Chassis_forward_back_ref + Chassis_left_right_ref + omega;	//���Ͻǵ����ʼ��ʱ����
	fWheelSpd[1] =  Chassis_forward_back_ref + Chassis_left_right_ref + omega;
	fWheelSpd[2] =  Chassis_forward_back_ref - Chassis_left_right_ref + omega;
	fWheelSpd[3] = -Chassis_forward_back_ref - Chassis_left_right_ref + omega;
	  
	//���� �ҵ��ٶ����ֵ
	fMaxSpd = fabs(fWheelSpd[0]);		
	if(fabs(fWheelSpd[1]) > fMaxSpd)
		fMaxSpd = fabs(fWheelSpd[1]);
	if(fabs(fWheelSpd[2]) > fMaxSpd)
		fMaxSpd = fabs(fWheelSpd[2]);
	if(fabs(fWheelSpd[3]) > fMaxSpd)
		fMaxSpd = fabs(fWheelSpd[3]);
  
	//�����������ٶ�
	if(fMaxSpd > maxspeed)
	{
		s16_WheelSpd[0]   = (int16_t)(fWheelSpd[0]*(maxspeed/fMaxSpd));
		s16_WheelSpd[1]   = (int16_t)(fWheelSpd[1]*(maxspeed/fMaxSpd));
		s16_WheelSpd[2]   = (int16_t)(fWheelSpd[2]*(maxspeed/fMaxSpd));
		s16_WheelSpd[3]   = (int16_t)(fWheelSpd[3]*(maxspeed/fMaxSpd));
	}
	else
	{
		s16_WheelSpd[0]   = (int16_t)fWheelSpd[0];
		s16_WheelSpd[1]   = (int16_t)fWheelSpd[1];
		s16_WheelSpd[2]   = (int16_t)fWheelSpd[2];
		s16_WheelSpd[3]   = (int16_t)fWheelSpd[3];
	}
	memcpy((void*)ChassisData.ChassisWheelSpeedRef, (void*)s16_WheelSpd, 8);
	//Power_Limit_OutputCal();
	CM_PID_Cal();
}
/**
  * @brief  ��������ģʽ ���
  * @param  float vx X�����ϵ��ٶ�
  * @param  float vy Y�����ϵ��ٶ�
  * @param  float omega ��ת���ٶ�
  * @param  float radian ��ʱ���������ĽǶ�(����)��˳ʱ��Ϊ����
  * @param  int16_t maxspped  ����ٶ�
  */
  #define Power_Max 100
#define Energy_Min  30
#define Energy_Dead 10
extern float OP_ChassisCurrent;
void Power_Limit_OutputCal()
{	
	float t_realPower = JudgingInfo.PowerHeatData.chassisPower;	//ʵʱ����
	float t_realEnergy = JudgingInfo.PowerHeatData.chassisPowerBuffer;
//	float t_real_adcPower = OP_ChassisCurrent*24;
  if(JudgeFrameRate>50)
	{
		if(t_realPower > 80)
		{
			//120W˥��
			float reduction = 1- (t_realPower-80)/40.0f;
			if(reduction < 0)
				reduction = 0;
			CM1SpeedPID.output *= reduction;
			CM2SpeedPID.output *= reduction;
			CM3SpeedPID.output *= reduction;
			CM4SpeedPID.output *= reduction;
		}
		
		
		static uint8_t slow_flag = 0;
		if(slow_flag == 0)	//δ����˥��ģʽ
		{
			if(t_realEnergy < Energy_Min)
				slow_flag = 1;//��ͨ˥��
			else if(t_realEnergy < Energy_Dead)
				slow_flag = 2; //����˥��
		}
		else if(t_realEnergy >50)//�˳�˥��ģʽ
		{
			slow_flag = 0;
		}
		if(slow_flag == 1)
		{
			ChassisData.ChassisWheelSpeedRef[0] *= 0.7f;
			ChassisData.ChassisWheelSpeedRef[1]	*= 0.7f;
			ChassisData.ChassisWheelSpeedRef[2]	*= 0.7f;
			ChassisData.ChassisWheelSpeedRef[3]	*= 0.7f;//ԭ����0.25
			VAL_LIMIT(CM1SpeedPID.output,-3500,3500);
			VAL_LIMIT(CM2SpeedPID.output,-3500,3500);
			VAL_LIMIT(CM3SpeedPID.output,-3500,3500);
			VAL_LIMIT(CM4SpeedPID.output,-3500,3500);
		}
		else if(slow_flag == 2)
		{
			ChassisData.ChassisWheelSpeedRef[0] *= 0.25f;
			ChassisData.ChassisWheelSpeedRef[1]	*= 0.25f;
			ChassisData.ChassisWheelSpeedRef[2]	*= 0.25f;
			ChassisData.ChassisWheelSpeedRef[3]	*= 0.25f;//ԭ����0.25
			VAL_LIMIT(CM1SpeedPID.output,-1000,1000);
			VAL_LIMIT(CM2SpeedPID.output,-1000,1000);
			VAL_LIMIT(CM3SpeedPID.output,-1000,1000);
			VAL_LIMIT(CM4SpeedPID.output,-1000,1000);
		}

	}
		else//û�в���ϵͳ�������ݵ�ʱ��
	{
		VAL_LIMIT(CM1SpeedPID.output,-3500,3500);
		VAL_LIMIT(CM2SpeedPID.output,-3500,3500);
		VAL_LIMIT(CM3SpeedPID.output,-3500,3500);
		VAL_LIMIT(CM4SpeedPID.output,-3500,3500);
	}
}

/**
  * @brief  ���Ƶ������
  * @param  None
  * @retval None
  */
void CM_LIMIT (PID_Regulator_t *PID_Stucture1,PID_Regulator_t *PID_Stucture2,PID_Regulator_t *PID_Stucture3,PID_Regulator_t *PID_Stucture4)
{
	static uint32_t Sum_Output,Over_Output;
	static float k;
	Sum_Output=fabs(PID_Stucture1->output)+fabs(PID_Stucture2->output)+fabs(PID_Stucture3->output)+fabs(PID_Stucture4->output);
	if (Sum_Output>PID_Stucture1->outputMax*4)
	{
		Over_Output=PID_Stucture1->outputMax*4;
		k=1.0f*Over_Output/Sum_Output;
		PID_Stucture1->output*=k;
		PID_Stucture2->output*=k;
		PID_Stucture3->output*=k;
		PID_Stucture4->output*=k;
	}
}

#define POWER 68.0f  //����75W
#define OUT_MAX  6700

/**
  * @brief  ����PID���
  * @param  None
  * @retval None
  */
void CM_PID_Cal(void)
{	
	PID_Task(&CM1SpeedPID, ChassisData.ChassisWheelSpeedRef[0], CM1_Measure.speed_rpm/10.0);
	PID_Task(&CM2SpeedPID, ChassisData.ChassisWheelSpeedRef[1], CM2_Measure.speed_rpm/10.0);
	PID_Task(&CM3SpeedPID, ChassisData.ChassisWheelSpeedRef[2], CM3_Measure.speed_rpm/10.0);
	PID_Task(&CM4SpeedPID, ChassisData.ChassisWheelSpeedRef[3], CM4_Measure.speed_rpm/10.0);
	
//	if(CM1SpeedPID.output>-500 && CM1SpeedPID.output<500 ) CM1SpeedPID.output=0;
//	if(CM2SpeedPID.output>-500 && CM2SpeedPID.output<500 ) CM2SpeedPID.output=0;
//	if(CM3SpeedPID.output>-500 && CM3SpeedPID.output<500 ) CM3SpeedPID.output=0;
//	if(CM4SpeedPID.output>-500 && CM4SpeedPID.output<500 ) CM4SpeedPID.output=0;//700
}

/**
  * @brief  ���õ������
  * @param  None
  * @retval None
  */

int a = 0;
void CM_SetOutput(void)
{
	if (GetGimbal_MoveMode() == STOP)    //|| dead_lock_flag == 1����ͣ����������У׼���޿�������ʱ����ʹ���̿���ֹͣ
	{
		Set_CM_Speed(&hcan1, 0,0,0,0);	
	}
	else
	{
		Set_CM_Speed(&hcan1,  CHASSIS_SPEED_ATTENUATION * CM1SpeedPID.output, \
							CHASSIS_SPEED_ATTENUATION * CM2SpeedPID.output, \
							CHASSIS_SPEED_ATTENUATION * CM3SpeedPID.output, \
							CHASSIS_SPEED_ATTENUATION * CM4SpeedPID.output);	
	}
}
/**
  * @brief  ʹ����λ������PIDֵ
  * @param  None
  * @retval None
  */
static void PID_Calibration(void)
{
	CMRotatePID.kp = AppParamRealUsed.RotateSpeedPID.kp_offset;
	CMRotatePID.ki = AppParamRealUsed.RotateSpeedPID.ki_offset/1000.0;
	CMRotatePID.kd = AppParamRealUsed.RotateSpeedPID.kd_offset;
	CM1SpeedPID.kp = CM2SpeedPID.kp  = CM3SpeedPID.kp = CM4SpeedPID.kp = AppParamRealUsed.ChassisSpeedPID.kp_offset;
	CM1SpeedPID.ki = CM2SpeedPID.ki = CM3SpeedPID.ki = CM4SpeedPID.ki = 0;
	CM1SpeedPID.kd = CM2SpeedPID.kd = CM3SpeedPID.kd = CM4SpeedPID.kd = AppParamRealUsed.ChassisSpeedPID.kd_offset;

	CM1SpeedPID.err[2] =  CM2SpeedPID.err[2] = CM3SpeedPID.err[2] = CM4SpeedPID.err[2] = 0;//��һ����������Ҳ���㣿
	CM1SpeedPID.ki = CM2SpeedPID.ki = CM3SpeedPID.ki = CM4SpeedPID.ki = 0;
}
/**
  * @brief  ���̿���������
  * @param  None
  * @retval None
  */

void CMControlLoop(void)
{
	  PID_Calibration();//ʹ����λ��
  	UnderpanMoveControl();	//��ͬģʽ �õ�����ֵ
   	Power_Limit_OutputCal();
	  CM_SetOutput();
}