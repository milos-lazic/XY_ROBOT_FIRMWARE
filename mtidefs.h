#ifndef _MT_IDEFS_H_
#define _MT_IDEFS_H_

#include "libs/bcm2836/bcm2836.h"

#define STEP_ANGLE    212   /* milli-degrees per step */

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
	e_bcm2836_GPIO_Pin     mSigDIR;    // DIR signal GPIO pin index
	e_bcm2836_GPIO_Pin     mSigSTEP;   // STEP signal GPIO pin index
	int                    angle;      // current motor angle in milli-degrees

} Motor_Struct;


typedef struct
{
	MotorTask_Motor_Id     mID;
	int                    delta;
} MotorTask_GoTo_Thread_Arg;

#endif