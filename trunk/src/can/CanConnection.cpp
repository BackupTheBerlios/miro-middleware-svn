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
#include "CanConnection.h"
#include "Parameters.h"
#include "PCanMessage.h"
#include "miro/Log.h"

// extern "C"
// {
// #include "canlib.h"
#include "canmsg.h"
#include "canioctl.h"
#include "can.h"
#include "pcan.h"     // pcan treiber

/* from peak-linux-driver-2.8/lib/libpcan.h */
// bitrate codes of BTR0/BTR1 registers
#define CAN_BAUD_1M     0x0014  //   1 MBit/s
#define CAN_BAUD_500K   0x001C  // 500 kBit/s
#define CAN_BAUD_250K   0x011C  // 250 kBit/s
#define CAN_BAUD_125K   0x031C  // 125 kBit/s
#define CAN_BAUD_100K   0x432F  // 100 kBit/s
#define CAN_BAUD_50K    0x472F  //  50 kBit/s
#define CAN_BAUD_20K    0x532F  //  20 kBit/s
#define CAN_BAUD_10K    0x672F  //  10 kBit/s
#define CAN_BAUD_5K     0x7F7F  //   5 kBit/s

// }

namespace
{
  // parameters for
  // 250kbit
  const unsigned char CAN_BRP   = 3;
  const unsigned char CAN_TSEG1 = 1;
  const unsigned char CAN_TSEG2 = 4;
  const unsigned char CAN_SJW   = 0;


  // We set an alarm for this many seconds before attempting to contact
  // the mcp. If the mcp is off, dead, etc the alarm will expire and
  // we can bag out.  If not, we reset the alarm and continue
  static const unsigned int INITIAL_WAIT_TIME = 5;
}

namespace Can
{
  ACE_Time_Value Connection::canTimeOut(0, 1000);

  Connection::Connection(ACE_Reactor * _reactor,
			 Miro::DevEventHandler * _eventHandler,
			 const Parameters& _parameters) :
    Super(_reactor, _eventHandler, _parameters),
    parameters_(_parameters)
  {
    initDevice();
  }

  void
  Connection::initDevice() const
  {

    MIRO_DBG_OSTR(SPARROW, LL_DEBUG, 
		  "CanConnection::initDevice() " << 
		  parameters_.module << " " << parameters_.device);

    if (parameters_.module == "Can") {
       CanConfig cfg;

       //Message::driver=OLD;

       // can_ClearStatus(fd);
       if(ioctl(ioBuffer_.get_handle(), CAN_CLEARSTAT, 0L))
         throw Miro::Exception("can_ClearStatus() ioctl error");

       // configure controller

       // can_GetConfig(fd,&cfg);
       if(ioctl(ioBuffer_.get_handle(), CAN_GETCONFIG, (void*)&cfg))
         throw Miro::Exception("can_GetConfig() ioctl error\n");

       cfg.gmask = XTID_MASK;
       cfg.mask15 = 0L;		/* accept all */
       cfg.id15 = XTID;
       cfg.BRP = CAN_BRP;
       cfg.TSEG1 = CAN_TSEG1;
       cfg.TSEG2 = CAN_TSEG2;
       cfg.SJW = CAN_SJW;

       // can_SetConfig(fd,&cfg);
       if(ioctl(ioBuffer_.get_handle(), CAN_SETCONFIG, (void*)&cfg))
         throw Miro::Exception("can_SetConfig() ioctl error\n");
    }


    /* PCAN Driver by peak-system  http://www.peak-system.com */
    else if (parameters_.module == "pcan") {
	TPCANInit caninit;
	caninit.wBTR0BTR1 = CAN_BAUD_250K; // Hier evtl. noch SJW1 und SJW0 setzen,
	                                   // d. h. Synchronization Jump Width
	caninit.ucCANMsgType = MSGTYPE_EXTENDED;
	caninit.ucListenOnly = 0;          // kein Listen only

        //Message::driver = PCAN;
	if(ioctl(ioBuffer_.get_handle(), PCAN_INIT , &caninit))
	  throw Miro::Exception("can PCAN_INIT() ioctl error");
    }
  }

  void
  Connection::write(Message& message)
  {
    ACE_Time_Value av(ACE_OS::gettimeofday() + ACE_Time_Value(1));

    if (writeMutex.acquire(av) == -1)
      throw Miro::CException(errno, "Error writing can device.");

    // Roland: Is this sleep necessary for Sparrow2003 can ???

    ACE_Time_Value time = ACE_OS::gettimeofday();
    ACE_Time_Value delta = time - lastWrite;
    if (delta < canTimeOut) {
      // is this sleep necessary ???
      // well, yes

      ACE_OS::sleep(canTimeOut - delta); // this is at least 10usec thanks to linux
      time = ACE_OS::gettimeofday();
    }

    // will definitely choke if base is off
    int rc = 0;
    if (parameters_.module == "pcan") {
       pcanmsg * msgp;
       message.canMessage((int **) &msgp);
       msgp->msg.msgtype = MSGTYPE_EXTENDED;
       rc = ioctl(ioBuffer_.get_handle(), PCAN_WRITE_MSG, msgp);
    }
    else {
       canmsg * msg;
       message.canMessage((int **) &msg);
       rc = ioBuffer_.send_n(msg, sizeof(canmsg));
    }

    lastWrite = time;

    writeMutex.release();
    if (rc == -1)
      throw Miro::CException(errno, "Error writing can device.");
  }
}
