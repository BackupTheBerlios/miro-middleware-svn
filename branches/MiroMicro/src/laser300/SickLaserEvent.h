// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id: SickLaserEvent.h,v 1.4 2007-02-25 07:09:42 hutz Exp $
//
#ifndef laserEvent_hh
#define laserEvent_hh

#include <ace/Event_Handler.h>
#include <ace/Synch.h>
#include <ace/Message_Block.h>
#include <cmath>
#include <ace/OS.h>

#include "SickLaserTask.h"

#include <deque>
#include <vector>
using std::deque;
using std::vector;

namespace Miro {

// forward declaration
class LaserTask;

/**
 * LaserEvent is an object wrapper for the event handler 
 * registered with the reactor.
 *
 * it is provided with a log facility and inherits from 
 * E_Event_Handler
 */
class LaserEvent : public ACE_Event_Handler {
public:
	/**
	 * constructor does initialization
	 */
	LaserEvent(LaserTask * _laserTask, LaserStatistic * _laserStatistic);

	/**
	 * destructor, only needed because virtual functions are used
	 */
	virtual ~LaserEvent();

public:
	/**
	 * handles file descriptor events
	 */
	virtual int handle_input(ACE_HANDLE fd); // fd select
	int read_tty(int fd, void *buf, int size, int wait);
	bool getS300MsgStart(int & telegrammsize, int & startIndex);
	void putMsgInDataBuf(int tSize, int startIndex);

	/**
	 * handles close events
	 * Called when object is removed from the <ACE_Reactor>.
	 */
	virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask);

	/**
	 * timeout is called by laserPollTask when the packet 
	 * could not be completed on time, as sending is blocked 
	 * until a complete packet is arrived
	 */
	void timeout();

	/**
	 * sync Mutex max be used to avoid sending to the laser while
	 * we receive from it, but this proved to be not pratical for
	 * deadlock reasons, and problems while resynchronizing to the
	 * laser packet stream.
	 */
	ACE_Thread_Mutex syncMutex;

	/**
	 * for handling the acknowledges of the laser (and making them known
	 * in upper layers) we use a mutex/conditionvariable pair and bool
	 * when an ackownledge is received ackNAck is set and ackNackCond 
	 * is broadcasted.
	 */
	ACE_Thread_Mutex ackNackMutex;
	ACE_Thread_Condition<ACE_Thread_Mutex> ackNackCond;
	bool ackNack;

	/**
	 * state mutex is needed for exclusive access to the state machine
	 * we use for decoding packets
	 */
	ACE_Thread_Mutex stateMutex;

private:

	/**
	 * we need a reference to the Task that handles our packets
	 * we use it to put the assembled packets in its message queue.
	 * Everything that may take some time should be done there.
	 */
	LaserTask* laserTask;

	LaserStatistic *laserStatistic;

	int lastfd;

	static const int S300_STREAMBUF_SIZE = 1024 * 1024;
	deque<unsigned char> bufstream_;
	vector<unsigned char> bufdata_;

private:
	/**
	 * termio settings of the laser interface, 
	 */
	struct termios newtio;

	/**
	 * for statistics we measure the time we need per received packet.
	 */
	ACE_Time_Value timeOfStx;
};

}
;
#endif

