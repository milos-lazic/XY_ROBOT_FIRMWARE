// Copyright(c) 2017 Pacific Robotics
// Author: Milos Lazic

#ifndef _CP_UDEFS_H_
#define _CP_UDEFS_H_

#include <pthread.h>

#include "cpidefs.h"
#include "cpproto.h"

/* Command proc definitions used accross the package */

/*
 *  ======== CmdProc_Motor_Cmd ========
 *  Descr: Enumeration of all possible command types
 *         used by the command process task.
 *
 *  Members:
 *
 *  Notes:
 */
typedef enum
{
	eCmd_Motor_Cmd_STEP = 0,
	eCmd_Motor_Cmd_GOTO,
	eCmd_Motor_Cmd_SERVO,

	eCmd_NUM_CMDS
	
} CmdProc_Motor_Cmd;


/*
 *  ======== CmdProc_Motor_Cmd ========
 *  Descr: Enumeration of all motor IDs used by the
 *         command process task.
 *
 *  Members:
 *
 *  Notes:
 */
typedef enum
{
	eCmd_Motor_Id_MOTORA = 0,
	eCmd_Motor_Id_MOTORB,

	eCmd_NUM_MOTORS
	
} CmdProc_Motor_Id;


/*
 *  ======== CmdProc_Motor_Cmd_Struct ========
 *  Descr: Structure used to format and store commands received
 *         from the client application.
 *
 *  Members:     cmd - (enum) command
 *
 *               cmdParams - command parameters
 *               	.stepCmdParams - STEP command parameters
 *                  	.mtrID - motor ID
 *                  	.steps - number of steps to drive motor
 *               	.goToCmdParams - GOTO command paramters
 *                  	.posX - X coordinate of position to go to
 *                      .posY - Y coordinate of position to go to
 *
 *               priority - command pririoty (usage TBD)
 *
 *  Notes: 1) For STEP command, store data in cmdParams.stepCmdParams;
 *            for GOTO command, store data in cmdParams.goToCmdParams; etc...   
 */
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

		struct servoCmdParams
		{
			unsigned int         pos;
		} servoCmdParams;

	} cmdParams;

	unsigned int          priority;

} CmdProc_Motor_Cmd_Struct;


/*
 *  ======== CmdProc_Motor_Cmd_Queue ========
 *  Descr: Motor command queue structure
 *
 *  Members:     in - pointer to queue location in which to perform
 *                    the next command insertion (CmdProc_Cmd_Queue_Put)
 *
 *               out - pointer to queue location from which to perform
 *                     the next command retrieval (CmdProc_Cmd_Queue_Get)
 *
 *               queue - (statically allocated) array to store queue elements
 *
 *               qmx - reference to mutex to synchronize queue access
 *
 *  Notes: 
 */
typedef struct
{
	CmdProc_Motor_Cmd_Struct     *in;
	CmdProc_Motor_Cmd_Struct     *out;
	CmdProc_Motor_Cmd_Struct      queue[MAX_CMD_QUEUE_BACKLOG];

	pthread_mutex_t               qmx;
	
} CmdProc_Motor_Cmd_Queue;



extern int CmdProc_Cmd_Queue_Init( CmdProc_Motor_Cmd_Queue *handle);

extern int CmdProc_Cmd_Queue_Put( CmdProc_Motor_Cmd_Queue         *handle,
	                              struct CmdProc_Motor_Cmd_Struct *cmd);

extern int CmdProc_Cmd_Queue_Get( CmdProc_Motor_Cmd_Queue         *handle,
	                              struct CmdProc_Motor_Cmd_Struct *cmdOutBuf);


#endif