#include "Nvic.h"
#include "main.h"
#include "stm32f4xx_hal.h"

void BSP_NVIC_Init(void)
{
//	HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 1, 1);
//	HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
//	HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 1, 1);
//	HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
//	HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 1, 1);
//	HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
//	HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 1, 1);
//	HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
//	HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 1, 1);
//	HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);


	HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);//����1
	HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);//����2
	HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART3_IRQn);//����3
	HAL_NVIC_SetPriority(USART6_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART6_IRQn);//����6
//	HAL_NVIC_SetPriority(UART7_IRQn, 0, 0);
//	HAL_NVIC_EnableIRQ(UART7_IRQn);//����7
	HAL_NVIC_SetPriority(UART8_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(UART8_IRQn);//����6

		
	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);//MPU�ⲿ�ж�

  HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
	HAL_NVIC_SetPriority(CAN1_TX_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);//CAN�����ж�
	
	HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
	HAL_NVIC_SetPriority(CAN2_TX_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(CAN2_TX_IRQn);//CAN�����ж�

	
		
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);//��ʱ��

	HAL_NVIC_SetPriority(ADC_IRQn  , 1, 1);
	HAL_NVIC_EnableIRQ(ADC_IRQn  );//MPU�ⲿ�ж�


	
}
