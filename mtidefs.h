#ifndef _MT_IDEFS_H_
#define _MT_IDEFS_H_

#include "libs/bcm2836/bcm2836.h"

typedef enum
{
	eMT_State_INIT = 0,
	eMT_State_IDLE,
	eMT_State_STEP,
	eMT_State_GOTO,

	eMT_NUM_STATES
} MotorTask_Sm_State;


typedef enum
{
	eMT_MotorID_MotorA = 0,
	eMT_MotorID_MotorB,

	eMT_NUM_MOTORS
} MotorTask_Motor_Id;


typedef struct 
{
	e_bcm2836_GPIO_Pin     mSigDIR;
	e_bcm2836_GPIO_Pin     mSigSTEP;

} Motor_Struct;

#endif