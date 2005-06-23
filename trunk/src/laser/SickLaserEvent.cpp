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


#include "SickLaserEvent.h"
#include "SickLaserOpcodes.h"
#include "SickLaserMessage.h"

#include "miro/Exception.h"
#include "miro/Log.h"

#include <algorithm>

namespace Miro
{
  using std::string;

  //
  // LaserEvent is an object wrapper for the event handler 
  // registered with the reactor
  //
  LaserEvent::LaserEvent(LaserTask* laserTask_, LaserStatistic *laserStatistic_) :
    syncMutex(),
    ackNackMutex(),
    ackNackCond(ackNackMutex),
    stateMutex(),
    laserTask(laserTask_),
    message(new ACE_Message_Block(sizeof(Miro::LaserMessage))),
    laserStatistic(laserStatistic_)
  {
    MIRO_LOG_CTOR("Sick::LaserEvent");
    state = NOSYNC;
    data= 0;
    packData = 0;
    lastfd = -1;

  }

  //
  // Destructor
  //
  LaserEvent::~LaserEvent() 
  {	
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
  int LaserEvent::handle_input (ACE_HANDLE fd) {
    unsigned char thisChar;
    int tocopy;
    
    
    tcgetattr ( fd, &newtio);

    // the state machine can be reset from outside, so we have to 
    // acquire acces to it.
    stateMutex.acquire();

    lastfd = fd;

    ++(laserStatistic->events);

    buffLen0 = read( fd, buff0, BUFFSIZE / 2 );
    buffPos0 = 0;

    if (buffLen0 <= -1) {
      throw Miro::Exception("LaserEvent::handle_input: Error on Laser file descriptor");
    }  
    if (buffLen0 == 0) {    
      throw Miro::Exception("LaserEvent::handle_input: Laser fd was called, but 0 bytes for reading");
    }

    while (buffPos0 < buffLen0) {

      switch(state) {
      case NOSYNC: //////////////////////////////////////////////////
	thisChar = buff0[ buffPos0++ ];
	if ( thisChar == LO_STX ) {
	  //
	  // insert char in packet
	  *(message->wr_ptr()) = thisChar;  
	  message->wr_ptr(sizeof(char));    
	  // now we should not disrupt the laser by sending other stuff
	  syncMutex.acquire();
	  timeOfStx = ACE_OS::gettimeofday();
	  // --> SYNC
	  state = SYNC;
	} else if ( thisChar == LR_ACK ) {
	  // keep state NOSYNC
	  // remember the event and broadcast acknowledge
	  ackNack = true;
	  ackNackCond.broadcast();
	} else if ( thisChar == LR_NACK ) {
	  // keep state NOSYNC
	  // remember the event broadcast not acknowledge
	  ackNack = false;
	  ackNackCond.broadcast();
	} 
	break;
      case SYNC: //////////////////////////////////////////////////
	thisChar = buff0[ buffPos0++ ];
	// only accept packets to address 128, this will help resynching
	if ( thisChar == 0x80 ) { 
	  // insert char in packet
	  *(message->wr_ptr()) = thisChar;  
	  message->wr_ptr(sizeof(char));    
	  // --> ADDR
	  state = ADDR;
	} else {
	  laserStatistic->packetsWrongAddress++;
	  if (thisChar == LO_STX) {
	    // this is a shortcut, but it is nearly never taken
	    state = SYNC;
	    MIRO_LOG_OSTR(LL_WARNING, "  resynched on packetheader while expecting addr :"<<(unsigned int)(thisChar));
	  } else {
	    state = NOSYNC;
	    syncMutex.release();
	    // reset assembled packet
	    message->wr_ptr(message->base());
	  }
	}
	break;
      case ADDR: //////////////////////////////////////////////////
	thisChar = buff0[ buffPos0++ ];
	*(message->wr_ptr()) = thisChar;  // Copy character into message block
	message->wr_ptr(sizeof(char));    // Move beyond the inserted char
      
	// --> LENGTH1
	state = LENGTH1;
	lengthlow = thisChar;
	break;
      case LENGTH1: //////////////////////////////////////////////////
	thisChar = buff0[ buffPos0++ ];
	*(message->wr_ptr()) = thisChar;  // Copy character into message block
	message->wr_ptr(sizeof(char));    // Move beyond the inserted char

	// --> COPYMSG
	state = COPYMSG;

	packData = ((unsigned short)(lengthlow) | ((unsigned short)(thisChar) << 8)) + 2; // () important

	// here we set the blockking of the fd to the exact number of bytes, we are expecting
#if 0
	newtio.c_cc[VMIN]     = packData-(buffLen0-buffPos0);   // blocking read until whole packet is arrived
	
	if (tcsetattr(fd,TCSANOW,&newtio) < 0)
	  throw Miro::CException(errno, "LaserImpl::setBaudrate:tcsetattr " + string(strerror(errno)) );
#endif

	// rough sensibility check, to avoid long lossed with non synched data
	if (packData > 1000) {
	  MIRO_LOG_OSTR(LL_WARNING, "      ignored large package :"<<packData);
	  laserStatistic->packetsToLong++;

	  // move actual input to get place for the packetpart to be reparsed
	  memmove(buff0+message->length()-1,buff0+buffPos0,buffLen0-buffPos0);
	
	  // copy packet back from message block, but leave out the first byte that 
	  // was not a correct stx !
	  // this way we actually recreate the input as if we newly got it from read 
	  memcpy(buff0, message->base()+1,  message->length()-1 );
	  buffPos0 = 0;
	  buffLen0 += message->length()-1;

	  // reset assembled packet
	  message->wr_ptr(message->base());

	  // --> NOSYNC
	  state = NOSYNC;
	  packData = 0;
	  syncMutex.release();
	} 
	data = 0;
	break;

      case COPYMSG: //////////////////////////////////////////////////
	//      thisChar = buff0[buffPos0++]; !!!

	tocopy=std::min(buffLen0-buffPos0, packData-data);
	memcpy( message->wr_ptr(), buff0+buffPos0, tocopy );
	message->wr_ptr( tocopy );
	buffPos0 +=  tocopy;
	data += tocopy;

	// packet finished, get it away
	if (data == packData) {
	  laserStatistic->recvTime += (ACE_OS::gettimeofday() - timeOfStx);
	  // check whether the packet is ok
	  if ( ((LaserMessage*)message->base ())->checkCrc() ) {	

#ifdef THREADEDWORKING
	    // get the message on its way to the task
	    if (laserTask->putq(message) == -1) {
	      throw Miro::Exception("LaserEvent::handle_input: Couldn't put MessageBlock");
	    }
	    // reset assembled packet
	    message = new ACE_Message_Block(sizeof(Miro::LaserMessage));	
#endif
#ifndef THREADEDWORKING
	    laserTask->doPkt((LaserMessage*)message->base ());
	    // reset assembled packet
	    message->wr_ptr(message->base());
	    message->rd_ptr(message->base()); // only place where we need it
#if 0
	// here we set the blockking of the fd to a typical header size, but not too small
	newtio.c_cc[VMIN]     = 1;   // blocking read until whole packet is arrived
	
	if (tcsetattr(fd,TCSANOW,&newtio) < 0)
	  throw Miro::CException(errno, "LaserImpl::setBaudrate:tcsetattr " + string(strerror(errno)) );
#endif
#endif

	  } 
	  else {
	    MIRO_LOG(LL_ERROR,"        CRC ERRROR.");
	    laserStatistic->packetsCRCError++;
#define RESYNCONPARTS
#ifdef RESYNCONPARTS
	    // try to chop a reasonable part of the packet for resynch
	    // copy actual input to a safe place
	    memcpy(buff1,buff0,BUFFSIZE);
	    buffPos1 = buffPos0;
	    buffLen1 = buffLen0;

	    // copy packet back from message block, but leave out the first byte that 
	    // was not a correct stx !
	    // this way we actually recreate the input as if we newly got it from read 
	    memcpy(buff0, message->base()+1,  message->length()-1 );
	    buffPos0 = 0;
	    buffLen0 = message->length()-1;

	    // copy the unread rest of the inupt back at its new place
	    memcpy(buff0+buffLen0, buff1+buffPos1, buffLen1 - buffPos1);
	    buffLen0 +=  (buffLen1 - buffPos1);
#endif
	    // reset assembled packet
	    message->wr_ptr(message->base());
	  }
	  state = NOSYNC;
	  packData = 0;
	  data = 0;
	  syncMutex.release();
	}	 
	break;
      default: //////////////////////////////////////////////////
	throw Miro::Exception("LaserEvent::handle_input: unknown state in laserEvent.");
	break;
      }
    }

    stateMutex.release();
    return 0;
  }

  int LaserEvent::handle_close(ACE_HANDLE, ACE_Reactor_Mask) {

    MIRO_LOG(LL_NOTICE, "LaserEvent handle_close called.");
    return 0;
  }

  void LaserEvent::timeout() {
    MIRO_LOG(LL_ERROR, "timeout called, flushing buffers.");
    // acquire mutex for state machine, avoid rare synch problems
    stateMutex.acquire();
    // rest state machine and assembled message
    packData = 0;
    data = 0;
    state = NOSYNC;
    message->wr_ptr(message->base());
    message->rd_ptr(message->base()); // here we may need it or not ??

    // flush input buffer, we will have an easier resynch
    // now and the actual message is already gone
    if (lastfd != -1) {
      if (tcflush(lastfd, TCIFLUSH) < 0)
	MIRO_LOG(LL_ERROR,"could not flush buffer.");
    } else {
      MIRO_LOG(LL_ERROR, "could not flush buffer, because no lastfd set.");
    }
    stateMutex.release();
    syncMutex.release();
  }
};
