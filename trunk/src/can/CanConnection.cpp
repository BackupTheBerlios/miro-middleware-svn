// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
//
// (c) 1999,2000, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////


#include "CanConnection.h"
#include "Parameters.h"

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
  };

namespace Can
{
  ACE_Time_Value Connection::canTimeOut(0, 1000);

  Connection::Connection(ACE_Reactor * _reactor,
			 Miro::DevEventHandler * _eventHandler,
			 const Parameters& _parameters) :
    Super(_reactor, _eventHandler, _parameters),
    parameters_(_parameters)
  {
    init();
  }

  void
  Connection::init() const
  {

    if(parameters_.module == "Can"){
       CanConfig cfg;

       Message::driver=OLD;

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

    // Roland: kann das raus ???
   /* if(parameters_.module == "sja1000"){

       unsigned long baud_rate = B250;
       long long acode = -1, amask = -1;

       Message::driver=OLD;

       if(ioctl(ioBuffer.get_handle(), CAN_IOCSBAUD, &baud_rate))
         throw Miro::Exception("can_IOCSBAUD() ioctl error");

       if(acode != -1){
          if(ioctl(ioBuffer.get_handle(), CAN_IOCSACODE, &acode))
            throw Miro::Exception("can_IOCSACODE() ioctl error");
       }

       if(amask != -1){
          if(ioctl(ioBuffer.get_handle(), CAN_IOCSAMASK, &amask))
            throw Miro::Exception("can_IOCSAMASK() ioctl error");
       }

    }*/

    /* PCAN Driver by peak-system  http://www.peak-system.com */
    if(parameters_.module == "pcan") {
	TPCANInit caninit;
	caninit.wBTR0BTR1 = CAN_BAUD_250K; // Hier evtl. noch SJW1 und SJW0 setzen,
	                                   // d. h. Synchronization Jump Width
	caninit.ucCANMsgType = MSGTYPE_EXTENDED;
	caninit.ucListenOnly = 0;          // kein Listen only

        Message::driver = PCAN;
	if(ioctl(ioBuffer.get_handle(), PCAN_INIT , &caninit))
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
    int rc;
    if (parameters_.module == "pcan") {
       pcanmsg * msgp;
       message.canMessage(&msgp);
       msgp->Msg.MSGTYPE = MSGTYPE_EXTENDED;
       for(int i = 0; i < msgp->Msg.LEN; i++)
          std::cout << msgp->Msg.DATA[i];
       rc = ioctl(ioBuffer.get_handle(), PCAN_WRITE_MSG, msgp);
    }
    else {
       canmsg * msg;
       message.canMessage(&msg);
       rc = ioBuffer.send_n(msg, sizeof(canmsg));
    }

    lastWrite = time;

    writeMutex.release();
    if (rc == -1)
      throw Miro::CException(errno, "Error writing can device.");
  }

  void
  Connection::deadHandler(int)
  {
    static std::string errorMessage = "SparrowBoard write failed." \
      "\nThe SparrowBoard probably crashed.";
    std::cerr << endl << endl << errorMessage << endl << endl;
    abort();
    //    throw Miro::Exception(errorMessage);
  }
};
