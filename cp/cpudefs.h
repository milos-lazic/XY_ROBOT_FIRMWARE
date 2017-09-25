#ifndef _CP_UDEFS_H_
#define _CP_UDEFS_H_

#include <pthread.h>

#include "cpidefs.h"
#include "cpproto.h"

/* Command proc definitions used accross the package */

typedef enum
{
	eCmd_Motor_Cmd_STEP = 0,
	eCmd_Motor_Cmd_GOTO,

	eCmd_NUM_CMDS
	
} CmdProc_Motor_Cmd;


typedef enum
{
	eCmd_Motor_Id_MOTORA = 0,
	eCmd_Motor_Id_MOTORB,

	eCmd_NUM_MOTORS
	
} CmdProc_Motor_Id;


typedef struct CmdProc_Motor_Cmd_Struct
{
	CmdProc_Motor_Cmd     cmd;

	union cmdParams 
	{

		struct stepCmdParams
		{
			CmdProc_Motor_Id     mtrID;
			int                  steps;
		} stepCmdParams;

		struct goToCmdParams
		{
			unsigned int         posX;
			unsigned int         posY;
		} goToCmdParams;

	} cmdParams;

	unsigned int          priority;

	struct CmdProc_Motor_Cmd_Struct     *next;

} CmdProc_Motor_Cmd_Struct;



typedef struct
{
	CmdProc_Motor_Cmd_Struct     *in;
	CmdProc_Motor_Cmd_Struct     *out;
	CmdProc_Motor_Cmd_Struct      queue[MAX_CMD_QUEUE_BACKLOG];

	pthread_mutex_t                      qmx;
	
} CmdProc_Motor_Cmd_Queue;



extern int CmdProc_Cmd_Queue_Init( CmdProc_Motor_Cmd_Queue *handle);

extern int CmdProc_Cmd_Queue_Put( CmdProc_Motor_Cmd_Queue         *handle,
	                              struct CmdProc_Motor_Cmd_Struct *cmd);

extern int CmdProc_Cmd_Queue_Get( CmdProc_Motor_Cmd_Queue         *handle,
	                              struct CmdProc_Motor_Cmd_Struct *cmdOutBuf);


#endif