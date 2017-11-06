// Copyright(c) 2017 Pacific Robotics
// Author: Milos Lazic

#ifndef _MT_PROTO_H_
#define _MT_PROTO_H_

#include "mtidefs.h"

/* motortask.c */

extern void * MotorTask_heartbeatFxn( void *arg);

extern Motor_Angles * MotorTask_LookupFxn( int x, int y);


#endif