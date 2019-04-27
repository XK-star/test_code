#ifndef __DRIVER_FRIMOTOR_H
#define __DRIVER_FRIMOTOR_H
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"

#define BULLET_SPEED 1575
#define MOTOR_SPEED 10000
#define START_EDCANGLE 4910
extern int16_t frimotor_speed;
extern uint8_t EGG_HOME_State ;

#define PWM1  TIM5->CCR2
#define PWM2  TIM5->CCR3

#define InitFrictionWheel()     \
        PWM1 = 800;             \
        PWM2 = 800;			\

#define SetFrictionWheelSpeed(x) \
        PWM1 = x;                \
        PWM2 = x;				\
				frimotor_speed = 0;     \
				
#define SetFrictionWheelSpeed3508(x) \
        frimotor_speed = x; \

#define REDUCTION 36.0f //减速比
#define REDUCTION2006 36.0f //减速比
#define REDUCTION3508 19.0f //减速比
						
typedef struct
{
	int16_t Speed;
	float	Angle;
}Turntale_ref_t;

void FriMotor_Control(void);
void Turntable_Control(void);
void Shoot_Single(uint8_t num);
void LASER_Task(void);
void Bullter_Single(uint8_t num);
		
extern uint8_t  BulletNumber;
extern uint8_t shootdouble;
extern uint8_t shootnum;
extern uint8_t turn_flag;

#endif
