#include "SuperviseTask.h"
#include "tim.h"
uint16_t DBUSFrameRate = 0;
uint16_t DBUSFrameCounter = 0;

uint16_t ChassisFrameRate[4] = {0};
uint16_t ChassisFrameCounter[4] = {0};

uint16_t PitchFrameRate = 0;
uint16_t PitchFrameCounter = 0;
	
uint16_t YawFrameRate = 0;
uint16_t YawFrameCounter = 0;

uint16_t TurntableFrameRate = 0;
uint16_t TurntableFrameCounter = 0;

uint16_t IMUFrameRate = 0;
uint16_t IMUFrameCounter = 0;

uint16_t UART2FrameRate = 0;
uint16_t UART2FrameCounter = 0;

uint16_t CAN_Send1_FrameRate = 0;
uint16_t CAN_Send1_FrameCounter = 0;

uint16_t CAN_Send2_FrameRate = 0;
uint16_t CAN_Send2_FrameCounter = 0;


uint16_t CAN_Res_FrameRate = 0;
uint16_t CAN_Res_FrameCounter = 0;

uint16_t JudgeFrameRate = 0;
uint16_t JudgeFrameCounter = 0;

uint16_t MiniPCFrameRate = 0;
uint16_t MiniPCFrameCounter = 0;

uint16_t IMU_Out_FrameRate = 0;
uint16_t IMU_Out_FrameCounter = 0;

uint16_t can1_res=0;

void Task_Monitor(void)
{
//******************  ��   ����֡��ͳ��   ��  **************************/
	static uint32_t time_mark = 0;
	if(Get_Time_Micros() - time_mark > 500000)
	{
		
		

		time_mark = Get_Time_Micros();
        //DBUS֡��ͳ��
        DBUSFrameRate = DBUSFrameCounter*2;	
        DBUSFrameCounter = 0;
        
        //��̨���֡��ͳ��
        PitchFrameRate    = PitchFrameCounter*2;
		    PitchFrameCounter = 0;
		
        YawFrameRate	  = YawFrameCounter*2;//����
        YawFrameCounter   = 0;
        can1_res=0;
        //���̵��֡��ͳ��
        ChassisFrameRate[0] = ChassisFrameCounter[0]*2;
        ChassisFrameCounter[0] = 0;
        ChassisFrameRate[1] = ChassisFrameCounter[1]*2;
        ChassisFrameCounter[1] = 0;
        ChassisFrameRate[2] = ChassisFrameCounter[2]*2;
        ChassisFrameCounter[2] = 0;
        ChassisFrameRate[3] = ChassisFrameCounter[3]*2;
        ChassisFrameCounter[3] = 0;
        
				//CAN����
        CAN_Send1_FrameRate = CAN_Send1_FrameCounter*2;
        CAN_Send1_FrameCounter = 0;
				
				CAN_Send2_FrameRate = CAN_Send1_FrameCounter*2;
        CAN_Send2_FrameCounter = 0;

				//CAN����
        CAN_Res_FrameRate = CAN_Res_FrameCounter*2;
        CAN_Res_FrameCounter = 0;

        //����ϵͳ֡��ͳ��
        JudgeFrameRate = JudgeFrameCounter*2;
        JudgeFrameCounter = 0;
		
		    //IMU֡��ͳ��
        IMUFrameRate = IMUFrameCounter*2;
        IMUFrameCounter = 0;
				//�ⲿIMU֡��ͳ��
        IMU_Out_FrameRate = IMU_Out_FrameCounter*2;
        IMU_Out_FrameCounter = 0;
		
		//����֡�ʼ��
		MiniPCFrameRate = MiniPCFrameCounter*2;
		MiniPCFrameCounter = 0;
	}
        
}
