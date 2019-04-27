#include "main.h"
#include "Driver_CurrentMeasure.h"
#include "BSP_Init.h"
#include "stm32f4xx_hal.h"

#define ADC_ENABLE
extern ADC_HandleTypeDef hadc1;

float OP_ChassisCurrent = 0;//���̵����Ƶ���ֵ
uint32_t OP_ADCFilter_Value = 0;
//��ȡ���̵����Ƶĵ�ѹֵ
void ADC_StartMeasure(void)
{
	#ifdef ADC_ENABLE
	HAL_ADC_Start_IT(&hadc1);
	#else
	OP_ChassisCurrent = 0;
	#endif
}


#define FILTER_DEEP 30
void ADC_Cal_Current(uint32_t adc_value)
{
	static uint32_t Filter_Buff[FILTER_DEEP] = {0};//ʮ�黬���˲� 
	for(uint16_t i = 0;i< FILTER_DEEP-1;i++)		//����ֵ		
	{
		Filter_Buff[i] = Filter_Buff[i+1];
	}
	
	Filter_Buff[FILTER_DEEP-1] = adc_value;
	
	uint32_t sum =0;//���
	for(uint8_t i = 0;i<FILTER_DEEP;i++)
	{
		sum += Filter_Buff[i];
	}
	OP_ADCFilter_Value = sum*1.0f/FILTER_DEEP;
	
	OP_ChassisCurrent = OP_ADCFilter_Value*(3.30f/4095/98/0.010f)*1.1f;//ADC��ѹ/�˷ŷŴ�98/��������0.005��
}



