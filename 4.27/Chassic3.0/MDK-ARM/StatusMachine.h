#ifndef  __STATUSMACHINE_H
#define  __STATUSMACHINE_H

typedef enum
{
	KEY_MOUSE_INPUT,
	STOP,
	REMOTE_INPUT,
}InputMode_e;

typedef enum
{												//															
  PREPARE_STATE,     		//�ϵ���ʼ��״̬ 4s������			
  NORMAL_AUTO_STATE,			//ȫ�Զ�ģʽ
  NORMAL_RC_STATE,			//ң����ģʽ //attackmode
	STOP_STATE,        	//ֹͣ�˶�״̬   
	CALI_STATE,    			//У׼״̬
	TEST_STATE,
}WorkState_e;

typedef enum//									Notmal									
{															
	Attack_Normal,//��ʼ����
	Attack_Lop,		//����
	Attack_CatWalk,   //è��
	Attack_Slow,     //����վ
	Attack_45Degree,  //45
	Attack_None,
}AttackMode_e;

typedef enum                 //lock
{
	GM_CM_Lock,
	GM_CM_Notlock,
}GM_CM_Mode_e;		//��̨��������ģʽ

typedef enum                 //perpare
{
	Gimbal_Prepare,
	Gimbal_Stop,
	Gimbal_RC_Mode,
	Gimbal_Mouse_Mode,
	Gimbal_Auto,
	Gimbal_Cali,
	Gimbal_Test,
}Gimbal_MoveMode_t;


typedef enum
{
  Chassis_Prepare,
	Chassis_Stop,
	Chassis_RC_Mode,
	Chassis_Mouse_Mode,
	Chassis_Auto,            //�ռ�ȫ�Զ�ģʽ
	Chassis_Test,
}Chassis_MoveMode_t;

typedef enum
{
	FRICTION_WHEEL_OFF = 0,
	FRICTION_WHEEL_TURNNING = 1,
	FRICTION_WHEEL_ON = 2,
}FrictionWheelState_e;

typedef enum
{
	NOSHOOTING = 0,
	SHOOTING = 1,
	
}Shoot_State_e;

void StatusMachine_Update(void);

InputMode_e GetInputMode(void );
WorkState_e GetWorkState(void);
AttackMode_e Get_AttackMode(void);
GM_CM_Mode_e GetGM_CM_Mode(void);
Gimbal_MoveMode_t GetGimbal_MoveMode(void);
Chassis_MoveMode_t GetChassis_MoveMode(void);
FrictionWheelState_e GetFrictionState(void);
Shoot_State_e GetShootState(void);


void SetWorkState(WorkState_e workState);
	void StatusMachine_Init(void);


extern InputMode_e         InputMode;
extern WorkState_e         WorkState;
extern WorkState_e     lastWorkstate;
extern AttackMode_e       Attack_Mode;
extern GM_CM_Mode_e       GM_CM_Mode;
extern Gimbal_MoveMode_t  Gimbal_MoveMode;
extern Chassis_MoveMode_t  Chassis_MoveMode;
extern Shoot_State_e  ShootState;

#endif
