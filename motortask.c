// Copyright(c) 2017 Pacific Robotics
// Author: Milos Lazic

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "mtidefs.h"
#include "mtudefs.h"
#include "mtproto.h"
#include "mtservo.h"
#include "cpudefs.h"

#define _CONFIG_WIRINGPI_

#ifdef _CONFIG_WIRINGPI_
#include <wiringPi.h>
#else
#include "libs/bcm2836/bcm2836.h"
#endif


//#define DELAY_1_MS  1000000
#define DELAY_1_MS  1000
#define DELAY_10_MS 10000000
/* Global (application-wide) variables */
extern CmdProc_Motor_Cmd_Queue cmdQueue;
extern Motor_Angles table;

/* Local variables */
static MotorTask_Sm_State           state = eMT_State_INIT;
static CmdProc_Motor_Cmd_Struct     cmd; // most recent command
unsigned int count = 0;


#ifndef _CONFIG_WIRINGPI_
static bcm2836_Peripheral           gpio;
#endif

/* contains instances of Motor_Struct objects; must be in same order as MotorTask_Motor_Id enumeration. */
static volatile Motor_Struct        Motor[eMT_NUM_MOTORS] =
{
	                           /* Connected to DIR on BED */   /* Connected to STEP on BED */   /* intial angle */
#ifndef _CONFIG_WIRINGPI_
	/* eMT_MotorID_MotorA */ { .mSigDIR = BCM2836_GPIO_PIN_19,  .mSigSTEP = BCM2836_GPIO_PIN_26,  .angle = 0 },
	/* eMT_MotorID_MotorB */ { .mSigDIR = BCM2836_GPIO_PIN_10,  .mSigSTEP = BCM2836_GPIO_PIN_9,   .angle = 0 },
#else
	/* NOTE: wiringPI and BCM28136 pin numbering is different; used shell command 'gpio readll' to determine
	         appropriate pin numbers when using wiringPi library. Ex: BCM2836 GPIO_PIN_19 = WIRINGPI_PIN_24 */

	/* eMT_MotorID_MotorA */ { .mSigDIR = 24,                   .mSigSTEP = 25,                   .angle = 124647,   .mSigEN = 3 },
	/* eMT_MotorID_MotorB */ { .mSigDIR = 27,                   .mSigSTEP = 28,                   .angle = 92646,   .mSigEN = 2 },
#endif
};

/* Local functions */

/*
 *  ======== MotorTask_stepCCW ========
 *  Rotate N steps in counter-clock-wise sense.
 *
 *  Args:     index - reference to motor (corresponding to apporpriate
 *                    entry in Motor array)
 *
 *            steps - number of steps to rotate
 *  
 *  Return:   none
 *
 *  Notes:    1) MotorTask_step makes sub-calls to direction-specific
 *               MotorTask_stepCW and MotorTask_stepCCW.
 */
static void MotorTask_stepCCW( MotorTask_Motor_Id index, unsigned int steps)
{
	int i;
	struct timespec ts_1ms = { .tv_sec = 0, .tv_nsec = DELAY_1_MS };

	/* NOTE: When BigEasyDrive v1.2 DIR input is HIGH, driver
	         will step in CCW direction on rising edge of 
	         STEP input
	 */

	write( STDOUT_FILENO, "     MotorTask_stepCCW\n", 24);

	/* set direction to counter-clock-wise */
#ifndef _CONFIG_WIRINGPI_
	bcm2836_GPIOSetPinLevel( &gpio, Motor[index].mSigDIR, BCM2836_GPIO_PIN_LEVEL_LOW);
#else
	digitalWrite( Motor[index].mSigEN, LOW);
	digitalWrite( Motor[index].mSigDIR, LOW);
#endif

	for ( i = 0; i < steps; i++)
	{
		/* steps occur on RISING edge - set STEP signal
		   LOW, then immediately to HIGH
		 */
#ifndef _CONFIG_WIRINGPI_
		bcm2836_GPIOSetPinLevel( &gpio, Motor[index].mSigSTEP, BCM2836_GPIO_PIN_LEVEL_LOW);
		bcm2836_GPIOSetPinLevel( &gpio, Motor[index].mSigSTEP, BCM2836_GPIO_PIN_LEVEL_HIGH);
#else
		digitalWrite( Motor[index].mSigSTEP, LOW);
		nanosleep( &ts_1ms, NULL);
		digitalWrite( Motor[index].mSigSTEP, HIGH);
#endif

		nanosleep( &ts_1ms, NULL);
	}

#ifdef _CONFIG_WIRINGPI_
	digitalWrite( Motor[index].mSigEN, HIGH);
#endif

	/* update motor angle */
	Motor[index].angle += STEP_ANGLE*steps;
}


/*
 *  ======== MotorTask_stepCW ========
 *  Rotate N steps in clock-wise sense.
 *
 *  Args:     index - reference to motor (corresponding to apporpriate
 *                    entry in Motor array)
 *
 *            steps - number of steps to rotate
 *  
 *  Return:   none
 *
 *  Notes:    1) MotorTask_step makes sub-calls to direction-specific
 *               MotorTask_stepCW and MotorTask_stepCCW.
 */
static void MotorTask_stepCW( MotorTask_Motor_Id index, unsigned int steps)
{
	int i;
	struct timespec ts_1ms = { .tv_sec = 0, .tv_nsec = DELAY_1_MS };

	/* NOTE: When BigEasyDrive v1.2 DIR input is LOW, driver
	         will step in CW direction on rising edge of 
	         STEP input
	 */

	write( STDOUT_FILENO, "     MotorTask_stepCW\n", 23);

	/* set direction to counter-clock-wise */
#ifndef _CONFIG_WIRINGPI_
	bcm2836_GPIOSetPinLevel( &gpio, Motor[index].mSigDIR, BCM2836_GPIO_PIN_LEVEL_HIGH);
#else
	digitalWrite( Motor[index].mSigEN, LOW);
	digitalWrite( Motor[index].mSigDIR, HIGH);
#endif

	for ( i = 0; i < steps; i++)
	{
		/* steps occur on RISING edge - set STEP signal
		   LOW, then immediately to HIGH
		 */

#ifndef _CONFIG_WIRINGPI_
		bcm2836_GPIOSetPinLevel( &gpio, Motor[index].mSigSTEP, BCM2836_GPIO_PIN_LEVEL_LOW);
		bcm2836_GPIOSetPinLevel( &gpio, Motor[index].mSigSTEP, BCM2836_GPIO_PIN_LEVEL_HIGH);
#else

		digitalWrite( Motor[index].mSigSTEP, LOW);
		nanosleep( &ts_1ms, NULL);
		digitalWrite( Motor[index].mSigSTEP, HIGH);
#endif

		nanosleep( &ts_1ms, NULL);
	}

#ifdef _CONFIG_WIRINGPI_
        digitalWrite( Motor[index].mSigEN, HIGH);
#endif

	/* update motor angle */
	Motor[index].angle -= STEP_ANGLE*steps;
}


/*
 *  ======== MotorTask_step ========
 *  Rotate N steps.
 *
 *  Args:     index - reference to motor (corresponding to apporpriate
 *                    entry in Motor array)
 *
 *            steps - number of steps to rotate (sign determines direction)
 *
 *  Return:   none
 *
 *  Notes:    1) For positive value of 'steps' argument, driver steps CCW;
 *               For negative value of 'steps' argument, driver steps CW.
 *
 *            2) MotorTask_step makes sub-calls to direction-specific
 *               MotorTask_stepCW and MotorTask_stepCCW.
 */
static void MotorTask_step( MotorTask_Motor_Id index, int steps)
{
	/* NOTE: for positive values of 'steps' argument, driver steps CCW;
	         for negative values of 'steps' argument, driver steps CW
	 */

	if ( steps >= 0)
	{
		MotorTask_stepCCW( index, steps);
	}
	else
	{
		MotorTask_stepCW( index, -1*steps);
	}
}


/*
 *  ======== MotorTask_GoTo_StartRoutine ========
 *  Routine to invoke when sub threads are created by MotorTask_SmState_GoToFxn.
 *
 *  Args:     arg - pointer to object containing motor ID and
 *                  step number
 *  
 *  Return:   none
 *
 *  Notes:    1) arg must be casted to MotorTask_GoTo_Thread_Arg type
 *               so that it may be dereferenced and the motor ID, step
 *               number values can be extracted.
 *
 *            2) Upon completion thread joins with the master thread.
 */
static void* MotorTask_GoTo_StartRoutine( void *arg)
{
	MotorTask_GoTo_Thread_Arg *p = (MotorTask_GoTo_Thread_Arg *) arg;

	MotorTask_step( p->mID, p->delta);

	return NULL;
}



/*
 *  ======== MotorTask_SmState_InitFxn ========
 *  Routine to execute while state machine is in 'Init' state.
 *
 *  Args:     none
 * 
 *  Return:   none
 *
 *  Notes:    1) Initial state entered by the motor task FSM; used
 *               to perform peripheral (GPIO) initialisation. Loops
 *               through members of Motor array and intializes the
 *               corresponding GPIO pins as digital outputs to drive
 *               DIR and STEP pins on BigEasyDriver.
 */
static void MotorTask_SmState_InitFxn( void)
{
	// state machine Init state function

#ifndef _CONFIG_WIRINGPI_	
	int rv;

	/* initialize GPIO peripheral */
	rv = bcm2836_initPeripheral( &gpio, BCM2836_GPIO_PERIPH_BYTE_LEN, BCM2836_GPIO_PERIPH_BASE_ADR);
	if ( rv == - 1)
	{
		perror("bcm2836_initPeripheral");	
	}
#else
	wiringPiSetup();

	/* MLAZIC_TBD: WiringPi set up the I2C driver, wiringPiI2CSetup() */
#endif

	/* Configure GPIOs controlling motors */
	for ( int i = 0; i < eMT_NUM_MOTORS; i++)
	{
#ifndef _CONFIG_WIRINGPI_
		bcm2836_GPIOPinTypeOutput( &gpio, Motor[i].mSigDIR);
		bcm2836_GPIOPinTypeOutput( &gpio, Motor[i].mSigSTEP);
#else
		pinMode( Motor[i].mSigDIR, OUTPUT);
		pinMode( Motor[i].mSigSTEP, OUTPUT);
		pinMode( Motor[i].mSigEN, OUTPUT);

		digitalWrite( Motor[i].mSigEN, HIGH);
#endif
	}

#ifndef _CONFIG_WIRINGPI_

#else
	/* MLAZIC_TBD: call mtservo_init() and mtservo routines needed to set up servo driver board */
#endif

	state = eMT_State_IDLE;
}


/*
 *  ======== MotorTask_SmState_IdleFxn ========
 *  Routine to execute while state machine is in 'Idle' state.
 *
 *  Args:     none
 *  
 *  Return:   none
 *
 *  Notes:    1) In this state the command queue is polled for
 *               new commands. If a command is received a switch
 *               case determines which state the FSM will transition
 *               to.
 */
static void MotorTask_SmState_IdleFxn( void)
{
	// state machine Idle state function

	int s;

	s = CmdProc_Cmd_Queue_Get( &cmdQueue, &cmd);
	if ( s == -1)
	{
		/* Failed to retrieve command */

		/* Loop back to same state */
		state = eMT_State_IDLE;
	}
	else
	{
		count++;
		printf("count = %d\n", count);
		switch( cmd.cmd)
		{

		case eCmd_Motor_Cmd_STEP:
			state = eMT_State_STEP;
			break;

		case eCmd_Motor_Cmd_GOTO:
			state = eMT_State_GOTO;
			break;

		default:
			/* invalid command */
			break;

		}
	}
}


/*
 *  ======== MotorTask_SmState_StepFxn ========
 *  Routine to execute while state machine is in 'Step' state.
 *
 *  Args:     none
 *  
 *  Return:   none
 *
 *  Notes:    1) This state is entered when the command to step
 *               is received. Depending on the motor ID embedded
 *               in the received command, the corresponding motor
 *               will be driven a specified number of steps. The
 *               sign of the steps member of the command packet
 *               determines the direction of stepping and the 
 *               appropriate subroutine to invoke.
 *
 *            2) This state is entered when MTR_CMD=STEP is issued;
 *               used for testing.
 */
static void MotorTask_SmState_StepFxn( void)
{

	// do work

	write( STDOUT_FILENO, "MotorTask_SmState_StepFxn\r\n", 28);

	switch( cmd.cmdParams.stepCmdParams.mtrID)
	{

	case eCmd_Motor_Id_MOTORA:
		MotorTask_step( eMT_MotorID_MotorA, cmd.cmdParams.stepCmdParams.steps);
		break;

	case eCmd_Motor_Id_MOTORB:
		MotorTask_step( eMT_MotorID_MotorB, cmd.cmdParams.stepCmdParams.steps);
		break;

	default:
		break;
	}

	state = eMT_State_IDLE;
}


/*
 *  ======== MotorTask_SmState_GoToFxn ========
 *  Routine to execute while state machine is in 'GoTo' state.
 *
 *  Args:     none
 *  
 *  Return:   none
 *
 *  Notes:    1) This state is entered when the command to go to
 *               a specified cartesian coordinate is received. The 
 *               corresponding motor shaft angles are determined 
 *               by look-up from a table. Two sub-threads are created
 *               and will independently drive each stepper motor to
 *               their required positions. Each thread is joined with
 *               the master thread upon completion and the state machine
 *               reverts to 'Idle' state.
 */
static void MotorTask_SmState_GoToFxn( void)
{
	// do work

	MotorTask_GoTo_Thread_Arg        argA, argB;
	Motor_Angles                    *targetAngles;
	pthread_t                        motorA_threadHandle, motorB_threadHandle;

	write( STDOUT_FILENO, "MotorTask_SmState_GoToFxn\r\n", 28);

	/* lookuptable() - for given posX and posY determine
	                   the required motor angles
	 */
	targetAngles = MotorTask_LookupFxn( cmd.cmdParams.goToCmdParams.posX,
	                                    cmd.cmdParams.goToCmdParams.posY);
	/* IF inverse kinematics unsuccessful */
	if ( targetAngles == NULL)
	{
		state = eMT_State_IDLE;
		return;
	}

	// argA.delta = (req_angle_A - current_angle_A) / DEGREES_PER_STEP
	argA.delta = (targetAngles->theta1 - Motor[eMT_MotorID_MotorA].angle) / STEP_ANGLE;
	argA.mID   = eMT_MotorID_MotorA;

	// argB.delta = (req_angle_B - current_andle_B) / DEGREES_PER_STEP
	argB.delta = (targetAngles->theta3 - Motor[eMT_MotorID_MotorB].angle) / STEP_ANGLE;
	argB.mID   = eMT_MotorID_MotorB;

	/* create sub-thread A */
	pthread_create( &motorA_threadHandle, NULL, MotorTask_GoTo_StartRoutine, &argA);

	/* create sub-thread B */
	pthread_create( &motorB_threadHandle, NULL, MotorTask_GoTo_StartRoutine, &argB);


	pthread_join( motorA_threadHandle, NULL);
	pthread_join( motorB_threadHandle, NULL);


	state = eMT_State_IDLE;
}


/*
 *  ======== MotorTask_Sm_Run ========
 *  Handles transition between motor task state machine states.
 *
 *  Args:     none
 *  
 *  Return:   none
 *
 *  Notes:    To determine which state to transition the machine
 *            to, MotorTask_Sm_Run checks the global variable 'state'.
 */
static void MotorTask_Sm_Run( void)
{
	switch( state)
	{

	case eMT_State_INIT:
		MotorTask_SmState_InitFxn();
		break;

	case eMT_State_IDLE:
		MotorTask_SmState_IdleFxn();
		break;

	case eMT_State_STEP:
		MotorTask_SmState_StepFxn();
		break;	

	case eMT_State_GOTO:
		MotorTask_SmState_GoToFxn();
		break;	

	default:
		// invalid state
		break;
	}
}


/*
 *  ======== MotorTask_heartbeatFxn ========
 *  Motor driver task function. Starts state machine which waits (polls)
 *  until command is available via global command queue.
 *
 *  Args:     arg - optional argument; passed in when task is created
 *                  in call to pthread_create()
 *
 *  Return:   Thread immediately marks itself as detached; return value is
 *            ignored and resources are released automatically.
 *
 *  Notes:
 */
void* MotorTask_heartbeatFxn( void *arg)
{
	struct timespec ts = { .tv_sec = 0, .tv_nsec = DELAY_10_MS };

	pthread_detach( pthread_self());

	printf("%s (%d) - motor driver task initialized...\n", __FILE__, __LINE__);


	while(1)
	{
		MotorTask_Sm_Run();

		/* sleep several milliseconds */
		nanosleep( &ts, NULL);
		
	}
}
