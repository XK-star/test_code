#ifndef __MAINIFOLD_H
#define __MAINIFOLD_H
#include "main.h"
typedef struct
{
//	float Yaw_angle_Q;//У׼������
//	float Pitch_angle_Q;
//	float Yaw_angle_S;
//	float Pitch_angle_S;
//	float Yaw_angle_C;
//	float Pitch_angle_C;
//	int16_t Mode;				//û�м�⵽Ϊ0 ����⵽��Ϊ��Ӧ���
//	uint8_t Used_Flag;		//������Ϣ��ʹ�ù��ı�־ 1��ʹ�ù�   0��δʹ��
//	uint32_t TimeStamp;		//ʶ��ɹ���ʱ���
		float Yaw_Now;	//δ�����ֵ
	  float Pitch_Now;
	  uint32_t Distance_Now;	//��λ������
	  uint8_t Flag_Get;
	uint8_t Used_Flag;		//������Ϣ��ʹ�ù��ı�־ 1��ʹ�ù�   0��δʹ��
	uint32_t TimeStamp;		//ʶ��ɹ���ʱ���


}MiniPC_Res;

extern MiniPC_Res MiniPC_Data;

extern uint8_t Mainifold_count;


void Mainifold_Res_Anl(void);
void Mainifold_Res_Task(void);
void Send_MiniPC(void);

#endif

