// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "SickLaserTask.h"
#include "SickLaserConnection.h"
#include "SickLaserMessage.h"
#include "SickLaserStatistic.h"

#include "miro/TimeHelper.h"

#include <iostream>

#include <netinet/in.h>

// #undef DEBUG

namespace Miro
{
  //////////////////////////////////////////////////////////////////////
  // LaserTask
  //
  LaserTask::LaserTask(LaserConnection& _laser, 
		       RangeSensorImpl& _laserI, 
		       LaserStatistic * _laserStatistic) :
    Super(),
    Log(INFO,"LaserTask"),
    laser_(_laser),
    laserI_(_laserI),
    laserStatistic_(_laserStatistic)
  {}

  //////////////////////////////////////////////////////////////////////
  // ~LaserTask
  LaserTask::~LaserTask() 
  {}

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
	log(INFO,"Statistics:");
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
#ifdef DEBUG
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Task 0x%x starts in thread %u\n", (void *) this, ACE_Thread::self ()));
#endif

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

    log(INFO, "left service.");

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
      log(INFO, "received LMS_TYPE MESSAGE");
      message.PrintDataOn(std::cerr);
      break;

    case LR_POWER_ON: 
      log(INFO, "received POWER_ON MESSAGE");
      message.PrintDataOn(std::cerr);
      break;

    case LR_ACK_RESET: 
      log(INFO, "received SW-RESET ACKNOWLEDGE MESSAGE");
      break;
      
    case LR_NOT_ACK: 
      log(WARNING, "received NOT ACKNOWLEDGE MESSAGE");
      break;

    case LR_MODE_CHANGE: 
      log(INFO, "received MODE CHANGE ACK");
      laser_.syncModeChange.acquire();
      switch (message.getUnsignedByte(0)) {
      case 0: 
	log(INFO, "Mode changed successfully");
	laser_.modeChanged = true;
	break;
      case 1:
	log(INFO, "Mode not changed, wrong password");
	laser_.modeChanged = false;
	break;
      case 2:
	log(INFO, "Mode not changed, error in LMI/LMS");
	laser_.modeChanged = false;
	break;
      default:
	log(INFO, "Mode not changed, unknown error");
	laser_.modeChanged = false;
	break;
      }
      laser_.syncModeChange.release();
      laser_.syncModeChangeCond.broadcast();
      break;

    case LR_REQ_DATA: 
      {
	//log(INFO, "received sensor data");
	//message.PrintDumpOn(std::cerr);
	int vals = (message.getUnsignedShort(0) & 0x3fff);
      
	// throw exception if not the expected number of data arrived
	if (vals != 361)
	  throw Miro::Exception("LaserTask::handleMessage: was expecting 361 values");

	Guard guard(laser_.syncLaserScan);
	RangeGroupEventIDL * data = new RangeGroupEventIDL();

	ACE_Time_Value now = ACE_OS::gettimeofday();
	timeA2C(now, data->time);
	data->group = 0;
	data->range.length(vals);

	for (long i = vals - 1; i >= 0; --i) {
	  data->range[i] = message.getUnsignedShort((i+1)*2) & 0x1fff;
	}

	laserI_.integrateData(data);

	laser_.syncLaserScanCond.broadcast();
	break;
      }
      case LR_LMS_STATUS: 
	log(INFO, "received sensor/LMI status");
	laser_.syncStatusCond.broadcast();
	// TODO further decode
	break;

      case LR_ERROR_TEST: 
	log(INFO, "received error/test");
	switch (message.getUnsignedByte(0)) {
	case 0: 
	  log(INFO, "No Error, Test OK");
	  break;
	case 1:
	  log(INFO, "Info");
	  break;
	case 2:
	  log(WARNING, "Warning");
	  break;
	case 3:
	  log(ERROR, "Error");
	  break;
	case 4:
	  log(ERROR, "Fatal Error");
	  break;
	}
	// TODO further decode

	break;

      case LR_MEMORY_DUMP: 
	log(INFO, "received memory dump");
	// TODO further decode
	break;

      case LR_AVERAGE_DATA: 
	log(INFO, "received average data");
	// TODO further decode
	break;

      case LR_DATA_RANGE: 
	log(INFO, "received data sub range");
	// TODO further decode
	break;

      case LR_LMI_CONF_DATA: 
	log(INFO, "received lmi configuration");
	// TODO further decode
	break;

      case LR_LMI_HANDLE_DEFS: 
	log(INFO, "received data definition");
	// TODO further decode
	break;

      case LR_LMS_VARIANT_CHANGE: 
	log(INFO, "received variant change");
	switch (message.getUnsignedByte(0)) {
	case 0: 
	  log(INFO, "variant change aborted");
	  break;
	case 1:
	  log(INFO, "variant change done");
	  break;
	}

	log(INFO, "angular scan range    :");
	cerr << message.getUnsignedShort(1) << endl;
	log(INFO, "single shot resolution:");
	cerr << message.getUnsignedShort(3) << endl;
      
	break;

      case LR_FIELD_CONF: 
	log(INFO, "received field configuration :");
	switch (message.getUnsignedByte(0)) {
	case 0: 
	  log(INFO, "field conf aborted");
	  break;
	case 1:
	  log(INFO, "field conf done");
	  break;
	}
	//  TODO further decode
	break;

      case LR_PASSWORD_CHANGE: 
	log(INFO, "received password change ack:");
	switch (message.getUnsignedByte(0)) {
	case 0: 
	  log(INFO, "password change aborted");
	  break;
	case 1:
	  log(INFO, "password change done");
	  break;
	case 2:
	  log(INFO, "acknowledge of user expected");
	  break;
	}
	switch (message.getUnsignedByte(1)) {
	case 0: 
	  log(INFO, "password for SICK-SERVICE and authorized customers");
	  break;
	case 1:
	  log(INFO, "password for maintenance");
	  break;
	  break;
	}
	break;
      case LR_FIELD_CONF_DATA: 
	log(INFO, "received field configuration data:");
	//  TODO further decode
	break;

      case LR_LEARN_FIELD_CONF: 
	log(INFO, "received learn field configuration :");
	switch (message.getUnsignedByte(0)) {
	case 0: 
	  log(INFO, "learning aborted");
	  break;
	case 1:
	  log(INFO, "done, verification can start");
	  break;
	case 3:
	  log(INFO, "learning active");
	  break;
	}
	log(INFO,"learning from LMS #:");
	cerr << message.getUnsignedByte(1) << endl;
	break;

      case LR_CONF_DYN_RECT: 
	log(INFO, "received configuration for dynamic rectangle:");
	//  TODO further decode
	break;

      case LR_CONF_DYN_SEGMENT: 
	log(INFO, "received dynamic segment configuration:");
	//  TODO further decode
	break;

      case LR_DYN_FIELD_INDEX_OR_SPEED: 
	log(INFO, "received field index or speed configuration :");
	switch (message.getUnsignedByte(0)) {
	case 0: 
	  log(INFO, "switch aborted");
	  break;
	case 1:
	  log(INFO, "switch done.");
	  break;
	}
	log(INFO,"Index of activated field #:");
	cerr << message.getUnsignedShort(1) << endl;
	break;

      case LR_SET_OUTPUT: 
	log(INFO, "received set output active/inactive :");
	switch (message.getUnsignedByte(0)) {
	case 0: 
	  log(INFO, "output switch aborted");
	  break;
	case 1:
	  log(INFO, "output switch done.");
	  break;
	}
	log(INFO,"output #:");
	cerr << message.getUnsignedShort(1) << endl;
	log(INFO,"active (1) or inactive (0) :");
	cerr << message.getUnsignedShort(2) << endl;
      
	break;

      case LR_LMI_INPUT: 
	log(INFO, "received input data (bit 0..3:A..D, 4:restart):");
	cerr << message.getUnsignedShort(0) << endl;
	break;

      case LR_SIMULATE_INPUT: 
	log(INFO, "received simulate input:");
	//  TODO further decode
	break;

      case LR_CALIBRATE: 
	log(INFO, "received calibrate:");
	switch (message.getUnsignedByte(0)) {
	case 0: 
	  log(INFO, "calibration not ok");
	  break;
	case 1:
	  log(INFO, "calibration ok");
	  break;
	}
	break;

      case LR_CHANGE_PERMANENT_BAUDRATE: 
	log(INFO, "received acknowledge of permanent baudrate change:");
	switch (message.getUnsignedByte(0)) {
	case 0: 
	  log(INFO, "change of baudrate not accepted");
	  break;
	case 1:
	  log(INFO, "change of baudrate accepted");
	  break;
	}
	switch (message.getUnsignedByte(1)) {
	case 0: 
	  log(INFO, "baudrate will be 9600 on poweron");
	  break;
	case 1:
	  log(INFO, "baudrate will be unchanged on poweron");
	  break;
	}
	break;

      case LR_CHANGE_ADDRESS: 
	log(INFO, "received acknowledge of address definition :");
	switch (message.getUnsignedByte(0)) {
	case 0: 
	  log(INFO, "change of address not accepted");
	  break;
	case 1:
	  log(INFO, "change of address accepted");
	  break;
	}
	log(INFO,"new address is :");
	cerr << message.getUnsignedByte(1) << endl;
	break;

      case LR_ACTIVATE_SENDER: 
	log(INFO, "received activate sender :");
	switch (message.getUnsignedByte(0)) {
	case 0: 
	  log(INFO, "activation of sender not accepted");
	  break;
	case 1:
	  log(INFO, "activation of sender accepted");
	  break;
	}
	log(INFO,"activation is :");
	cerr << message.getUnsignedByte(1) << endl;
	break;

      case LR_LMI_CONF: 
	log(INFO, "received LMI conf :");
	switch (message.getUnsignedByte(0)) {
	case 0: 
	  log(INFO, "LMI conf not accepted");
	  break;
	case 1:
	  log(INFO, "LMI conf accepted");
	  break;
	}
	log(INFO,"number of attached sensors :");
	cerr << message.getUnsignedByte(1) << endl;
	// TODO further decode 
	break;

      case LR_HANDLE_DEFS: 
	log(INFO, "received case definition :");
	// TODO further decode 
	break;
      case LR_PIXEL_ORIENTED: 
	log(INFO, "received pixel oriented mode acknowledge:");
	switch (message.getUnsignedByte(0)) {
	case 0: 
	  log(INFO, "state change not accepted");
	  break;
	case 1:
	  log(INFO, "state change accepted");
	  break;
	}
	switch (message.getUnsignedByte(1)) {
	case 0: 
	  log(INFO, "state : not pixel oriented");
	  break;
	case 1:
	  log(INFO, "state : pixel oriented");
	  break;
	}
	break;
      case LR_CARTESIAN: 
	log(INFO, "received cartesian scan:");
	break;

      case LR_CONF_LMS: 
	log(INFO, "received LMS configuration:");
	switch (message.getUnsignedByte(0)) {
	case 0: 
	  log(INFO, "conf not accepted");
	  break;
	case 1:
	  log(INFO, "conf accepted");
	  break;
	}
	break;


      default:
	cerr << __FILE__  << " : " 
	     << __LINE__ << ":" 
	     << __FUNCTION__ ; 
	log(WARNING, "() - Unhandled opcode in :");
	message.PrintDumpOn(cerr);
      }
  }
};
