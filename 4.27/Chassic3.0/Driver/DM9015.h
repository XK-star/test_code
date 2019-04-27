#ifndef __DM9015_H_
#define __DM9015_H_
#include "stdint.h"
#include "usart.h"



#define RATE_BUF_SIZE 6

typedef struct{
	int32_t raw_value;   									//���������������ԭʼֵ
	int32_t last_raw_value;								//��һ�εı�����ԭʼֵ
	int32_t ecd_value;                       //��������������ı�����ֵ
	int32_t diff;													//���α�����֮��Ĳ�ֵ
	int32_t temp_count;                   //������
	uint8_t buf_count;								//�˲�����buf��
	int32_t ecd_bias;											//��ʼ������ֵ	
	int32_t ecd_raw_rate;									//ͨ������������õ����ٶ�ԭʼֵ
	int32_t rate_buf[RATE_BUF_SIZE];	
	int32_t round_cnt;										//Ȧ��
	int32_t filter_rate;											//�ٶ�
	float ecd_angle;											//�Ƕ�
	float last_ecd_angle;
}DM_Encoder;

void DMfold_Res_Task(void);
void DM_SendPower(int16_t power);
void DM_SendAngularSpeed(int32_t angularspeed);
void DM_SendAngle(int64_t angle);
void DM_SendPosition(int64_t angle,int32_t angularspeed);


#endif
