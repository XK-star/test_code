1.HAL_CAN.c �ļ��иģ�ʹcan�жϿɷ�������

USART1 ң����
USART2 DM90
UASRT6 ����IMU

����ʱ�������

YAW  DM90
PITCH 6623

TIM2 Get_Time_Micros()  �Զ���װ��ֵ32λ

SPI5 ����mpu6050

�����������ã�
TIM2-CH1-4
TIM3-CH2   MPU���ȵ���
TIM8-CH1-4
TIM12-CH1  ������
TIM5-CH1 B PWM1
TIM5-CH2 C PWM2

USART1 ң����
USART2 ��λ��
USART3 
USART6 ����ϵͳ
UART7 
USRT8 

I2C2 
SPI4 ����mpu6050
ADC1
DAC


�Զ���LED Green -PF14   PG1-PG8
                 Red    -PE11
   
��·�ɿص�Դ��PH2-5 ��1ʹ��

GM9015
VCC 7.4~32V
ID 1-4
Ĭ�ϲ����� 115200

����ϵͳ 
��stm32f427xx.h�¸���
66  #define __NVIC_PRIO_BITS          4       /*!< STM32F4XX uses 4 Bits for the Priority Levels */
core_cm4.h��
247 #define __NVIC_PRIO_BITS          4U                    

���������ж�
stm32f427xx_hal_uart.c 
985 //    SET_BIT(huart->Instance->CR3, USART_CR3_EIE);

i2cʱ��ʹ�ܷ���GPIO��ǰ
i2c.c
      /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();


mcuPID






