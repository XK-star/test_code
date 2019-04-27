#ifndef __DRIVER_LEVELLIMIT_H_
#define __DRIVER_LEVELLIMIT_H_

void Power_Limit_OutputCal();

typedef enum
{
	shoot_no,
	shoot_ok,
}SHOOT_JUDGE_e;


extern SHOOT_JUDGE_e BIG_shoot;
extern SHOOT_JUDGE_e LITTLE_shoot;
extern SHOOT_JUDGE_e BIG_shoot2;

#endif
