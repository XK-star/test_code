/****************************�ȼ�Ѫ��������������**********************************/
/*






*/


#include "Driver_LevelLimit.h" 
#include "Driver_Judge.h"

SHOOT_JUDGE_e BIG_shoot;
SHOOT_JUDGE_e LITTLE_shoot;
SHOOT_JUDGE_e BIG_shoot2;

///**
//  * @brief  ��������ģʽ ���
//  * @param  float vx X�����ϵ��ٶ�
//  * @param  float vy Y�����ϵ��ٶ�
//  * @param  float omega ��ת���ٶ�
//  * @param  float radian ��ʱ���������ĽǶ�(����)��˳ʱ��Ϊ����
//  * @param  int16_t maxspped  ����ٶ�
//  */
//  #define Power_Max 100
//#define Energy_Min  30
//#define Energy_Dead 10
//extern float OP_ChassisCurrent;
//void Power_Limit_OutputCal()
//{	
//	float t_realPower = JudgingInfo.PowerHeatData.chassisPower;	//ʵʱ����
//	float t_realEnergy = JudgingInfo.PowerHeatData.chassisPowerBuffer;
////	float t_real_adcPower = OP_ChassisCurrent*24;

//		if(t_realPower > 80)
//		{
//			//120W˥��
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
//		if(slow_flag == 0)	//δ����˥��ģʽ
//		{
//			if(t_realEnergy < Energy_Min)
//				slow_flag = 1;//��ͨ˥��
//			else if(t_realEnergy < Energy_Dead)
//				slow_flag = 2; //����˥��
//		}
//		else if(t_realEnergy >50)//�˳�˥��ģʽ
//		{
//			slow_flag = 0;
//		}
//		if(slow_flag == 1)
//		{
//			ChassisData.ChassisWheelSpeedRef[0] *= 0.7f;
//			ChassisData.ChassisWheelSpeedRef[1]	*= 0.7f;
//			ChassisData.ChassisWheelSpeedRef[2]	*= 0.7f;
//			ChassisData.ChassisWheelSpeedRef[3]	*= 0.7f;//ԭ����0.25
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
//			ChassisData.ChassisWheelSpeedRef[3]	*= 0.25f;//ԭ����0.25
//			VAL_LIMIT(CM1SpeedPID.output,-1000,1000);
//			VAL_LIMIT(CM2SpeedPID.output,-1000,1000);
//			VAL_LIMIT(CM3SpeedPID.output,-1000,1000);
//			VAL_LIMIT(CM4SpeedPID.output,-1000,1000);
//		}

//	else//û�в���ϵͳ�������ݵ�ʱ��
//	{
//		VAL_LIMIT(CM1SpeedPID.output,-3500,3500);
//		VAL_LIMIT(CM2SpeedPID.output,-3500,3500);
//		VAL_LIMIT(CM3SpeedPID.output,-3500,3500);
//		VAL_LIMIT(CM4SpeedPID.output,-3500,3500);
//	}
//}

/**
  * @brief  ��������
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








