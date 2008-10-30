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
// $Id: SickLaserTask.h,v 1.4 2007-02-25 07:09:42 hutz Exp $
//
#ifndef laserTask_hh
#define laserTask_hh

#include "miro/Task.h"

#include <vector>
using std::vector;

namespace Laser {
class Parameters;
}

namespace Miro {
// forward declaration
class LaserConnection;
class LaserMessage;
class RangeSensorImpl;
class LaserStatistic;

/**
 * handles complete packages as assembled by laserEvent
 * gets the packages from its message queue, where 
 * laserEvent puts them.
 * keeps statistics about corrupted packages
 * uses the reference to LaserImpl to signal the data
 * received via condition variables
 */
class LaserTask : public Task {
	/**
	 * main super class
	 */
	typedef Task Super;

public:
	/**
	 * Constructor
	 */
	LaserTask(LaserConnection& _laser, RangeSensorImpl& _laserI,
			LaserStatistic * _laserStatistic = NULL);

	/**
	 * Destructor
	 */
	virtual ~LaserTask();

	/**
	 * this handles one packet that is already decoded from a message
	 * block, this used to call this from outside, if we do not run in
	 * thread, but for convenience we use it in the threaded approach
	 * also
	 */
	void doPkt(int tSize, vector<unsigned char>& bufdata);

	/**
	 * this is the task main loop, which defines the work to be done
	 * this is worked upon on the number of threads that are created
	 * inside this task
	 */
	virtual int svc();

protected:
	/**
	 * Laser message interface: the asynchronously called handler
	 * does the (hardware specific decoding of the message
	 */
	virtual void handleMessage(int tSize, vector<unsigned char>& bufdata);

protected:
	/**
	 * reference to the LaserImpl class, for signaling data
	 * via condition variables
	 */
	LaserConnection& laser_;
	RangeSensorImpl& laserI_;

	/**
	 * Reference to the parameter set. Must not be constant because
	 * the SickLaserTask might need to update some fields in the
	 * LaserDescription
	 */
	::Laser::Parameters& parameters_;

	/**
	 * ptr to the central device error statistic
	 */
	LaserStatistic * laserStatistic_;

	/**
	 * Conversion factor for raw laser scan values (in mm)
	 */
	double rangeFactor; //XXX
};
}
;
#endif

