#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <pthread.h>

#include "cpidefs.h"
#include "cpudefs.h"
#include "cpproto.h"

/* Global (program-wide) variables */
CmdProc_Motor_Cmd_Queue      cmdQueue;

/* Local Variables */
static CmdProc_Sm_State      state = eCmd_State_INIT;
static int                   serv_fd, new_fd;
static fd_set                master_fds, read_fds;
static int                   fdmax;

/* Local functions */
static int CmdProc_Cmd_Queue_isEmpty( CmdProc_Motor_Cmd_Queue *handle);
static int CmdProc_Cmd_Queue_isFull( CmdProc_Motor_Cmd_Queue *handle);




static int CmProc_getCmdStr( int handle, char *buffer, size_t size)
{
	return read( handle, (void *)buffer, size);
}


/* return 0 IF successful, ELSE -1 */
static int CmdProc_parseMtrCmd( const char *cmdStr)
{
	/* IMPORTANT:
	 * Motor commands MUST adhere to the following format:
	 *
	 * AT!MTR_CMD=<COMMAND_ID>,<PARAM0>,<PARAM1>,<PARAM2>,<PARAM3>
	 *
	 * Valid commands:
	 *
	 *      AT!MTR_CMD=STEP,<MOTORID>,<STEP_COUNT>,<NULL>
	 *
	 *      AT!MTR_CMD=GOTO,<MOTORID>,<ANGLE>,<NULL>
	 *
	 */

	char *cmdID;
	char *param0, *param1;
	CmdProc_Motor_Cmd_Struct cmd;

	cmdID = strtok( (char *)cmdStr, ",");

	param0 = strtok( NULL, ",");

	param1 = strtok( NULL, ",");

	/* IF STEP command */
	if ( strcmp( cmdID, "STEP") == 0)
	{
		cmd.cmd = eCmd_Motor_Cmd_STEP;

		if ( strcmp( param0, "MOTORA") == 0)
		{
			cmd.cmdParams.stepCmdParams.mtrID = eCmd_Motor_Id_MOTORA;
		}
		else if ( strcmp( param0, "MOTORB") == 0)
		{
			cmd.cmdParams.stepCmdParams.mtrID = eCmd_Motor_Id_MOTORB;
		}
		else
		{
			// Invalid motor ID
			return -1;
		}

		cmd.cmdParams.stepCmdParams.steps = atoi(param1);
	}
	/* ELIF GO TO (angle) command */
	else if ( strcmp( cmdID, "GOTO") == 0)
	{
		cmd.cmd = eCmd_Motor_Cmd_GOTO;

		cmd.cmdParams.goToCmdParams.posX = atoi(param0);

		cmd.cmdParams.goToCmdParams.posY = atoi(param1);
	}
	else
	{
		// Invalid command
		return -1;
	}


	/* place command on Motor command queue */
	CmdProc_Cmd_Queue_Put( &cmdQueue, &cmd);

	/* return success */
	return 0;
}


/* return 0 IF valid command, ELSE -1 */
static int CmdProc_processCmd( const char *cmd)
{
	char *scanptr = (char *)cmd;
	int s;

	/* Check for header (AT!) token */
	if ( strstr( scanptr, AT_TOK) == NULL)
	{
		return -1;
	}

	scanptr += strlen(AT_TOK);

	/* IF command is a motor command - MTR_CMD */
	if ( strstr( scanptr, MOTOR_CMD_TOK) != NULL)
	{
		scanptr += strlen(MOTOR_CMD_TOK);

		/* Parse motor command */
		s = CmdProc_parseMtrCmd( scanptr);
		if ( s == -1)
		{
			return -1;
		}

	}
	else if ( *scanptr == '\0' || *scanptr == '\n')
	{
		return 0;
	}
	else
	{
		// invalid command
		return -1;
	}
	return 0;
}



static void CmdProc_SmState_InitFxn( void)
{
	// state machine Init state function

	struct addrinfo       hints, *servinfo, *p;
	socklen_t             sin_size;
	int                   rv;

	int                   yes = 1;

	/* clear 'hints' strcture */
	memset( &hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (( rv = getaddrinfo( NULL, PORT, &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
	}

	for ( p = servinfo; p != NULL; p = p->ai_next)
	{
		if (( serv_fd = socket( p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			perror("server: socket");
			continue;
		}

		if ( setsockopt( serv_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		{
			perror("setsockopt");
		}

		if ( bind(serv_fd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(serv_fd);
			perror("server: bind");
			continue;
		}

		break;
	}

	freeaddrinfo( servinfo);

	if ( p == NULL)
	{
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	/* clear master and temp fd sets */
	FD_ZERO( &read_fds);
	FD_ZERO( &master_fds);

	FD_SET( serv_fd, &master_fds);
	fdmax = serv_fd;


	/* Initialize command queue */
	rv = CmdProc_Cmd_Queue_Init( &cmdQueue);
	if ( rv == -1)
	{
		close( serv_fd);
		perror("CmdProc_Cmd_Queue_Init");
		return; // return; try again..
	}


	// mark server socket for listening 
	if ( listen( serv_fd, MAX_SERVER_BACKLOG) == -1)
	{
		perror("listen");
		exit(1);
	}

	state = eCmd_State_LISTEN;
}


static void CmdProc_SmState_ListenFxn( void)
{
	// state machine Listen state function

	read_fds = master_fds; // copy master file descriptor set

	/* wait for event on monitored file descriptors */
	if ( select( fdmax+1, &read_fds, NULL, NULL, NULL) == -1)
	{
		perror("select");
		exit(4);
	}

	// run through existing connections looking for data to read
	for ( int i = 0; i <= fdmax; i++)
	{
		if ( FD_ISSET( i, &read_fds))
		{
			if ( i == serv_fd)
			{
				// event on listener socket (new connection)
				new_fd = accept( serv_fd, NULL, NULL);
				if ( new_fd == -1)
				{
					perror("accept");
				}
				else
				{
					FD_SET( new_fd, &master_fds);
					if ( new_fd > fdmax);
						fdmax = new_fd;

					state = eCmd_State_CONNECTED;
					return;
				}
			}
			else
			{
				// data to read on client socket
				// NOTE: should never be reached in this state
				//       as no connections have be established
				//       at this point
			}
		}
	}

	// loop back to same state
	state = eCmd_State_LISTEN;
}


static void CmdProc_SmState_ConnectedFxn( void)
{
	// state machine Connected state function
	char                  buf[MAX_CMD_LEN];
	int                   nbytes;
	int                   s;

	read_fds = master_fds; // copy master file descriptor set

	/* wait for event on monitored file descriptors */
	if ( select( fdmax+1, &read_fds, NULL, NULL, NULL) == -1)
	{
		perror("select");
		exit(4);
	}

		// run through existing connections looking for data to read
		for ( int i = 0; i <= fdmax; i++)
		{
			if ( FD_ISSET( i, &read_fds))
			{

				if ( i == serv_fd)
				{
					// event on listener socket (new connection)
					// NOTE: ignore new connection requests in this state
					new_fd = accept( serv_fd, NULL, NULL);
					(void) close(new_fd); // immediately close new connection
				}
				else
				{
					// data to read on client socket
					if ( (nbytes = CmProc_getCmdStr( i, buf, sizeof(buf))) <= 0)
					{
						if ( nbytes == 0)
						{
							// connection closed
							printf("connection closed\n");
						}
						else
						{
							// read() error
							perror("read");
						}
						close(i);
						FD_CLR( i, &master_fds);

						// return to listen state
						state = eCmd_State_LISTEN;
						return;						
					}
					else
					{
						buf[nbytes] = '\0';
						// handle data from client

						s = CmdProc_processCmd( buf);
						/* IF command is valid */
						if ( s == 0)
						{
							write( i, "OK\r\n", 5);
						}
						/* ELSE invalid */
						else
						{
							write( i, "ERR\r\n", 6);
						}
					}
				}
			}
		}

		// loop back to same state
		state = eCmd_State_CONNECTED;
}


static void CmdProc_Sm_Run( void)
{
	switch( state)
	{

	case eCmd_State_INIT:
		CmdProc_SmState_InitFxn();
		break;

	case eCmd_State_LISTEN:
		CmdProc_SmState_ListenFxn();
		break;

	case eCmd_State_CONNECTED:
		CmdProc_SmState_ConnectedFxn();
		break;

	default:
		// invalid state
		break;
	}
}


int CmdProc_Cmd_Queue_Init( CmdProc_Motor_Cmd_Queue *handle)
{
	int s;

	if ( handle == NULL)
		return -1;

	handle->in = &(handle->queue[0]);
	handle->out = &(handle->queue[0]);

	s = pthread_mutex_init( &(handle->qmx), NULL);
	if ( s != 0)
	{
		perror("pthread_mutex_init");
		return -1;
	}

	return 0;
}


static int CmdProc_Cmd_Queue_isEmpty( CmdProc_Motor_Cmd_Queue *handle)
{
	// return 1 if empty; 0 if contains elements
	return ((handle->in == handle->out) ? 1 : 0);
}

static int CmdProc_Cmd_Queue_isFull( CmdProc_Motor_Cmd_Queue *handle)
{
	// return 1 IF full; ELSE 0
	return ( ( (handle->in == handle->out -1) || 
	           (handle->in == &(handle->queue[MAX_CMD_QUEUE_BACKLOG-1]) && handle->out == &handle->queue[0])) ? 1 : 0);
	           
}

int CmdProc_Cmd_Queue_Put( CmdProc_Motor_Cmd_Queue         *handle,
	                       CmdProc_Motor_Cmd_Struct        *cmd)
{
	int s;

	if ( handle == NULL || cmd == NULL)
		return -1;

	/* Enter critical section */
	s = pthread_mutex_lock( &(handle->qmx));
	if ( s != 0)
		return -1; // failed to acquire mutex


	/* check if queue is full */
	if ( CmdProc_Cmd_Queue_isFull( handle))
	{
		/* Leave critical section */
		(void) pthread_mutex_unlock( &(handle->qmx));
		return -1;
	}

	/* Copy data to command queue */
	memcpy( handle->in, cmd, sizeof(CmdProc_Motor_Cmd_Struct));

	/* IF insertion pointer is at the end of the buffer, wrap around */
	if ( handle->in == &(handle->queue[MAX_CMD_QUEUE_BACKLOG-1]))
	{
		handle->in = &(handle->queue[0]);
	}
	else
	{
		handle->in += 1;
	}

	/* Leave critical section */
	(void) pthread_mutex_unlock( &(handle->qmx));

	/* Return success */
	return 0;
}


// Notes: returns immediately if buffer is empty
int CmdProc_Cmd_Queue_Get( CmdProc_Motor_Cmd_Queue         *handle,
	                       CmdProc_Motor_Cmd_Struct        *cmdOutBuf)
{
	int s;

	if ( handle == NULL)
		return -1;

	/* Enter critical section */
	s = pthread_mutex_lock( &(handle->qmx));
	if ( s != 0)
		return -1; // failed to acquire mutex

	/* IF buffer is empty, return error */
	if ( CmdProc_Cmd_Queue_isEmpty( handle))
	{
		/* Leave critical section */
		(void) pthread_mutex_unlock( &(handle->qmx));
		return -1;
	}


	/* IF buffer is supplied (cmdOutBuf != NULL), copy contents into it */
	if ( cmdOutBuf != NULL)
	{
		/* Copy data from queue to storage buffer */
		memcpy( cmdOutBuf, handle->out, sizeof(CmdProc_Motor_Cmd_Struct));
	}

	/* IF retrieval pointer is at the end of the buffer, wrap around */
	if ( handle->out == &(handle->queue[MAX_CMD_QUEUE_BACKLOG-1]))
	{
		handle->out = &(handle->queue[0]);
	}
	else
	{
		handle->out += 1;
	}

	/* Leave critical section */
	(void) pthread_mutex_unlock( &(handle->qmx));

	/* Return success */
	return 0;
}



void* CmdProc_heartbeatFxn( void *arg)
{

	pthread_detach( pthread_self());

	printf("%s (%d) - cmd proc task initialized...\n", __FILE__, __LINE__);

	while(1)
	{
		CmdProc_Sm_Run();
	}
}
