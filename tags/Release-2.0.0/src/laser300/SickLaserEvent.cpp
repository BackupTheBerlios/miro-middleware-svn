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
// $Id: SickLaserEvent.cpp,v 1.4 2007-02-25 07:09:42 hutz Exp $
//
#include "SickLaserEvent.h"

#include "miro/Exception.h"
#include "miro/Log.h"

#include <algorithm>
#include <iostream>

using std::cout;
using std::endl;

#define MAX_WAIT 500       /* 0.5 sec */
#define MIN_WAIT 80		   /* 0.08 sec */

namespace Miro {
using std::string;

//
// LaserEvent is an object wrapper for the event handler 
// registered with the reactor
//
LaserEvent::LaserEvent(LaserTask* laserTask_, LaserStatistic *laserStatistic_) :
	syncMutex(), ackNackMutex(), ackNackCond(ackNackMutex), stateMutex(),
			laserTask(laserTask_), laserStatistic(laserStatistic_),
			bufstream_(0), bufdata_(S300_STREAMBUF_SIZE) {
	MIRO_LOG_CTOR("Sick::LaserEvent");
	lastfd = -1;
}

//
// Destructor
//
LaserEvent::~LaserEvent() {
	MIRO_LOG_DTOR("Sick::LaserEvent");

	ackNackCond.broadcast();
	syncMutex.release();
	ackNackMutex.release();
	stateMutex.release();

	MIRO_LOG_DTOR_END("Sick::LaserEvent");
}

//
// file descriptor callback
//
int LaserEvent::handle_input(ACE_HANDLE fd) {
	tcgetattr(fd, &newtio);

	// the state machine can be reset from outside, so we have to 
	// acquire acces to it.
	stateMutex.acquire();
	lastfd = fd;

	const int size = 1024;
	unsigned char buf[size];

	int n = read_tty(fd, buf, size, MIN_WAIT);
	if (n < 0) {
		return 0;
	} else if (n == 0) {
		return 0;
	} else if (n <= size) {
		for (int i = 0; i < n; i++)
			bufstream_.push_back(buf[i]);

		int tSize, startIndex;
		if (getS300MsgStart(tSize, startIndex)) {
			putMsgInDataBuf(tSize, startIndex);
			//send message to task
			laserTask->doPkt(tSize, bufdata_);
		}
	}

	stateMutex.release();

	return 0;
}

/* read something from the serial line with timeout
 * and return size of buffer filled (can be 0 on timeout) 
 * If timeout is zero do a blocked read.
 */
int LaserEvent::read_tty(int fd, void *buf, int size, int wait) {
	struct timeval tv;
	fd_set fdset;

	if (wait > 0) { /* to a timed read when wait > 0 */
		tv.tv_sec = wait/1000;
		tv.tv_usec = (wait % 1000) * 1000;
		FD_ZERO(&fdset);
		FD_SET(fd, &fdset);
		if (select(fd + 1, &fdset, NULL, NULL, &tv) <= 0)
			return (0);
	}
	return (read(fd, buf, size));
}

/// returns true, if telegram start found, and bufstream_ includes a complete telegram
bool LaserEvent::getS300MsgStart(int & telegrammsize, int & startIndex) {
	int size = bufstream_.size();
	if (size < 14)
		return false;

	for (int i = 0; i < size; i++) {
		if (size - i < 14) {
			return false;
		}

		//XXX FILTER PROTOCOL VERSION NUMBER
		if (! ((bufstream_.at(i) == 0x0) && (bufstream_.at(i + 1) == 0x0)
				&& (bufstream_.at(i + 2) == 0x0) && (bufstream_.at(i + 3)
				== 0x0) && (bufstream_.at(i + 4) == 0x0) && (bufstream_.at(i
				+ 5) == 0x0))) {
			continue;
		}

		if (! ((bufstream_.at(i + 8) == 0xFF) && (bufstream_.at(i + 9) == 0x07)
				&& (bufstream_.at(i + 10) == 0x02) && (bufstream_.at(i + 11)
				== 0x01))) {
			continue;
		}
		if (! (bufstream_.at(i + 12) == 0x00)) {
			continue;
		}
		if (! (bufstream_.at(i + 13) == 0x00)) {
			continue;
		}

		telegrammsize = bufstream_.at(i + 6) * 256 + bufstream_.at(i + 7) + 2; // +1 fuer CRC TODO: there is 1 more measurement in data, than the protocol says
		telegrammsize *= 2; // size from words to bytes
		if (i + telegrammsize > size) {
			return false;
		}
		startIndex = i; // + 14;

		return true;
	}
	return false;
}

void LaserEvent::putMsgInDataBuf(int tSize, int startIndex) {
	bufdata_.clear();
	for (int i = startIndex; i < startIndex + tSize; i++) {
		bufdata_.push_back(bufstream_.at(i));
	}
	bufstream_.erase(bufstream_.begin(), bufstream_.begin() + (startIndex
			+ tSize));
}

int LaserEvent::handle_close(ACE_HANDLE, ACE_Reactor_Mask) {

	MIRO_LOG(LL_NOTICE, "LaserEvent handle_close called.");
	return 0;
}

void LaserEvent::timeout() {
	MIRO_LOG(LL_ERROR, "timeout called, flushing buffers.");
	// acquire mutex for state machine, avoid rare synch problems
	stateMutex.acquire();

	// flush input buffer, we will have an easier resynch
	// now and the actual message is already gone
	if (lastfd != -1) {
		if (tcflush(lastfd, TCIFLUSH) < 0)
			MIRO_LOG(LL_ERROR, "could not flush buffer.");
	} else {
		MIRO_LOG(LL_ERROR, "could not flush buffer, because no lastfd set.");
	}
	stateMutex.release();
	syncMutex.release();
}
}
;
