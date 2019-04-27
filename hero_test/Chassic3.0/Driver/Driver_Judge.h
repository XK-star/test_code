#ifndef __JUDGINGSYSTEM_H
#define __JUDGINGSYSTEM_H
#include "main.h"
#include "cmsis_os.h"

#define NEW_JUDGE

//uartͨ�����ã�������115200,����λ8,ֹͣλ1������λ�ޣ���������

//ͨ��Э���ʽ
//FrameHeader(4-Byte) 
//CmdID(2-Byte) 
//Data(n-Byte) 
//FrameTail(2-Byte, CRC16)


/************************************NEW**************************************/
#ifdef NEW_JUDGE

#define JudgeBufferLength       50
#define JudgeFrameLength_1      44
#define JudgeFrameLength_2      12
#define JudgeFrameLength_3      25
#define JudgeFrameLength_4      25

#define HEADER_LEN   5
#define CMD_LEN      2    //cmdid bytes
#define CRC_LEN      2    //crc16 bytes

typedef enum
{
  GAME_STATE_ID         = 0x0001,  //10Hz
  GAME_RESULT_ID        = 0x0002,  //ending
  ROBOT_SURVIVORS_ID    = 0x0003,  //1hz
  EVENT_DATA_ID         = 0x0101,  //
  SUPPLY_ACTIION_ID     = 0x0102,  //	
  SUPPLY_BOOKING_ID     = 0x0103,//δ����
  ROBOT_STATE_ID        = 0x0201,//10hz
  REAL_POWER_HEAT_ID    = 0x0202,//50hz
	REAL_ROBOT_POS_ID     = 0x0203,//10hz
	BUFF_MUSK_ID          = 0x0204,//
	ROBOT_ENERGY_ID       = 0x0205,//10hz
	ROBOT_HURT_ID         = 0x0206,//
	REAL_SHOOT_ID         = 0x0207,//
	
} judge_data_id_e;

typedef __packed struct
{
 uint8_t game_type : 4;
 uint8_t game_progress : 4;
 uint16_t stage_remain_time;
} ext_game_state_t;

typedef __packed struct
{
 uint8_t winner;
} ext_game_result_t;

typedef __packed struct
{
 uint16_t robot_legion;
} ext_game_robot_survivors_t;

typedef __packed struct
{
 uint32_t event_type;
} ext_event_data_t;

typedef __packed struct
{
 uint8_t supply_projectile_id; 
 uint8_t supply_robot_id; 
 uint8_t supply_projectile_step; 
uint8_t supply_projectile_num;
} ext_supply_projectile_action_t;

typedef __packed struct
{
 uint8_t supply_projectile_id;
 uint8_t supply_robot_id; 
uint8_t supply_num;
} ext_supply_projectile_booking_t;

typedef __packed struct
{
 uint8_t robot_id;
 uint8_t robot_level;
 uint16_t remain_HP;
 uint16_t max_HP;
 uint16_t shooter_heat0_cooling_rate;
 uint16_t shooter_heat0_cooling_limit;
 uint16_t shooter_heat1_cooling_rate;
 uint16_t shooter_heat1_cooling_limit;
 uint8_t mains_power_gimbal_output : 1;
 uint8_t mains_power_chassis_output : 1;
 uint8_t mains_power_shooter_output : 1;
} ext_game_robot_state_t;

typedef __packed struct
{
 uint16_t chassis_volt; 
 uint16_t chassis_current; 
 float chassisPower; 
 uint16_t chassisPowerBuffer; 
 uint16_t shooter_heat0; //С����
 uint16_t shooter_heat1; //����
} ext_power_heat_data_t;

typedef __packed struct
{
 float x;
 float y;
 float z;
 float yaw;
} ext_game_robot_pos_t;

typedef __packed struct
{
 uint8_t power_rune_buff;
}ext_buff_musk_t;

typedef __packed struct
{
 uint8_t energy_point;
 uint8_t attack_time;
} aerial_robot_energy_t;

typedef __packed struct
{
 uint8_t armor_id : 4;
 uint8_t hurt_type : 4;
} ext_robot_hurt_t;

typedef __packed struct
{
 uint8_t bullet_type;
 uint8_t bullet_freq;
 float bullet_speed;
} ext_shoot_data_t;

//���ؽ���
typedef __packed struct
{
 uint16_t data_cmd_id;
 uint16_t send_ID;
 uint16_t receiver_ID;
}ext_student_interactive_header_data_t;

//�ͻ����Զ�������
typedef __packed struct
{
float data1;
float data2;
float data3;
uint8_t masks;
} client_custom_data_t;

typedef struct
{
    ext_game_robot_state_t GameRobotState;
    ext_robot_hurt_t RobotHurt;
    ext_shoot_data_t ShootData;
    ext_power_heat_data_t PowerHeatData;
    //extRfidDetect_t RfidDetect;
    ext_game_result_t GameResult;
    ext_buff_musk_t GetBuff;
    //extShowData_t ShowData;
} JudgingInfo_t;


/*********************************OLD******************************************/
#else

#define JudgeBufferLength       50
#define JudgeFrameLength_1      44
#define JudgeFrameLength_2      12
#define JudgeFrameLength_3      25
#define JudgeFrameLength_4      25

#define HEADER_LEN   5
#define CMD_LEN      2    //cmdid bytes
#define CRC_LEN      2    //crc16 bytes

typedef enum
{
  GAME_INFO_ID       = 0x0001,  //10Hz
  REAL_BLOOD_DATA_ID = 0x0002,
  REAL_SHOOT_DATA_ID = 0x0003,
  REAL_POWER_DATA_ID = 0x0004,  //50hz
  REAL_FIELD_DATA_ID = 0x0005,  //10hZ
  GAME_RESULT_ID     = 0x0006,
  GAIN_BUFF_ID       = 0x0007,
  
  STU_CUSTOM_DATA_ID = 0x0100,
} judge_data_id_e;


typedef __packed struct
{
    uint8_t validFlag;
	float x;
    float y;
    float z;
    float yaw;
} positon_t;

typedef __packed struct
{
    uint16_t stageRemianTime;
    uint8_t gameProgress;
    uint8_t robotLevel;
    uint16_t remainHP;
    uint16_t maxHP;
    positon_t position;
} extGameRobotState_t;

typedef __packed struct
{
    uint8_t armorType : 4;
    uint8_t hurtType : 4;
} extRobotHurt_t;

typedef __packed struct
{
    uint8_t bulletType;
    uint8_t bulletFreq;
    float bulletSpeed;
    float reserved;
} extShootData_t;

typedef __packed struct
{
    float chassisVolt;
    float chassisCurrent;
    float chassisPower;
    float chassisPowerBuffer;
    uint16_t shooterHeat0;
    uint16_t shooterHeat1;
} extPowerHeatData_t;

typedef __packed struct
{
    uint8_t cardType;
    uint8_t cardIdx;
} extRfidDetect_t;

typedef __packed struct
{
    uint8_t winner;
} extGameResult_t;

typedef __packed struct
{
    uint8_t buffType;
    uint8_t buffAddition;
} extGetBuff_t;

typedef __packed struct
{
    float data1;
    float data2;
    float data3;
    uint8_t mask;
} extShowData_t;

//typedef __packed struct 
//{
//    uint8_t flag;    //0��Ч 1��Ч
//    uint32_t x; 
//    uint32_t y;    
//    uint32_t z; 
//    uint32_t compass;
//}tLocData;

//typedef __packed struct
//{ 
//    uint32_t remainTime;        //����ʣ��ʱ��
//    uint16_t remainLifeValue;   //������ʣ��Ѫ��
//    float  realChassisOutV;     //ʵʱ���������ѹ
//    float  realChassisOutA;     //ʵʱ�����������
//    tLocData locData;           //��λ��Ϣ
//    float remainPower;          //ʣ������ ����60J
//}tGameInfo;         //����������Ϣ  ����Ƶ��50Hz

//typedef __packed struct 
//{
//    uint8_t   weakId:4; //ǰ0 ��1 ��2 ��3 ��һ4 �϶�5
//    uint8_t   way:4;    //װ���˺�0 �ӵ�����1 �ӵ���Ƶ2 ������3 ģ������4 ��ͨ����6 ͣ��ƺ��Ѫ0xa ���̳���Ѫ0xb
//    uint16_t  value; 	//�仯ֵ
//}tRealBloodChangedData;

//typedef __packed struct 
//{ 
//    float realBulletShootSpeed;
//    float realBulletShootFreq; 
//    float realGolfShootSpeed; 
//    float realGolfShootFreq; 
//}tRealShootData;

typedef struct
{
    extGameRobotState_t GameRobotState;
    extRobotHurt_t RobotHurt;
    extShootData_t ShootData;
    extPowerHeatData_t PowerHeatData;
    extRfidDetect_t RfidDetect;
    extGameResult_t GameResult;
    extGetBuff_t GetBuff;
    extShowData_t ShowData;
} JudgingInfo_t;

///** 
//  * @brief  ������ʼ��Ϣ
//  */
//typedef struct
//{
//	int8_t s8_StartCntDwnFlg;
//	int8_t s8_StartFlg;
//	int8_t s8_TimoutFlg;
//	int8_t s8_EndFlg;
//	
//	uint32_t Blood_Change_Time;//����Ѫ���仯��ʱ����//ms
//	uint32_t lifeval_attack_time;//Ѫ���仯ʱ������¼//us
//	uint8_t BeingAttackFlg;	//������ı�־
//	uint8_t BulletType;		//�ӵ����� 0 С�ӵ�  1�߶���
//}GameStartTypeDef;


//typedef struct
//{
//	uint8_t Infantry;
//	uint32_t Infantry_Attack_Time;//�����������ʱ���
//	
//	uint8_t Hero;
//	uint32_t Hero_Attack_Time;
//	
//	uint8_t Drone;
//	uint32_t Drone_Attack_Time;
//	
//	u16 ValuePerSecond;
//	uint8_t TooManyAnemy;
//	uint32_t TooManyAnemy_Attack_Time;
//}Enemy_Appear_Flag_t;


#endif

unsigned int Verify_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength) ;
void Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
uint32_t Verify_CRC16_Check_Sum(uint8_t  *pchMessage, uint32_t  dwLength) ;
void Append_CRC16_Check_Sum(uint8_t * pchMessage,uint32_t dwLength) ;

void JudgeData_anl(uint8_t *pData);
uint8_t JudgingSystem_DataFrame_Process(uint8_t com_data);
void JudgingSystem_DataFrame_Process_DMA(uint8_t *com_data);
void JudgingSystem_DataFrame_Process_NEW(uint8_t *com_data, uint8_t DataLength);
void JudgingSystem(void);
void JudgeSend(void);
void Judge(void);

//extern Enemy_Appear_Flag_t Enemy_Appear_Flag;
extern JudgingInfo_t JudgingInfo;

extern uint8_t Judge_TxBuffer[256];
extern uint8_t Judge_TxCounter;
extern uint8_t Judge_count;

extern uint8_t HP_Used;


void Judge_Task(void);
#endif

