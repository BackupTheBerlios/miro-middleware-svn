// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999,2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors: 
//   Stefan Enderle, 
//   Stefan Sablatnoeg, 
//   Hans Utz
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "CanConnection.h"
#include "CanParameters.h"

// extern "C" 
// {
// #include "canlib.h"
#include "canmsg.h"
#include "canioctl.h"
// }

namespace
{
  // parameters for 
  // 250kbit 
  const unsigned char CAN_BRP   = 3;
  const unsigned char CAN_TSEG1 = 1;
  const unsigned char CAN_TSEG2 = 4;
  const unsigned char CAN_SJW   = 0;
};

namespace Can
{
  ACE_Time_Value Connection::canTimeOut(0, 1000);

  Connection::Connection(ACE_Reactor * _reactor, 
			 Miro::DevEventHandler * _eventHandler,
			 const Parameters& _parameters) :
    Super(_reactor, _eventHandler, _parameters)
  {
    init();
  }

  void
  Connection::init() const
  {
    CanConfig cfg;

    // can_ClearStatus(fd);
    if(ioctl(ioBuffer.get_handle(), CAN_CLEARSTAT, 0L))
      throw Miro::Exception("can_ClearStatus() ioctl error");
    
    // configure controller

    // can_GetConfig(fd,&cfg);
    if(ioctl(ioBuffer.get_handle(), CAN_GETCONFIG, (void*)&cfg))
      throw Miro::Exception("can_GetConfig() ioctl error\n");

    cfg.gmask = XTID_MASK;
    cfg.mask15 = 0L;		/* accept all */
    cfg.id15 = XTID;
    cfg.BRP = CAN_BRP;
    cfg.TSEG1 = CAN_TSEG1;
    cfg.TSEG2 = CAN_TSEG2;
    cfg.SJW = CAN_SJW;

    // can_SetConfig(fd,&cfg);
    if(ioctl(ioBuffer.get_handle(), CAN_SETCONFIG, (void*)&cfg))
      throw Miro::Exception("can_SetConfig() ioctl error\n");
  }

  void 
  Connection::write(const Message& message)
  {
    // is this sleep necessary ???
    // well, yes
    Miro::Guard guard(writeMutex);
    ACE_Time_Value time = ACE_OS::gettimeofday();
    ACE_Time_Value delta = time - lastWrite;
    if (delta < canTimeOut) {
      ACE_OS::sleep(canTimeOut - delta); // this is at least 10usec thanks to linux
      time = ACE_OS::gettimeofday();
    }

    int rc = ioBuffer.send_n(message.canMessage(), sizeof(canmsg));
    lastWrite = time;
    ACE_OS::sleep(canTimeOut);
    if (rc == -1)
      throw Miro::CException(errno, "Error writing can device.");
  }
};
