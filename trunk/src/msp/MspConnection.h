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
#ifndef MspConnection_h
#define MspConnection_h

#include "abus/AbusConnection.h"

namespace Msp
{
  // forward declerations
  struct SonParmsType;
  class Parameters;

  class Connection : public Abus::Connection
  {
    typedef Abus::Connection Super;
  public:
    Connection(ACE_Reactor * _reactor, 
	       Miro::DevEventHandler * _eventHandler);
    virtual ~Connection();

    // server behaviour
    void sonarStart() throw(Miro::Exception);
    void sonarStop() throw(Miro::Exception);
    void irStart() throw() {};
    void irStop()  throw() {};
    void tactileStart() throw() {};
    void tactileStop()  throw() {};
  
    // possible commands to send to all msp devices
    void reqReset      (unsigned char devId) throw(Miro::Exception);
    void reqVer        (unsigned char devId) throw(Miro::Exception);

    void reqIrParms    (unsigned char devId) throw(Miro::Exception);
    void setIrParms    (unsigned char devId, unsigned short interval) throw(Miro::Exception);

    void reqSonarTable (unsigned char devId) throw(Miro::Exception);
    void setSonarTable (unsigned char devId, const unsigned long *table[]) throw(Miro::Exception);
    void reqSonar      (unsigned char devId, const unsigned long *list) throw(Miro::Exception);
				// This is used to get single  
				// readings from a single MSP. 
				// It is not typically used.   
				// See the mspterm.c courses   
				// for more info.              

    void reqSonarStart (unsigned char devId) throw(Miro::Exception);
    void reqSonarStop  (unsigned char devId) throw(Miro::Exception);
    void reqSonarParms (unsigned char devId) throw(Miro::Exception);
    void setSonarParms (unsigned char devId, const SonParmsType * parms) throw(Miro::Exception);
				// set any parms that you don't want 
				// to change to -1 (0xFFFF) 

    // protected:
    bool sonarsPinging;
    bool sonarsRequested;
    int masterMspBusId;

  private:
    //  private:
    static const unsigned long ping1[7];
    static const unsigned long ping2[7];
    static const unsigned long ping3[7];
    static const unsigned long ping4[7];

    static const unsigned long * globalSonarTable[5];
  };
};
#endif




