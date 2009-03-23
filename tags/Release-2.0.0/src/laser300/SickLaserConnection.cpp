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
// $Id: SickLaserConnection.cpp,v 1.9 2007-02-25 07:09:42 hutz Exp $
//
#include <ace/Reactor.h>

#include "SickLaserConnection.h"
#include "Parameters.h"

#include <termios.h>
#include <linux/serial.h>

#include <iostream>
#include <cmath>

#include "miro/Exception.h"
#include "miro/Log.h"

namespace Miro {
using std::string;
using std::cout;
using std::cerr;
using std::endl;

/***************************************************************************
 *
 *  Laser Implementation 
 *
 ***************************************************************************/

LaserConnection::LaserConnection(ACE_Reactor * _reactor,
		RangeSensorImpl& _rangeSenorI, LaserStatistic * _laserStatistic) :
	reactor_(_reactor), parameters_(*::Laser::Parameters::instance()),
			laserStatistic_(_laserStatistic), laserTask_(new LaserTask(*this, _rangeSenorI, _laserStatistic)),
			laserEvent_(new LaserEvent(laserTask_, _laserStatistic)), fd(0), syncLaserScan(),
			syncLaserScanCond(syncLaserScan), syncModeChange(),
			syncModeChangeCond(syncModeChange), syncVariantChange(),
			syncVariantChangeCond(syncVariantChange), syncStatus(),
			syncStatusCond(syncStatus) {
	// open the device
	std::string device = parameters_.device;//new std::string("/dev/ttyUSB0");
	fd = open(device.c_str(), O_RDWR);

	if (fd < 0)
		cout << "Failed to open device." << endl;

#ifdef THREADEDWORKING
	// start the thread handling (complete) messages
	laserTask->open(NULL);
#endif

	// register handlers at the reactor
	// we need a handler to listen to the asynchronous file handler
	// of the laser device, this handles single characters and composes
	// messages which it passed on to laserTask
	//
	laserEvent_->set_handle(fd);
	int rc = reactor_->register_handler(fd, laserEvent_,
			ACE_Event_Handler::READ_MASK);
	if (rc == -1)
		throw Miro::Exception("LaserConnection::LaserConnection: Failed to register handler for laser file descriptor.");

	// set true after initHardware()
	hardwareInitialized = false;
}

LaserConnection::~LaserConnection() {
	MIRO_LOG_DTOR("LaserConnection");

	// before this destructor is called
	// do not delete this thread manager of ACE needs it
	//delete laserTask;
	//delete laserPollTask;

	MIRO_LOG_DTOR_END("LaserConnection");
}

void LaserConnection::setBaudrate() {
	struct termios t;
	if (tcgetattr(fd, &t) < 0) {
		cout <<"ERROR"<<endl;
	}

	t.c_iflag = IGNBRK | IGNPAR | INPCK;
	t.c_oflag = 0;
	//t.c_cflag = B38400 | CS8 | CREAD | CLOCAL; 
	t.c_cflag = B460800 | CS8 | CREAD | CLOCAL;

	t.c_lflag = 0;
	memset(&t.c_cc, 0, sizeof(cc_t) * NCCS); //clear control characters 
	t.c_cc[VMIN] = 1;
	t.c_cc[VTIME] = 0;

	if (tcsetattr(fd, TCSAFLUSH, &t) < 0) {
		cout <<"ERROR"<<endl;
	}

	if (!do_change_baudrate()) {
		cout<<"ERROR"<<endl;
	}
}

bool LaserConnection::do_change_baudrate() {
	struct termios oldtio;

	if (fd < 0) {
		cout<<"ERROR"<<endl;
		return false;
	}

	if (tcgetattr(fd, &oldtio)<0) {
		cout <<"sick.c: error while getting serial attributes"<<endl;
		exit(-1);
	}

	cfsetispeed(&oldtio, B38400);
	cfsetospeed(&oldtio, B38400);
	//cfsetispeed(&oldtio,B460800);
	//cfsetospeed(&oldtio,B460800);

	oldtio.c_cflag |= (CLOCAL | CREAD);

	/*
	 * Set the new options for the port...
	 */
	tcsetattr(fd, TCSANOW, &oldtio);

	//No parity (8N1) 
	oldtio.c_cflag &= ~PARENB;
	oldtio.c_cflag &= ~CSTOPB;
	oldtio.c_cflag &= ~CSIZE;
	oldtio.c_cflag |= CS8;

	//No Flow Control
	oldtio.c_cflag &= ~CRTSCTS;

	if (tcsetattr(fd, TCSAFLUSH, &oldtio) <0) {
		cout<<"sick.c: error while setting serial attributes"<<endl;
		exit(-1);
	}

	return true;
}

void LaserConnection::initHardware() {
	int baudcode= B38400;
	setBaudrate();
	hardwareInitialized = true;
}

void LaserConnection::stopTasks() {
	serial_struct ss;

	MIRO_DBG(SICK, LL_DEBUG, "entered stoptasks.");

	// Stop hardware triggered communication
	// no more calls to event, and therefore task
	reactor_->remove_handler(laserEvent_, ACE_Event_Handler::READ_MASK);
	MIRO_DBG(SICK, LL_DEBUG, "removed fd handler.");

#ifdef THREADEDWORKING
	// Insert a HANGUP message block
	// to tell the thread to exit svc
	//message = new ACE_Message_Block(0,0);
	//message->msg_type (ACE_Message_Block::MB_HANGUP);
	//laserTask->putq (message);
	//MIRO_DBG(SICK, LL_DEBUG, "sent hangup to laserTask.");
#endif

	syncModeChangeCond.broadcast();
	syncStatusCond.broadcast();

#ifdef THREADEDWORKING
	// Wait for the threads of the Task to exit.
	// It is rather rude to let the Task go out of scope without doing this first.  
	//laserTask->wait ();
	MIRO_DBG(SICK, LL_DEBUG,"laserTask exited.");
#endif

	// restore settings and 
	// close file descriptor
	// get serial info
	if (ioctl(fd, TIOCGSERIAL, &ss) < 0) {
		throw Miro::CException(errno, "stopTasks TIOCGSERIAL");
	}
	ss.custom_divisor = 0;
	ss.baud_base = 115200;
	ss.flags |= ASYNC_LOW_LATENCY;
	// set serial info
	if (ioctl(fd, TIOCSSERIAL, &ss) < 0) {
		throw Miro::CException(errno, "stopTasks TIOCSSERIAL");
	}

	if (tcsetattr(fd, TCSANOW, &oldtio) < 0) {
		throw Miro::CException(errno, "LaserConnection::stopTask TCSANOW");
	}
	if (fd >= 0)
::		close(fd);

		// signal everything we have, to make hanging threads run free
		syncModeChange.release();
		syncVariantChange.release();
		syncStatus.release();
		laserEvent_->syncMutex.release();
		laserEvent_->stateMutex.release();
		laserEvent_->ackNackCond.broadcast();
		laserEvent_->ackNackMutex.release();

	}
};
