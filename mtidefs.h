#ifndef _MT_IDEFS_H_
#define _MT_IDEFS_H_

typedef enum
{
	eMT_State_INIT = 0,
	eMT_State_IDLE,
	eMT_State_STEP,
	eMT_State_GOTO,

	eMT_NUM_STATES
} MotorTask_Sm_State;

#endif