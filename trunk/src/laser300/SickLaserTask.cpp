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
// $Id: SickLaserTask.cpp,v 1.5 2007-02-25 07:09:42 hutz Exp $
//
#include "SickLaserTask.h"
#include "SickLaserConnection.h"
#include "SickLaserStatistic.h"
#include "Parameters.h"

#include "miro/TimeHelper.h"
#include "miro/Log.h"

#include <iostream>
#include <sstream>

#include <netinet/in.h>

namespace Miro {
using std::cout;
using std::cerr;
using std::endl;

//////////////////////////////////////////////////////////////////////
// LaserTask
//
LaserTask::LaserTask(LaserConnection& _laser, RangeSensorImpl& _laserI,
		LaserStatistic * _laserStatistic) :
	Super(), laser_(_laser), laserI_(_laserI),
			parameters_(*::Laser::Parameters::instance()),
			laserStatistic_(_laserStatistic) {
	MIRO_LOG_CTOR("Miro::LaserTask");
	rangeFactor = 1;
}

//////////////////////////////////////////////////////////////////////
// ~LaserTask
LaserTask::~LaserTask() {
	MIRO_LOG_DTOR("Miro::LaserTask");
}

void LaserTask::doPkt(int tSize, vector<unsigned char>& bufdata) {
	// do decoding
	handleMessage(tSize, bufdata);

	// keep statistics
	if (laserStatistic_) {
		++(laserStatistic_->packetsProcessed);

		ACE_Time_Value elapsed = ACE_OS::gettimeofday()
				- laserStatistic_->startTime;

		// show statistic rarely
		if (!((laserStatistic_->packetsProcessed) % 1000)) {
			MIRO_LOG(LL_NOTICE, "Statistics:");
			double mPerPkt = (double)laserStatistic_->recvTime.msec()
					/ (laserStatistic_->packetsProcessed
							+ laserStatistic_->packetsWrongAddress
							+ laserStatistic_->packetsCRCError
							+ laserStatistic_->packetsToLong );

			std::cerr << "running since " << elapsed.sec() << " s, " << mPerPkt
					<< " ms/recvd.pkt, " << laserStatistic_->packetsProcessed
					/ elapsed.sec() << " pkts/s, "
					<< laserStatistic_->packetsProcessed << " pkts, "
					<< laserStatistic_->packetsWrongAddress
					<< " wrong address, " << laserStatistic_->packetsToLong
					<< " too long, " << laserStatistic_->packetsCRCError
					<< " CRC errors, " << laserStatistic_->timeouts
					<< " timeouts, " << laserStatistic_->events << " events."
					<< std::endl;
		}
	}
}

////////////////////////////////////////////////////////////////////////
// Now the svc() method where everything interesting happens.
//
int LaserTask::svc() {
	MIRO_DBG_OSTR(SICK, LL_DEBUG, "("<<(void *) this <<"|"<<ACE_Thread::self()
			<<") Task 0x%x starts in thread %u\n");

	//NO THREADED WORKING SUPPORTED
	
	return (0);
}

////////////////////////////////////////////////////////////////////////
// handleMessage
//
void LaserTask::handleMessage(int tSize, vector<unsigned char>& bufdata) {
	if (laser_.hardwareInitialized) {

		Guard guard(laser_.syncLaserScan);
		RangeGroupEventIDL * data = new RangeGroupEventIDL();

		ACE_Time_Value now = ACE_OS::gettimeofday();
		timeA2C(now, data->time);
		int dataStart = 24;
		int numRanges = (tSize - 2 - 24)/2;

		data->group = 0;
		data->range.length(numRanges);

		//cout << "Ranges: ";
		double angle = -270.0/2.0;
		for (int i = 0; i < numRanges; i++) {
			unsigned char first = bufdata.at(i * 2 + dataStart);
			unsigned char second = bufdata.at(i * 2 + dataStart + 1);
			int dist = (first + second *256) & (0xFFF);
			data->range[i] = dist * 10;
			//cout << (dist*10) << ":";
		}

		laserI_.integrateData(data);

		laser_.syncLaserScanCond.broadcast();
	}
}
}
;
