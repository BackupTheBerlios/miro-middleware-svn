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
// $Id: SickLaserConnection.h,v 1.7 2007-02-25 07:09:42 hutz Exp $
//
#ifndef laserConnection_hh
#define laserConnection_hh

// #include "miro/DevConnection.h"
#include "miro/RangeSensorImpl.h"
#include "idl/LaserC.h"

#include "SickLaserTask.h"
#include "SickLaserEvent.h"

#include <ace/Event_Handler.h>
#include <ace/Synch.h>

#include <exception>
#include <string>

#include <termios.h>

namespace Laser {
class Parameters;
}
;

namespace Miro {
// forward declarations
class RangeSensorImpl;

class LaserTask;
class LaserPollTask;
class LaserMessage;
class LaserEvent;

/**
 * LaserConnection
 */
class LaserConnection {
public:
	//! Constructor
	LaserConnection(ACE_Reactor * _reactor_, RangeSensorImpl& _rangeSenorI,
			LaserStatistic * _laserStatistic);

	//! Destructor
	virtual ~LaserConnection();

	/**
	 * initialize and program baudrate of laser
	 * has to be calles when the reactor is fully set up,
	 * (from LaserServer::LaserServer, at the moment)
	 */
	void initHardware();

	//! Stop all action of tasks and deregister event handlers.
	void stopTasks();

protected:
	void setBaudrate();
	bool do_change_baudrate();

	/**
	 * for event handling (file descriptors)
	 * the reactor listens for data sent by the laser
	 */
	ACE_Reactor* reactor_;

	const ::Laser::Parameters& parameters_;
	//! prt to the central error statistic object
	LaserStatistic * laserStatistic_;

	/**
	 * The laserTask is a group of threads (or only one) that handles 
	 * complete pakets received by the laser. It does not have to bother 
	 * for protocol issues. It is seperate thread, which discouples the
	 * hardware and protocol handling from the actual work to do.
	 */
	LaserTask * laserTask_;

	/**
	 * The laserEvent decodes the communication protocol and reassembles the
	 * pakets which maybe fragmented into several portions depending on
	 * timing and buffers. This is the object which is called by the
	 * reactor when there is activity on the filE descriptor
	 */
	LaserEvent * laserEvent_;

	/**
	 * file descriptor of the laser interface
	 */
	int fd;

	/**
	 * termio settings of the laser interface, the old settings are 
	 * restored before
	 * we exit
	 */
	struct termios oldtio, newtio;

public:
	/**
	 * mutex for synchronization on access to mode with LaserTask
	 */
	Mutex syncLaserScan;

	/**
	 * condition variable for synchronization on update of mode by LaserTask
	 */
	Condition syncLaserScanCond;

	/**
	 * mutex for synchronization on access to mode with LaserTask
	 */
	Mutex syncModeChange;

	/**
	 * condition variable for synchronization on update of mode by LaserTask
	 */
	Condition syncModeChangeCond;

	/**
	 * mutex for synchronization on access to variant with LaserTask
	 */
	Mutex syncVariantChange;

	/**
	 * condition variable for synchronization on update of variant by LaserTask
	 */
	Condition syncVariantChangeCond;

	/**
	 * set if the laser acknowledged the mode change
	 */
	bool modeChanged;

	/**
	 * set if the laser acknowledged the variant change
	 */
	bool variantChanged;

	/**
	 * set as soon as the hardware initialization has been completed
	 */
	bool hardwareInitialized;

	/**
	 * mutex for synchronization on status requests
	 */
	Mutex syncStatus;

	/**
	 * condition variable for synchronization on status requests
	 */
	Condition syncStatusCond;
};
}
;
#endif

