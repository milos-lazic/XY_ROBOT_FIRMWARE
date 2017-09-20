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


#define DELAY_10_MS 10000000

/* Global (application-wide) variables */
extern CmdProc_Motor_Cmd_Queue cmdQueue;

/* Local variables */
static MotorTask_Sm_State           state = eMT_State_INIT;
static CmdProc_Motor_Cmd_Struct     cmd; // most recent command
static bcm2836_Peripheral           gpio;

/* Local functions */

static void MotorTask_SmState_InitFxn( void)
{
	// state machine Init state function
	
	int s;

	/* initialize GPIO peripheral */
	s = bcm2836_initPeripheral( &gpio, BCM2836_GPIO_PERIPH_BYTE_LEN, BCM2836_GPIO_PERIPH_BASE_ADR);
	if ( s == - 1)
	{
		perror("bcm2836_initPeripheral");	
	}

	bcm2836_GPIOPinTypeOutput( &gpio, BCM2836_GPIO_PIN_4);
	bcm2836_GPIOSetPinLevel( &gpio, BCM2836_GPIO_PIN_4, BCM2836_GPIO_PIN_LEVEL_LOW);



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

	state = eMT_State_IDLE;
}


static void MotorTask_SmState_GoToFxn( void)
{
	// do work

	write( STDOUT_FILENO, "MotorTask_SmState_GoToFxn\r\n", 28);

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