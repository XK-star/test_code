#include "main.h"
#include "Driver_CurrentMeasure.h"
#include "BSP_Init.h"
#include "stm32f4xx_hal.h"

#define ADC_ENABLE
extern ADC_HandleTypeDef hadc1;

float OP_ChassisCurrent = 0;//底盘电流计电流值
uint32_t OP_ADCFilter_Value = 0;
//获取底盘电流计的电压值
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
	static uint32_t Filter_Buff[FILTER_DEEP] = {0};//十组滑动滤波 
	for(uint16_t i = 0;i< FILTER_DEEP-1;i++)		//更新值		
	{
		Filter_Buff[i] = Filter_Buff[i+1];
	}
	
	Filter_Buff[FILTER_DEEP-1] = adc_value;
	
	uint32_t sum =0;//求和
	for(uint8_t i = 0;i<FILTER_DEEP;i++)
	{
		sum += Filter_Buff[i];
	}
	OP_ADCFilter_Value = sum*1.0f/FILTER_DEEP;
	
	OP_ChassisCurrent = OP_ADCFilter_Value*(3.30f/4095/98/0.010f)*1.1f;//ADC电压/运放放大倍98/采样电阻0.005Ω
}



