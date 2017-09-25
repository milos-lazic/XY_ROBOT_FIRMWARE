#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "mtidefs.h"
#include "mtudefs.h"
#include "mtproto.h"
#include "cpudefs.h"
#include "libs/bcm2836/bcm2836.h"


#define DELAY_1_MS   1000000
#define DELAY_10_MS 10000000

/* Global (application-wide) variables */
extern CmdProc_Motor_Cmd_Queue cmdQueue;

/* Local variables */
static MotorTask_Sm_State           state = eMT_State_INIT;
static CmdProc_Motor_Cmd_Struct     cmd; // most recent command
static bcm2836_Peripheral           gpio;

static Motor_Struct                 Motor[eMT_NUM_MOTORS] =
{
	                           /* Connected to DIR on BED */   /* Connected to STEP on BED */   /* intial angle */
	/* eMT_MotorID_MotorA */ { .mSigDIR = BCM2836_GPIO_PIN_5,  .mSigSTEP = BCM2836_GPIO_PIN_6,  .angle = 0 },
	/* eMT_MotorID_MotorB */ { .mSigDIR = BCM2836_GPIO_PIN_13, .mSigSTEP = BCM2836_GPIO_PIN_19, .angle = 0 },
};

/* Local functions */

static void MotorTask_stepCCW( MotorTask_Motor_Id index, unsigned int steps)
{
	int i;
	struct timespec ts_1ms = { .tv_sec = 0, .tv_nsec = DELAY_1_MS };

	/* NOTE: When BigEasyDrive v1.2 DIR input is HIGH, driver
	         will step in CCW direction on rising edge of 
	         STEP input
	 */

	/* set direction to counter-clock-wise */
	bcm2836_GPIOSetPinLevel( &gpio, Motor[index].mSigDIR, BCM2836_GPIO_PIN_LEVEL_HIGH);

	for ( i = 0; i < steps; i++)
	{
		/* steps occur on RISING edge - set STEP signal
		   LOW, then immediately to HIGH
		 */

		bcm2836_GPIOSetPinLevel( &gpio, Motor[index].mSigSTEP, BCM2836_GPIO_PIN_LEVEL_LOW);
		bcm2836_GPIOSetPinLevel( &gpio, Motor[index].mSigSTEP, BCM2836_GPIO_PIN_LEVEL_HIGH);

		nanosleep( &ts_1ms, NULL);
	}

	/* update motor angle */
	Motor[index].angle += STEP_ANGLE;
}


static void MotorTask_stepCW( MotorTask_Motor_Id index, unsigned int steps)
{
	int i;
	struct timespec ts_1ms = { .tv_sec = 0, .tv_nsec = DELAY_1_MS };

	/* NOTE: When BigEasyDrive v1.2 DIR input is LOW, driver
	         will step in CW direction on rising edge of 
	         STEP input
	 */

	/* set direction to counter-clock-wise */
	bcm2836_GPIOSetPinLevel( &gpio, Motor[index].mSigDIR, BCM2836_GPIO_PIN_LEVEL_LOW);

	for ( i = 0; i < steps; i++)
	{
		/* steps occur on RISING edge - set STEP signal
		   LOW, then immediately to HIGH
		 */

		bcm2836_GPIOSetPinLevel( &gpio, Motor[index].mSigSTEP, BCM2836_GPIO_PIN_LEVEL_LOW);
		bcm2836_GPIOSetPinLevel( &gpio, Motor[index].mSigSTEP, BCM2836_GPIO_PIN_LEVEL_HIGH);

		nanosleep( &ts_1ms, NULL);
	}

	/* update motor angle */
	Motor[index].angle -= STEP_ANGLE;
}


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


static void* MotorTask_GoTo_StartRoutine( void *arg)
{
	MotorTask_GoTo_Thread_Arg *p = (MotorTask_GoTo_Thread_Arg *) arg;

	MotorTask_step( p->mID, p->delta);
}



static void MotorTask_SmState_InitFxn( void)
{
	// state machine Init state function
	
	int rv;

	/* initialize GPIO peripheral */
	rv = bcm2836_initPeripheral( &gpio, BCM2836_GPIO_PERIPH_BYTE_LEN, BCM2836_GPIO_PERIPH_BASE_ADR);
	if ( rv == - 1)
	{
		perror("bcm2836_initPeripheral");	
	}

	/* Configure GPIOs controlling motors */
	for ( int i = 0; i < eMT_NUM_MOTORS; i++)
	{
		bcm2836_GPIOPinTypeOutput( &gpio, Motor[i].mSigDIR);

		bcm2836_GPIOPinTypeOutput( &gpio, Motor[i].mSigSTEP);
		bcm2836_GPIOSetPinLevel( &gpio, Motor[i].mSigSTEP, BCM2836_GPIO_PIN_LEVEL_HIGH);
	}


	state = eMT_State_IDLE;
}

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


static void MotorTask_SmState_GoToFxn( void)
{
	// do work

	MotorTask_GoTo_Thread_Arg        argA, argB;
	pthread_t                        motorA_threadHandle, motorB_threadHandle;

	write( STDOUT_FILENO, "MotorTask_SmState_GoToFxn\r\n", 28);

	/* lookuptable() - for given posX and posY determine
	                   the required motor angles
	 */

	/* MLAZIC_TBD: fake deltas for testing */
	argA.delta = 45000 / STEP_ANGLE; /* rotate +45 degrees */
	argB.delta = -45000 / STEP_ANGLE; /* rotate -45 degrees */
	/* MLAZIC_END */

	// argA.delta = (req_angle_A - current_angle_A) / DEGREES_PER_STEP
	argA.mID   = eMT_MotorID_MotorA;

	// argB.delta = (req_angle_B - current_andle_B) / DEGREES_PER_STEP
	argB.mID   = eMT_MotorID_MotorB;

	/* create sub-thread A */
	pthread_create( &motorA_threadHandle, NULL, MotorTask_GoTo_StartRoutine, &argA);

	/* create sub-thread B */
	pthread_create( &motorB_threadHandle, NULL, MotorTask_GoTo_StartRoutine, &argB);


	pthread_join( motorA_threadHandle, NULL);
	pthread_join( motorB_threadHandle, NULL);


	state = eMT_State_IDLE;
}


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