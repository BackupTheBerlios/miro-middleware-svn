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
// $Id$
//
#include "SickLaserTask.h"
#include "SickLaserConnection.h"
#include "SickLaserMessage.h"
#include "SickLaserStatistic.h"
#include "Parameters.h"

#include "miro/TimeHelper.h"
#include "miro/Log.h"

#include <iostream>
#include <sstream>

#include <netinet/in.h>

namespace Miro
{
  using std::cout;
  using std::cerr;
  using std::endl;

  //////////////////////////////////////////////////////////////////////
  // LaserTask
  //
  LaserTask::LaserTask(LaserConnection& _laser, 
		       RangeSensorImpl& _laserI, 
		       LaserStatistic * _laserStatistic) :
    Super(),
    laser_(_laser),
    laserI_(_laserI),
    parameters_(*::Laser::Parameters::instance()),
    laserStatistic_(_laserStatistic)
  {
    MIRO_LOG_CTOR("Miro::LaserTask");
    rangeFactor = 1;
  }

  //////////////////////////////////////////////////////////////////////
  // ~LaserTask
  LaserTask::~LaserTask() 
  {
    MIRO_LOG_DTOR("Miro::LaserTask");
  }

  void 
  LaserTask::doPkt(LaserMessage *data) 
  {
    // do decoding
    handleMessage(*data);

    // keep statistics
    if (laserStatistic_) {
      ++(laserStatistic_->packetsProcessed);
    
      ACE_Time_Value elapsed = ACE_OS::gettimeofday() - laserStatistic_->startTime ;

      // show statistic rarely
      if (!((laserStatistic_->packetsProcessed) % 1000)) {
	MIRO_LOG(LL_NOTICE,"Statistics:");
	double mPerPkt = (double)laserStatistic_->recvTime.msec() / 
	  ( laserStatistic_->packetsProcessed + 
	    laserStatistic_->packetsWrongAddress + 
	    laserStatistic_->packetsCRCError +
	    laserStatistic_->packetsToLong );

	std::cerr << "running since " 
		  << elapsed.sec() << " s, "
		  << mPerPkt << " ms/recvd.pkt, "
		  << laserStatistic_->packetsProcessed / elapsed.sec() 
		  << " pkts/s, "
		  << laserStatistic_->packetsProcessed 
		  << " pkts, " 
		  << laserStatistic_->packetsWrongAddress
		  << " wrong address, " 
		  << laserStatistic_->packetsToLong  
		  << " too long, " 
		  << laserStatistic_->packetsCRCError 
		  << " CRC errors, "
		  << laserStatistic_->timeouts
		  << " timeouts, " 
		  << laserStatistic_->events
		  << " events."  << std::endl;
      }
    }
  }
  
  ////////////////////////////////////////////////////////////////////////
  // Now the svc() method where everything interesting happens.
  //
  int 
  LaserTask::svc()
  {
    MIRO_DBG_OSTR(SICK, LL_DEBUG, "("<<(void *) this <<"|"<<ACE_Thread::self ()<<") Task 0x%x starts in thread %u\n");

    // Where we getq() the message
    ACE_Message_Block *message;
    LaserMessage * data;

    while (true) {
      // Get the message...
      if (getq (message) == -1) {
	throw Miro::Exception("LaserTask::svc: could not getq from message queue");
      }

      // Is it a shutdown request?
      if (message->msg_type () == ACE_Message_Block::MB_HANGUP) {
	break;
      }

      // Get the LaserMessage pointer out of the ACE message block.
      data = (LaserMessage*)message->rd_ptr ();
      if (!data) 
	throw Miro::Exception("LaserTask::svc: got empty message block");
      
      doPkt( data );

      // throw away message block
      message->release ();
    }

    MIRO_LOG(LL_NOTICE, "left service.");

    return (0);
  }

  ////////////////////////////////////////////////////////////////////////
  // handleMessage
  //
  void 
  LaserTask::handleMessage(const LaserMessage& message) 
  {
    // we do not need to chack for crc here, we are only passed crc
    // correct packets
    switch (message.cmd()) {
      
    case LR_LMS_TYPE:
      MIRO_LOG(LL_NOTICE, "received LMS_TYPE MESSAGE");
      message.PrintDataOn(std::cerr);
      break;

    case LR_POWER_ON: 
      MIRO_LOG(LL_NOTICE, "received POWER_ON MESSAGE");
      message.PrintDataOn(std::cerr);
      break;

    case LR_ACK_RESET: 
      MIRO_LOG(LL_NOTICE, "received SW-RESET ACKNOWLEDGE MESSAGE");
      break;
      
    case LR_NOT_ACK: 
      MIRO_LOG(LL_WARNING, "received NOT ACKNOWLEDGE MESSAGE");
      break;

    case LR_MODE_CHANGE: 
      MIRO_LOG(LL_NOTICE, "received MODE CHANGE ACK");
      laser_.syncModeChange.acquire();
      switch (message.getUnsignedByte(0)) {
      case 0: 
	MIRO_LOG(LL_NOTICE, "Mode changed successfully");
	laser_.modeChanged = true;
	break;
      case 1:
	MIRO_LOG(LL_NOTICE, "Mode not changed, wrong password");
	laser_.modeChanged = false;
	break;
      case 2:
	MIRO_LOG(LL_NOTICE, "Mode not changed, error in LMI/LMS");
	laser_.modeChanged = false;
	break;
      default:
	MIRO_LOG(LL_NOTICE, "Mode not changed, unknown error");
	laser_.modeChanged = false;
	break;
      }
      laser_.syncModeChange.release();
      laser_.syncModeChangeCond.broadcast();
      break;

    case LR_REQ_DATA: 
      if (laser_.hardwareInitialized) {
	int vals = (message.getUnsignedShort(0) & 0x3fff);
	int expected = (int)(parameters_.fov / parameters_.scanResolution + 1);
      
	// throw exception if not the expected number of data arrived
	if (vals != expected) {
	  std::ostringstream outs;
	  outs << "LaserTask::handleMessage: expected " << expected
	       << " values, got " << vals;
	  throw Miro::Exception(outs.str());
	}

	Guard guard(laser_.syncLaserScan);
	RangeGroupEventIDL * data = new RangeGroupEventIDL();

	ACE_Time_Value now = ACE_OS::gettimeofday();
	timeA2C(now, data->time);
	data->group = 0;
	data->range.length(vals);

	for (long i = vals - 1; i >= 0; --i) {
	  data->range[i] = (int)(rangeFactor *
			   (message.getUnsignedShort((i+1)*2) & 0x1fff));
	}

	laserI_.integrateData(data);

	laser_.syncLaserScanCond.broadcast();
      }
      break;

      case LR_LMS_STATUS:
	MIRO_LOG(LL_NOTICE, "received sensor/LMI status");
	laser_.syncStatusCond.broadcast();

	if (message.datalength() >= 122) {
	  switch (message.getUnsignedByte(121)) {
	    case 0: // Range factor 1cm, maximum scan range 80m
	      rangeFactor = 10;
	      parameters_.laserDescription.group[0].description.maxRange
			  = 81910;
	      break;
	    case 1: // Range factor 1mm, maximum scan range 8m
	      rangeFactor = 1;
	      parameters_.laserDescription.group[0].description.maxRange
			  = 8191;
	      break;
	  }
	}
	// TODO further decode
	break;

      case LR_ERROR_TEST: 
	MIRO_LOG(LL_NOTICE, "received error/test");
	switch (message.getUnsignedByte(0)) {
	case 0: 
	  MIRO_LOG(LL_NOTICE, "No Error, Test OK");
	  break;
	case 1:
	  MIRO_LOG(LL_NOTICE, "Info");
	  break;
	case 2:
	  MIRO_LOG(LL_WARNING, "Warning");
	  break;
	case 3:
	  MIRO_LOG(LL_ERROR, "Error");
	  break;
	case 4:
	  MIRO_LOG(LL_ERROR, "Fatal Error");
	  break;
	}
	// TODO further decode

	break;

      case LR_MEMORY_DUMP: 
	MIRO_LOG(LL_NOTICE, "received memory dump");
	// TODO further decode
	break;

      case LR_AVERAGE_DATA: 
	MIRO_LOG(LL_NOTICE, "received average data");
	// TODO further decode
	break;

      case LR_DATA_RANGE: 
	MIRO_LOG(LL_NOTICE, "received data sub range");
	// TODO further decode
	break;

      case LR_LMI_CONF_DATA: 
	MIRO_LOG(LL_NOTICE, "received lmi configuration");
	// TODO further decode
	break;

      case LR_LMI_HANDLE_DEFS: 
	MIRO_LOG(LL_NOTICE, "received data definition");
	// TODO further decode
	break;

      case LR_LMS_VARIANT_CHANGE:
	MIRO_LOG(LL_NOTICE, "received variant change");
	laser_.syncVariantChange.acquire();
	switch (message.getUnsignedByte(0)) {
	case 0:
	  MIRO_LOG(LL_NOTICE, "variant change aborted");
	  laser_.variantChanged = false;
	  break;
	case 1:
	  MIRO_LOG(LL_NOTICE, "variant change done");
	  laser_.variantChanged = true;
	  break;
	default:
	  MIRO_LOG(LL_WARNING, "variant change: unknown status");
	  laser_.variantChanged = false;
	}
	laser_.syncVariantChange.release();
	laser_.syncVariantChangeCond.broadcast();

	MIRO_LOG_OSTR(LL_NOTICE, "angular scan range    :"<< message.getUnsignedShort(1) << endl);
	MIRO_LOG_OSTR(LL_NOTICE, "single shot resolution:"<< message.getUnsignedShort(3) << endl);
	break;

      case LR_FIELD_CONF: 
	MIRO_LOG(LL_NOTICE, "received field configuration :");
	switch (message.getUnsignedByte(0)) {
	case 0: 
	  MIRO_LOG(LL_NOTICE, "field conf aborted");
	  break;
	case 1:
	  MIRO_LOG(LL_NOTICE, "field conf done");
	  break;
	}
	//  TODO further decode
	break;

      case LR_PASSWORD_CHANGE: 
	MIRO_LOG(LL_NOTICE, "received password change ack:");
	switch (message.getUnsignedByte(0)) {
	case 0: 
	  MIRO_LOG(LL_NOTICE, "password change aborted");
	  break;
	case 1:
	  MIRO_LOG(LL_NOTICE, "password change done");
	  break;
	case 2:
	  MIRO_LOG(LL_NOTICE, "acknowledge of user expected");
	  break;
	}
	switch (message.getUnsignedByte(1)) {
	case 0: 
	  MIRO_LOG(LL_NOTICE, "password for SICK-SERVICE and authorized customers");
	  break;
	case 1:
	  MIRO_LOG(LL_NOTICE, "password for maintenance");
	  break;
	  break;
	}
	break;
      case LR_FIELD_CONF_DATA: 
	MIRO_LOG(LL_NOTICE, "received field configuration data:");
	//  TODO further decode
	break;

      case LR_LEARN_FIELD_CONF: 
	MIRO_LOG(LL_NOTICE, "received learn field configuration :");
	switch (message.getUnsignedByte(0)) {
	case 0: 
	  MIRO_LOG(LL_NOTICE, "learning aborted");
	  break;
	case 1:
	  MIRO_LOG(LL_NOTICE, "done, verification can start");
	  break;
	case 3:
	  MIRO_LOG(LL_NOTICE, "learning active");
	  break;
	}
	MIRO_LOG_OSTR(LL_NOTICE,"learning from LMS #:"<< message.getUnsignedByte(1) << endl);
	break;

      case LR_CONF_DYN_RECT: 
	MIRO_LOG(LL_NOTICE, "received configuration for dynamic rectangle:");
	//  TODO further decode
	break;

      case LR_CONF_DYN_SEGMENT: 
	MIRO_LOG(LL_NOTICE, "received dynamic segment configuration:");
	//  TODO further decode
	break;

      case LR_DYN_FIELD_INDEX_OR_SPEED: 
	MIRO_LOG(LL_NOTICE, "received field index or speed configuration :");
	switch (message.getUnsignedByte(0)) {
	case 0: 
	  MIRO_LOG(LL_NOTICE, "switch aborted");
	  break;
	case 1:
	  MIRO_LOG(LL_NOTICE, "switch done.");
	  break;
	}
	MIRO_LOG_OSTR(LL_NOTICE,"Index of activated field #:" << message.getUnsignedShort(1) << endl);
	break;

      case LR_SET_OUTPUT: 
	MIRO_LOG(LL_NOTICE, "received set output active/inactive :");
	switch (message.getUnsignedByte(0)) {
	case 0: 
	  MIRO_LOG(LL_NOTICE, "output switch aborted");
	  break;
	case 1:
	  MIRO_LOG(LL_NOTICE, "output switch done.");
	  break;
	}
	MIRO_LOG_OSTR(LL_NOTICE,"output #:" << message.getUnsignedShort(1) << endl);
	MIRO_LOG_OSTR(LL_NOTICE,"active (1) or inactive (0) :" << message.getUnsignedShort(2) << endl);
      
	break;

      case LR_LMI_INPUT: 
	MIRO_LOG_OSTR(LL_NOTICE, "received input data (bit 0..3:A..D, 4:restart):" << message.getUnsignedShort(0) << endl);
	break;

      case LR_SIMULATE_INPUT: 
	MIRO_LOG(LL_NOTICE, "received simulate input:");
	//  TODO further decode
	break;

      case LR_CALIBRATE: 
	MIRO_LOG(LL_NOTICE, "received calibrate:");
	switch (message.getUnsignedByte(0)) {
	case 0: 
	  MIRO_LOG(LL_NOTICE, "calibration not ok");
	  break;
	case 1:
	  MIRO_LOG(LL_NOTICE, "calibration ok");
	  break;
	}
	break;

      case LR_CHANGE_PERMANENT_BAUDRATE: 
	MIRO_LOG(LL_NOTICE, "received acknowledge of permanent baudrate change:");
	switch (message.getUnsignedByte(0)) {
	case 0: 
	  MIRO_LOG(LL_NOTICE, "change of baudrate not accepted");
	  break;
	case 1:
	  MIRO_LOG(LL_NOTICE, "change of baudrate accepted");
	  break;
	}
	switch (message.getUnsignedByte(1)) {
	case 0: 
	  MIRO_LOG(LL_NOTICE, "baudrate will be 9600 on poweron");
	  break;
	case 1:
	  MIRO_LOG(LL_NOTICE, "baudrate will be unchanged on poweron");
	  break;
	}
	break;

      case LR_CHANGE_ADDRESS: 
	MIRO_LOG(LL_NOTICE, "received acknowledge of address definition :");
	switch (message.getUnsignedByte(0)) {
	case 0: 
	  MIRO_LOG(LL_NOTICE, "change of address not accepted");
	  break;
	case 1:
	  MIRO_LOG(LL_NOTICE, "change of address accepted");
	  break;
	}
	MIRO_LOG_OSTR(LL_NOTICE,"new address is :" << message.getUnsignedByte(1) << endl);
	break;

      case LR_ACTIVATE_SENDER: 
	MIRO_LOG(LL_NOTICE, "received activate sender :");
	switch (message.getUnsignedByte(0)) {
	case 0: 
	  MIRO_LOG(LL_NOTICE, "activation of sender not accepted");
	  break;
	case 1:
	  MIRO_LOG(LL_NOTICE, "activation of sender accepted");
	  break;
	}
	MIRO_LOG_OSTR(LL_NOTICE,"activation is :" << message.getUnsignedByte(1) << endl);
	break;

      case LR_LMI_CONF: 
	MIRO_LOG(LL_NOTICE, "received LMI conf :");
	switch (message.getUnsignedByte(0)) {
	case 0: 
	  MIRO_LOG(LL_NOTICE, "LMI conf not accepted");
	  break;
	case 1:
	  MIRO_LOG(LL_NOTICE, "LMI conf accepted");
	  break;
	}
	MIRO_LOG_OSTR(LL_NOTICE,"number of attached sensors :" << message.getUnsignedByte(1) << endl);
	// TODO further decode 
	break;

      case LR_HANDLE_DEFS: 
	MIRO_LOG(LL_NOTICE, "received case definition :");
	// TODO further decode 
	break;
      case LR_PIXEL_ORIENTED: 
	MIRO_LOG(LL_NOTICE, "received pixel oriented mode acknowledge:");
	switch (message.getUnsignedByte(0)) {
	case 0: 
	  MIRO_LOG(LL_NOTICE, "state change not accepted");
	  break;
	case 1:
	  MIRO_LOG(LL_NOTICE, "state change accepted");
	  break;
	}
	switch (message.getUnsignedByte(1)) {
	case 0: 
	  MIRO_LOG(LL_NOTICE, "state : not pixel oriented");
	  break;
	case 1:
	  MIRO_LOG(LL_NOTICE, "state : pixel oriented");
	  break;
	}
	break;
      case LR_CARTESIAN: 
	MIRO_LOG(LL_NOTICE, "received cartesian scan:");
	break;

      case LR_CONF_LMS: 
	MIRO_LOG(LL_NOTICE, "received LMS configuration:");
	switch (message.getUnsignedByte(0)) {
	case 0: 
	  MIRO_LOG(LL_NOTICE, "conf not accepted");
	  break;
	case 1:
	  MIRO_LOG(LL_NOTICE, "conf accepted");
	  break;
	}
	break;


      default:
	cerr << __FILE__  << " : " 
	     << __LINE__ << ":" 
	     << __FUNCTION__ ; 
	MIRO_LOG(LL_WARNING, "() - Unhandled opcode in :");
	message.PrintDumpOn(cerr);
      }
  }
};
