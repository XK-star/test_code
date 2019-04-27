#ifndef __PID_H
#define __PID_H

#define YAW_POSITION_KP_DEFAULTS  12
#define YAW_POSITION_KI_DEFAULTS  0
#define YAW_POSITION_KD_DEFAULTS  0

#define YAW_SPEED_KP_DEFAULTS  25//  25
#define YAW_SPEED_KI_DEFAULTS  15
#define YAW_SPEED_KD_DEFAULTS  0

// avoid bang --->  position:20.0  speed:19.0
//big bang   22.5 20.0
#define PITCH_POSITION_KP_DEFAULTS  15
#define PITCH_POSITION_KI_DEFAULTS  0
#define PITCH_POSITION_KD_DEFAULTS  0

#define PITCH_SPEED_KP_DEFAULTS  5
#define PITCH_SPEED_KI_DEFAULTS  0
#define PITCH_SPEED_KD_DEFAULTS  0

#define CHASSIS_SPEED_KP_DEFAULTS  0//28
#define CHASSIS_SPEED_KI_DEFAULTS  0
#define CHASSIS_SPEED_KD_DEFAULTS  0

#define CHASSIS_ROTATE_KP_DEFAULTS  0//14
#define CHASSIS_ROTATE_KI_DEFAULTS  0
#define CHASSIS_ROTATE_KD_DEFAULTS  0


#define SHOOT_SPEED_KP_DEFAULTS  1.5
#define SHOOT_SPEED_KI_DEFAULTS  0
#define SHOOT_SPEED_KD_DEFAULTS  0

#define BULLET_TURNTABLE_KP_DEFAULTS  4000
#define BULLET_TURNTABLE_KI_DEFAULTS  0
#define BULLET_TURNTABLE_KD_DEFAULTS  0

#define FRIMOTOR_KP_DEFAULTS 18
#define FRIMOTOR_KI_DEFAULTS 3
#define FRIMOTOR_KD_DEFAULTS 4

#define PID_TURNTABLE_SPEED_INIT (7)
#define PID_TURNTABLE_SPEED      (1000)	//���̵�� һ������
#define PID_TURNTABLE_CHECK_SPEED (3)

#define CHASSIS_SPEED_ATTENUATION   (1.0f)

#define CHASSIS_MOTOR_ROTATE_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	CHASSIS_ROTATE_KP_DEFAULTS,\
	CHASSIS_ROTATE_KI_DEFAULTS,\
	CHASSIS_ROTATE_KD_DEFAULTS,\
	0,\
	0,\
	0,\
	4900,\
	1000,\
	1500,\
	0,\
	600,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}

//gimbal position pid control
//20  19
#define GIMBAL_MOTOR_PITCH_POSITION_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	PITCH_POSITION_KP_DEFAULTS,\
	PITCH_POSITION_KI_DEFAULTS,\
	PITCH_POSITION_KD_DEFAULTS,\
	0,\
	0,\
	0,\
	4900,\
	1000,\
	1500,\
	0,\
	4900,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}

//gimbal speed pid control
#define GIMBAL_MOTOR_PITCH_SPEED_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	PITCH_SPEED_KP_DEFAULTS,\
	PITCH_SPEED_KI_DEFAULTS,\
	PITCH_SPEED_KD_DEFAULTS,\
	0,\
	0,\
	0,\
	4900,\
	1000,\
	1500,\
	0,\
	28000,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}

//gimbal yaw position pid control
#define GIMBAL_MOTOR_YAW_POSITION_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	YAW_POSITION_KP_DEFAULTS,\
	YAW_POSITION_KI_DEFAULTS,\
	YAW_POSITION_KD_DEFAULTS,\
	0,\
	0,\
	0,\
	4900,\
	1000,\
	1500,\
	0,\
	800,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}

//gimbal yaw speed pid control
#define GIMBAL_MOTOR_YAW_SPEED_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	YAW_SPEED_KP_DEFAULTS,\
	YAW_SPEED_KI_DEFAULTS,\
	YAW_SPEED_KD_DEFAULTS,\
	0,\
	0,\
	0,\
	4900,\
	1000,\
	1500,\
	0,\
	850,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}

//D����ԭ��Ϊ0.4
#define CHASSIS_MOTOR_SPEED_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	CHASSIS_SPEED_KP_DEFAULTS,\
	CHASSIS_SPEED_KI_DEFAULTS,\
	CHASSIS_SPEED_KD_DEFAULTS,\
	0,\
	0,\
	0,\
	4900,\
	1000,\
	1500,\
	0,\
	8000,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}

#define SHOOT_MOTOR_SPEED_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	SHOOT_SPEED_KP_DEFAULTS,\
	SHOOT_SPEED_KI_DEFAULTS,\
	SHOOT_SPEED_KD_DEFAULTS,\
	0,\
	0,\
	0,\
	1000,\
	600,\
	100,\
	0,\
	8500,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}

#define SHOOT_MOTOR_POSITION_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	SHOOT_POSITION_KP_DEFAULTS,\
	SHOOT_POSITION_KI_DEFAULTS,\
	SHOOT_POSITION_KD_DEFAULTS,\
	0,\
	0,\
	0,\
	1000,\
	600,\
	100,\
	0,\
	8500,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}

#define FRIMOTOR_PID_DEFAULT \
{	\
	0,\
	0,\
	{0,0,0},\
	FRIMOTOR_KP_DEFAULTS,\
	FRIMOTOR_KI_DEFAULTS,\
	FRIMOTOR_KD_DEFAULTS,\
	0,\
	0,\
	0,\
	4900,\
	1000,\
	1500,\
	0,\
	8000,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}

#define BULLET_TURNTABLE_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	BULLET_TURNTABLE_KP_DEFAULTS,\
	BULLET_TURNTABLE_KI_DEFAULTS,\
	BULLET_TURNTABLE_KD_DEFAULTS,\
	0,\
	0,\
	0,\
	5000,\
	1000,\
	1500,\
	0,\
	9000,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}

#define PID_CALI_DEFAULT  \
{\
	0,\
	0,\
	0,\
	0,\
}

#define	IMU_POSITION_KP_DEFAULT 2//���ֵ�Ͷ�ʱ����Ƶ���й�//ǣ�����ܶණ��
#define	IMU_POSITION_KI_DEFAULT 0//���ɼӻ���
#define IMU_POSITION_KD_DEFAULT 0

#define IMU_HEAT_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0,0},\
	IMU_POSITION_KP_DEFAULT,\
	IMU_POSITION_KI_DEFAULT,\
	IMU_POSITION_KD_DEFAULT,\
	0,\
	0,\
	0,\
	400,\
	6,\
	10,\
	0,\
	2500,\
	0,\
	0,\
	0,\
	&PID_Calc,\
	&PID_Reset,\
}//������˶�ʱ��3��Ԥװֵ���ǵû���Ҫ��pid������ֵ//���������pidϵ�����ֵҲҪע��

typedef struct PID_Regulator_t
{
	float ref;
	float fdb;
	float err[3];
	float kp;
	float ki;
	float kd;
	float componentKp;
	float componentKi;
	float componentKd;
	float componentKpMax;
	float componentKiMax;
	float componentKdMax;
	float output;
	float outputMax;
	float kp_offset;
	float ki_offset;
	float kd_offset;
	void (*Calc)(struct PID_Regulator_t *pid);//����ָ��
	void (*Reset)(struct PID_Regulator_t *pid);
}PID_Regulator_t;

void PID_Reset(PID_Regulator_t *pid);
void PID_Calc(PID_Regulator_t *pid);
float PID_Task(PID_Regulator_t *PID_Stucture, float ref, float fdb);
float PID_Task_test(PID_Regulator_t *PID_Stucture, float ref, float fdb);
#endif
