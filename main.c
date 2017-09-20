#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "cpproto.h"
#include "mtproto.h"


int main ( int argc, char *argv[])
{
	pthread_t CmdProc_threadHandle;
	(void) pthread_create( &CmdProc_threadHandle, NULL, CmdProc_heartbeatFxn, NULL);

	pthread_t Motor_threadHandle;
	(void) pthread_create( &Motor_threadHandle, NULL, MotorTask_heartbeatFxn, NULL);

	while(1)
	{
		// loop forever
	}

	exit(0);
}
