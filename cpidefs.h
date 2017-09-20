#ifndef _CP_IDEFS_H_
#define _CP_IDEFS_H_

/* Defintions used only internally by the Command Proc module */


#define PORT "1234"
#define MAX_CMD_LEN  256
#define MAX_CMD_QUEUE_BACKLOG 250
#define MAX_SERVER_BACKLOG 5



#define AT_TOK               "AT!"

#define MOTOR_CMD_TOK        "MTR_CMD="


typedef enum 
{
	eCmd_State_INIT = 0,
	eCmd_State_LISTEN,
	eCmd_State_CONNECTED,

	eCmd_NUM_STATES

} CmdProc_Sm_State;


#endif