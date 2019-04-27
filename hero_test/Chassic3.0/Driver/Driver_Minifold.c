///* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Driver_Minifold.h"
#include "data.h"
#include "tim.h"
#include "imu_out.h"
#include "usart.h"
#include "StatusMachine.h"

///* Variables -----------------------------------------------------------------*/
CIRCLE_BUFF_t Que_MiniPC_Tx  = {0, 0, {0}};
MiniPC_Res MiniPC_Data={0,0,0,0,0,0};
CIRCLE_BUFF_t Que_Mainifold = {0, 0, {0}};
static uint8_t Res_OK = 0;//���ճɹ���־

//  ֡��ʽ 
//	֡ͷ AD  (1byte)0
//	����У׼��ŷ���� (�Ƕ�short int 2byte ��6���Ƕ� 12byte)1-12
//  Mode  (Ŀ��� 2byte)13-14     
//	֡β AE  (1byte)15
static uint8_t frame_temp_mini[30];
/**
  * @brief  ���ڽ������ݴ���
  * @param  uint8_t data
  * @retval None
  */
void Mainifold_Res_Task(void)
{
	while(bufferlen(&Que_Mainifold) != 0)
	{
		//Ѱ��֡ͷAD
		if(bufferPop(&Que_Mainifold,frame_temp_mini) == 0xFF)  
		{
			buffer_multiPop(&Que_Mainifold, &frame_temp_mini[1],6);
			if(bufferPop(&Que_Mainifold,&frame_temp_mini[7]) == 0xFE)	
			{
				Res_OK = 1;
			}
		}
		if(Res_OK)
		{
			MiniPC_Data.Yaw_Now = ((int16_t)(frame_temp_mini[3] << 8 | frame_temp_mini[2])) / 100.0f;
			MiniPC_Data.Pitch_Now = ((int16_t)(frame_temp_mini[5] << 8 | frame_temp_mini[4])) / 100.0f;
			MiniPC_Data.Distance_Now =  frame_temp_mini[6];
			MiniPC_Data.Flag_Get = frame_temp_mini[1];
			
			
			
//			Mainifold_RawData.Yaw_angle_Q   = ((int16_t)(frame_temp_mini[2]<<8 | frame_temp_mini[1]))/100.0f;
//			Mainifold_RawData.Pitch_angle_Q = ((int16_t)(frame_temp_mini[4]<<8 | frame_temp_mini[3]))/100.0f;
//			Mainifold_RawData.Yaw_angle_S   = ((int16_t)(frame_temp_mini[6]<<8 | frame_temp_mini[5]))/100.0f;
//			Mainifold_RawData.Pitch_angle_S = ((int16_t)(frame_temp_mini[8]<<8 | frame_temp_mini[7]))/100.0f;
//			Mainifold_RawData.Yaw_angle_C   = ((int16_t)(frame_temp_mini[10]<<8 | frame_temp_mini[9]))/100.0f;
//			Mainifold_RawData.Pitch_angle_C = ((int16_t)(frame_temp_mini[12]<<8 | frame_temp_mini[11]))/100.0f;
//			Mainifold_RawData.Mode = frame_temp_mini[14]<<8 | frame_temp_mini[13];
//			Mainifold_RawData.Used_Flag = 0;
			
			
			//MiniPCFrameCounter++;
	if(MiniPC_Data.Flag_Get == 1)		
		{
							MiniPC_Data.TimeStamp = Get_Time_Micros();//��¼�ɹ�ʶ���ʱ��
			}
		}
	} 
}

int16_t Yaw_Send,Pitch_Send;
//����DMA������֮��,ֱ�ӵ��ú��� ÿ����һ�μ�����һ֡
void Send_MiniPC(void)
{
	uint8_t Send_MiniPC[50] = {0};			//׼������Ҫ���͵�����
	uint8_t* pdata = Send_MiniPC;
	static uint8_t MiniPC_DMA_BUFF[200] = {0}; //DMAȡֵ������
	uint8_t cnt = 0;

	Yaw_Send=yaw_angle_out*100;
	Pitch_Send=pitch_angle_out*100;
	//����������֡��ֱ�����ȥ
	Send_MiniPC[cnt++] = 0xAA;
	Send_MiniPC[cnt++] = (uint8_t)(Yaw_Send>>8);
	Send_MiniPC[cnt++] = (uint8_t)Yaw_Send;
	Send_MiniPC[cnt++] = (uint8_t)(Pitch_Send>>8);
	Send_MiniPC[cnt++] = (uint8_t)Pitch_Send;
//	if(Gimbal_MoveMode == Gimbal_Mouse_Mode)
		Send_MiniPC[cnt++] = 0x01;
//	else
//		Send_MiniPC[cnt++] = 0x00;
	Send_MiniPC[cnt++] = 0xAD;
	for(int i = 0;i<cnt;i++)
	{
		bufferPush(&Que_MiniPC_Tx, pdata[i]);//����д�����
	}
	
	uint8_t Protocol_len = bufferlen(&Que_MiniPC_Tx);//�������ݳ���
	buffer_multiPop(&Que_MiniPC_Tx, MiniPC_DMA_BUFF, Protocol_len);//�Ӷ�����ȡ����������
//	HAL_UART_Transmit_DMA(&huart8, MiniPC_DMA_BUFF, Protocol_len);//����DMA����  ���ڴ���huartx
	 HAL_UART_Transmit(&huart8, MiniPC_DMA_BUFF, Protocol_len,10);
}
