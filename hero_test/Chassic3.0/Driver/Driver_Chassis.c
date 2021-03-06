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

RampGen_t LRSpeedRamp = RAMP_GEN_DAFAULT;   //mouse左右移动斜坡
RampGen_t FBSpeedRamp = RAMP_GEN_DAFAULT;   //mouse前后移动斜坡
RampGen_t MBSpeedRamp = RAMP_GEN_DAFAULT;   //mouse猫步移动斜坡 

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
  * @brief  底盘运动控制
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
			ChassisData.ChassisAngle = 0;//正值 前进方向逆时针转
		}break;
		
		case Chassis_RC_Mode:                                    
		{
			
				ChassisData.ChassisSpeedRef.Y		= RC_CtrlData.rc.ch1*RC2CAR;
				ChassisData.ChassisSpeedRef.X   	= RC_CtrlData.rc.ch0*RC2CAR; 
				ChassisData.ChassisSpeedRef.Omega	= PID_Task(&CMRotatePID, 0, -GMYawEncoder.ecd_angle); 	//9015特殊
				ChassisData.ChassisAngle 			= GMYawEncoder.ecd_angle;
	
		}break; 
		
		case Chassis_Mouse_Mode:
		{ 
			switch(Get_AttackMode())
			{
			  case Attack_Normal:
				{
					if(Remote_CheckJumpKey(KEY_SHIFT) == 1)//Shift 高速模式
					{
						Key2Speed(HIGH_FORWARD_BACK_SPEED, HIGH_LEFT_RIGHT_SPEED);
					}
					else if(Remote_CheckJumpKey(KEY_CTRL) == 1)//Ctrl或者取弹 低速模式
					{
						Key2Speed(LOW_FORWARD_BACK_SPEED, LOW_LEFT_RIGHT_SPEED);
					}				
					else
					{
						Key2Speed(NORMAL_FORWARD_BACK_SPEED, NORMAL_LEFT_RIGHT_SPEED);
					}
					
					//云台解锁模式
					if(GetGM_CM_Mode() == GM_CM_Notlock)
					{
						ChassisData.ChassisAngle = GMYawEncoder.ecd_angle;//云台解锁时 底盘运动方向朝云台
						
						if(fabs(GMYawEncoder.ecd_angle) > YAW_MAX)//超过限定角 底盘跟过去
						{
							if(GMYawEncoder.ecd_angle >0)
								ChassisData.ChassisSpeedRef.Omega = PID_Task(&CMRotatePID, YAW_MAX, -GMYawEncoder.ecd_angle);
							else
								ChassisData.ChassisSpeedRef.Omega = PID_Task(&CMRotatePID, -YAW_MAX, -GMYawEncoder.ecd_angle);							
						}
					}
					//底盘跟随
					else					
					{
						ChassisData.ChassisAngle = 0;//跟随的时候朝地盘方向
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
					
					//Shift 高速模式
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
										//云台解锁模式
					if(GetGM_CM_Mode() == GM_CM_Notlock)
					{
						ChassisData.ChassisAngle = GMYawEncoder.ecd_angle;//云台解锁时 底盘运动方向朝云台
						
						if(fabs(GMYawEncoder.ecd_angle) > YAW_MAX)//超过限定角 底盘跟过去
						{
							if(GMYawEncoder.ecd_angle >0)
								ChassisData.ChassisSpeedRef.Omega = PID_Task(&CMRotatePID, YAW_MAX, -GMYawEncoder.ecd_angle);
							else
								ChassisData.ChassisSpeedRef.Omega = PID_Task(&CMRotatePID, -YAW_MAX, -GMYawEncoder.ecd_angle);							
						}
					}
					//底盘跟随
					else					
					{
						ChassisData.ChassisAngle = 0;//跟随的时候朝地盘方向
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
						ChassisData.ChassisSpeedRef.Omega	= PID_Task(&CMRotatePID, 0, -GMYawEncoder.ecd_angle); 	//9015特殊
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
  * @brief  根据3个速度值计算轮子的速度
  * @param  float vx X方向上的速度
  * @param  float vy Y方向上的速度
  * @param  float omega 旋转的速度
  * @param  float radian 此时基地所处的角度(弧度)（顺时针为正）
  * @param  int16_t maxspped  最大速度
  * @retval None
  */
static void CalculateWheelSpeed(float vx, float vy, float omega, float radian, uint16_t maxspeed)
{
	float   fWheelSpd[5];
	float	Chassis_forward_back_ref;		//以战车坐标系的参考速度
	float	Chassis_left_right_ref;
	float 	fMaxSpd = 0;
	int16_t s16_WheelSpd[5];
	Chassis_forward_back_ref = vy*cos(radian)+vx*sin(radian);		
	Chassis_left_right_ref   = vx*cos(radian)-vy*sin(radian);
	//麦轮解算
	fWheelSpd[0] = -Chassis_forward_back_ref + Chassis_left_right_ref + omega;	//右上角电机开始逆时针数
	fWheelSpd[1] =  Chassis_forward_back_ref + Chassis_left_right_ref + omega;
	fWheelSpd[2] =  Chassis_forward_back_ref - Chassis_left_right_ref + omega;
	fWheelSpd[3] = -Chassis_forward_back_ref - Chassis_left_right_ref + omega;
	  
	//排序 找到速度最大值
	fMaxSpd = fabs(fWheelSpd[0]);		
	if(fabs(fWheelSpd[1]) > fMaxSpd)
		fMaxSpd = fabs(fWheelSpd[1]);
	if(fabs(fWheelSpd[2]) > fMaxSpd)
		fMaxSpd = fabs(fWheelSpd[2]);
	if(fabs(fWheelSpd[3]) > fMaxSpd)
		fMaxSpd = fabs(fWheelSpd[3]);
  
	//按比例限制速度
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
  * @brief  功率限制模式 输出
  * @param  float vx X方向上的速度
  * @param  float vy Y方向上的速度
  * @param  float omega 旋转的速度
  * @param  float radian 此时基地所处的角度(弧度)（顺时针为正）
  * @param  int16_t maxspped  最大速度
  */
  #define Power_Max 100
#define Energy_Min  30
#define Energy_Dead 10
extern float OP_ChassisCurrent;
void Power_Limit_OutputCal()
{	
	float t_realPower = JudgingInfo.PowerHeatData.chassisPower;	//实时功率
	float t_realEnergy = JudgingInfo.PowerHeatData.chassisPowerBuffer;
//	float t_real_adcPower = OP_ChassisCurrent*24;
  if(JudgeFrameRate>50)
	{
		if(t_realPower > 80)
		{
			//120W衰减
			float reduction = 1- (t_realPower-80)/40.0f;
			if(reduction < 0)
				reduction = 0;
			CM1SpeedPID.output *= reduction;
			CM2SpeedPID.output *= reduction;
			CM3SpeedPID.output *= reduction;
			CM4SpeedPID.output *= reduction;
		}
		
		
		static uint8_t slow_flag = 0;
		if(slow_flag == 0)	//未进入衰减模式
		{
			if(t_realEnergy < Energy_Min)
				slow_flag = 1;//普通衰减
			else if(t_realEnergy < Energy_Dead)
				slow_flag = 2; //超级衰减
		}
		else if(t_realEnergy >50)//退出衰减模式
		{
			slow_flag = 0;
		}
		if(slow_flag == 1)
		{
			ChassisData.ChassisWheelSpeedRef[0] *= 0.7f;
			ChassisData.ChassisWheelSpeedRef[1]	*= 0.7f;
			ChassisData.ChassisWheelSpeedRef[2]	*= 0.7f;
			ChassisData.ChassisWheelSpeedRef[3]	*= 0.7f;//原本是0.25
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
			ChassisData.ChassisWheelSpeedRef[3]	*= 0.25f;//原本是0.25
			VAL_LIMIT(CM1SpeedPID.output,-1000,1000);
			VAL_LIMIT(CM2SpeedPID.output,-1000,1000);
			VAL_LIMIT(CM3SpeedPID.output,-1000,1000);
			VAL_LIMIT(CM4SpeedPID.output,-1000,1000);
		}

	}
		else//没有裁判系统功率数据的时候
	{
		VAL_LIMIT(CM1SpeedPID.output,-3500,3500);
		VAL_LIMIT(CM2SpeedPID.output,-3500,3500);
		VAL_LIMIT(CM3SpeedPID.output,-3500,3500);
		VAL_LIMIT(CM4SpeedPID.output,-3500,3500);
	}
}

/**
  * @brief  限制电机电流
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

#define POWER 68.0f  //功率75W
#define OUT_MAX  6700

/**
  * @brief  计算PID输出
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
  * @brief  设置电机电流
  * @param  None
  * @retval None
  */

int a = 0;
void CM_SetOutput(void)
{
	if (GetGimbal_MoveMode() == STOP)    //|| dead_lock_flag == 1紧急停车，编码器校准，无控制输入时都会使底盘控制停止
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
  * @brief  使用上位机给的PID值
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

	CM1SpeedPID.err[2] =  CM2SpeedPID.err[2] = CM3SpeedPID.err[2] = CM4SpeedPID.err[2] = 0;//？一个赋零其余也是零？
	CM1SpeedPID.ki = CM2SpeedPID.ki = CM3SpeedPID.ki = CM4SpeedPID.ki = 0;
}
/**
  * @brief  底盘控制主任务
  * @param  None
  * @retval None
  */

void CMControlLoop(void)
{
	  PID_Calibration();//使用上位机
  	UnderpanMoveControl();	//不同模式 得到给定值
   	Power_Limit_OutputCal();
	  CM_SetOutput();
}
