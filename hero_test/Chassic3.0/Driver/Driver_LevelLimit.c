/****************************等级血量热量功率限制**********************************/
/*






*/


#include "Driver_LevelLimit.h" 
#include "Driver_Judge.h"

SHOOT_JUDGE_e BIG_shoot;
SHOOT_JUDGE_e LITTLE_shoot;
SHOOT_JUDGE_e BIG_shoot2;

///**
//  * @brief  功率限制模式 输出
//  * @param  float vx X方向上的速度
//  * @param  float vy Y方向上的速度
//  * @param  float omega 旋转的速度
//  * @param  float radian 此时基地所处的角度(弧度)（顺时针为正）
//  * @param  int16_t maxspped  最大速度
//  */
//  #define Power_Max 100
//#define Energy_Min  30
//#define Energy_Dead 10
//extern float OP_ChassisCurrent;
//void Power_Limit_OutputCal()
//{	
//	float t_realPower = JudgingInfo.PowerHeatData.chassisPower;	//实时功率
//	float t_realEnergy = JudgingInfo.PowerHeatData.chassisPowerBuffer;
////	float t_real_adcPower = OP_ChassisCurrent*24;

//		if(t_realPower > 80)
//		{
//			//120W衰减
//			float reduction = 1- (t_realPower-80)/40.0f;
//			if(reduction < 0)
//				reduction = 0;
//			CM1SpeedPID.output *= reduction;
//			CM2SpeedPID.output *= reduction;
//			CM3SpeedPID.output *= reduction;
//			CM4SpeedPID.output *= reduction;
//		}
//		
//		
//		static uint8_t slow_flag = 0;
//		if(slow_flag == 0)	//未进入衰减模式
//		{
//			if(t_realEnergy < Energy_Min)
//				slow_flag = 1;//普通衰减
//			else if(t_realEnergy < Energy_Dead)
//				slow_flag = 2; //超级衰减
//		}
//		else if(t_realEnergy >50)//退出衰减模式
//		{
//			slow_flag = 0;
//		}
//		if(slow_flag == 1)
//		{
//			ChassisData.ChassisWheelSpeedRef[0] *= 0.7f;
//			ChassisData.ChassisWheelSpeedRef[1]	*= 0.7f;
//			ChassisData.ChassisWheelSpeedRef[2]	*= 0.7f;
//			ChassisData.ChassisWheelSpeedRef[3]	*= 0.7f;//原本是0.25
//			VAL_LIMIT(CM1SpeedPID.output,-4000,4000);
//			VAL_LIMIT(CM2SpeedPID.output,-4000,4000);
//			VAL_LIMIT(CM3SpeedPID.output,-4000,4000);
//			VAL_LIMIT(CM4SpeedPID.output,-4000,4000);
//		}
//		else if(slow_flag == 2)
//		{
//			ChassisData.ChassisWheelSpeedRef[0] *= 0.25f;
//			ChassisData.ChassisWheelSpeedRef[1]	*= 0.25f;
//			ChassisData.ChassisWheelSpeedRef[2]	*= 0.25f;
//			ChassisData.ChassisWheelSpeedRef[3]	*= 0.25f;//原本是0.25
//			VAL_LIMIT(CM1SpeedPID.output,-1000,1000);
//			VAL_LIMIT(CM2SpeedPID.output,-1000,1000);
//			VAL_LIMIT(CM3SpeedPID.output,-1000,1000);
//			VAL_LIMIT(CM4SpeedPID.output,-1000,1000);
//		}

//	else//没有裁判系统功率数据的时候
//	{
//		VAL_LIMIT(CM1SpeedPID.output,-3500,3500);
//		VAL_LIMIT(CM2SpeedPID.output,-3500,3500);
//		VAL_LIMIT(CM3SpeedPID.output,-3500,3500);
//		VAL_LIMIT(CM4SpeedPID.output,-3500,3500);
//	}
//}

/**
  * @brief  发弹限制
**/

void shoot_limitcal()
{
	switch (JudgingInfo.GameRobotState .robot_level)
	{
		case 1:
			if(JudgingInfo.PowerHeatData.shooter_heat0<220)
				LITTLE_shoot=shoot_ok ;
			else BIG_shoot=shoot_no ;
			if(JudgingInfo.PowerHeatData.shooter_heat1<50)
				BIG_shoot=shoot_ok ;
			else BIG_shoot=shoot_no ;

			;break ;
		case 2:
			if(JudgingInfo.PowerHeatData.shooter_heat0<340)
				LITTLE_shoot=shoot_ok ;
			else LITTLE_shoot=shoot_no ;
			
			if(JudgingInfo.PowerHeatData.shooter_heat1<50)
				BIG_shoot2=shoot_ok ;
			else if(JudgingInfo.PowerHeatData.shooter_heat1<150)
				BIG_shoot=shoot_ok ;
			else BIG_shoot=shoot_no ;
			;break ;
		case 3:
			if(JudgingInfo.PowerHeatData.shooter_heat0<460)
				LITTLE_shoot=shoot_ok ;
			else LITTLE_shoot=shoot_no ;
			if(JudgingInfo.PowerHeatData.shooter_heat1<200)
				BIG_shoot2=shoot_ok ;
			else if(JudgingInfo.PowerHeatData.shooter_heat1<300)
				BIG_shoot=shoot_ok ;
			else BIG_shoot=shoot_no ;
			;break ;	
	}
	
}








