// Copyright(c) 2017 Pacific Robotics
// Author: Milos Lazic

#ifndef _MT_IDEFS_H_
#define _MT_IDEFS_H_

#define _CONFIG_WIRINGPI_

#ifdef _CONFIG_WIRINGPI_
#include <wiringPi.h>
#else
#include "libs/bcm2836/bcm2836.h"
#endif

#define STEP_ANGLE    212   /* milli-degrees per step */

/*
 *  ======== MotorTask_Sm_State ========
 *  Descr: Enumeration of motor task state machine states.
 *
 *  Members:
 *
 *  Notes:
 */
typedef enum
{
	eMT_State_INIT = 0,
	eMT_State_IDLE,
	eMT_State_STEP,
	eMT_State_GOTO,

	eMT_NUM_STATES
} MotorTask_Sm_State;


/*
 *  ======== MotorTask_Motor_Id ========
 *  Descr: Enumeration of motor motor IDs.
 *
 *  Members:
 *
 *  Notes: 1) must match order of Motor_Struct instances
 *            in global Motor array (motortas.c)
 */
typedef enum
{
	eMT_MotorID_MotorA = 0,
	eMT_MotorID_MotorB,

	eMT_NUM_MOTORS
} MotorTask_Motor_Id;


/*
 *  ======== Motor_Struct ========
 *  Descr: Module-wide motor structure. Each physical stepper motor
 *         in the system is represented by its own instance of Motor_Struct
 *
 *  Members:     mSigDIR - bcm2836 (RPi) GPIO pin to control BigEasyDriver DIR input
 *
 *               mSigSTEP - bcm2836 (RPi) GPIO pin to control BigEasyDriver STEP input
 *
 *               angle - current motor angle (0 on startup)
 *
 *  Notes:       1) The member values for each instance are known at compile time
 *                  as they are defined in global Motor array (motortask.c).
 */
typedef struct 
{
#ifdef _CONFIG_WIRINGPI_
	int                    mSigDIR;    // DIR signal GPIO pin index (wiringPI)
	int                    mSigSTEP;   // STEP signal GPIO pin index (wiringPI)
#else
	e_bcm2836_GPIO_Pin     mSigDIR;    // DIR signal GPIO pin index
	e_bcm2836_GPIO_Pin     mSigSTEP;   // STEP signal GPIO pin index
#endif
	int                    angle;      // current motor angle in milli-degrees

} Motor_Struct;


/*
 *  ======== MotorTask_GoTo_Thread_Arg ========
 *  Descr: Data type used to encapsulate command parameters when sub-threads are
 *         created with MotorTask_GoTo_StartRoutine as the start routine
 *
 *  Members:     mID - motor ID
 *
 *               delta - number of steps to drive motor (signed value)
 *
 *  Notes:       1) delta denotes the number of steps needed to reach a specified
 *                  angle as required by GoTo state.
 *
 *                  ex: If the current angle of the motor is +10000 millidegrees
 *                      and the required position is -5000 millidegrees
 *
 *                      delta = (-5000 - (+10000))/STEP_ANGLE
 */
typedef struct
{
	MotorTask_Motor_Id     mID;
	int                    delta;
} MotorTask_GoTo_Thread_Arg;

#endif