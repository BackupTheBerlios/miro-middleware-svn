#ifndef CANIOCTL_H
#define CANIOCTL_H
// -*- c++ -*-
//
// canioctl.h 
//
// hardware dependent part of a kernel module for a can bus driver for the 
// MSMCAN card (by delta components)
//
// most code contributed by Michael Wallner and Andre Mueller, ported for
// this hardware by
// Stefan Sablatnoeg
//
// (C) 1999 Stefan Sablatnoeg
// 

#include <linux/ioctl.h>
#include "canmsg.h"

typedef struct {
	int reg,val;
} RegStruct;

#define CAN_ID			0xA1

#define CAN_RESET 		_IO(CAN_ID,0)

/* controller configuration */
#define CAN_SETCONFIG	_IOW(CAN_ID,1,sizeof(CanConfig))
#define CAN_GETCONFIG	_IOR(CAN_ID,2,sizeof(CanConfig))

/* controller status */
#define CAN_GETSTAT		_IOR(CAN_ID,5,sizeof(CanStatus))
#define CAN_CLEARSTAT	_IO(CAN_ID,6)

#define CAN_READREG		_IOWR(CAN_ID,8,sizeof(RegStruct))
#define CAN_WRITEREG	_IOW(CAN_ID,9,sizeof(RegStruct))
#define CAN_GETREGISTERS _IOR(CAN_ID,10,256)
#endif
